#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define LOGFILE "Bank.log"
#define MAX_CLI 1000
#define MAX_REQ 1000

typedef struct {
    char id[32];
    char op[32];
    char credit[32];
    char fifo[64];
} request;

typedef struct {
    char response[MAX_REQ][256];
    request requests[MAX_REQ];
    int accounts[MAX_CLI][4];
    int active;
    int b_id;
} shared;

typedef struct {
    int fifo_fd;
    int shmem_id;
    int active;
    int client_count;
} server;

sem_t acc_lock;
sem_t teller_lock;
sem_t req_lock;
sem_t sh_lock;

server srv;
shared *sh_ptr;
char fifo_path[128];
char msg_buffer[256];
 
void create_shm(const char *path) { // create shared memory
    key_t key = ftok(path, 1); 
    if (key == -1) { perror("ftok"); exit(EXIT_FAILURE); }
    srv.shmem_id = shmget(key, sizeof(shared), IPC_CREAT | 0666);
    if (srv.shmem_id == -1) { perror("shmget"); exit(EXIT_FAILURE); }
    sh_ptr = (shared *)shmat(srv.shmem_id, NULL, 0);
    if (sh_ptr == (void *)-1) { perror("shmat"); exit(EXIT_FAILURE); }
}

void cleanup_all() { // cleanup all resources
    printf("Removing ServerFIFO...");
    shmdt(sh_ptr);
    shmctl(srv.shmem_id, IPC_RMID, NULL);
    close(srv.fifo_fd);
    sem_destroy(&acc_lock);
    sem_destroy(&teller_lock);
    sem_destroy(&req_lock);
    sem_destroy(&sh_lock);
    printf("Updating log file...\n");
    unlink(fifo_path);
    printf("Bank says \"Bye\"...\n");
}

void write_log() { // write log to file
    int fd = open(LOGFILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("Cannot open log file"); return; }

    sem_wait(&acc_lock);
    for (int i = 1; i <= sh_ptr->active; i++) {
        int len = snprintf(msg_buffer, sizeof(msg_buffer), "BankID_%02d D %d W %d %d\n",
                           i, sh_ptr->accounts[i][1], sh_ptr->accounts[i][2], sh_ptr->accounts[i][3]);
        write(fd, msg_buffer, len);
    }
    int len = snprintf(msg_buffer, sizeof(msg_buffer), "## end of log.\n");
    write(fd, msg_buffer, len);
    sem_post(&acc_lock);

    close(fd);
}

void sig_handler(int sig) { // handles signals
    (void)sig;
    write_log();
    printf("\nSignal received closing active Tellers\n");
    cleanup_all();
    exit(EXIT_SUCCESS);
}

int is_numeric(char *s) { // check if string is numeric
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i]))
            return 0;
    return 1;
}

int verify(request r) { // verify request
    if (strlen(r.op) == 0 || strlen(r.credit) == 0 || strlen(r.id) == 0 || strlen(r.fifo) == 0)
        return -2;
    if (strcmp(r.op, "deposit") != 0 && strcmp(r.op, "withdraw") != 0)
        return -3;
    if (!is_numeric(r.credit))
        return -3;
    if (atoi(r.credit) < 0)
        return -4;
    if (strcmp(r.id + 7, "None") == 0)
        return -5;
    return 0;
}

void reply_client(char path[]) { // reply to client
    int fd = open(path, O_WRONLY);
    if (fd == -1) { perror("Cannot open client fifo"); return; }
    if (write(fd, msg_buffer, sizeof(msg_buffer)) == -1)
        perror("Cannot write to client fifo");
    close(fd);
}

void load_db() { // load database from log file
    int fd = open(LOGFILE, O_RDONLY);
    if (fd == -1) {
        printf("No previous logs.. Creating the bank database\n");
        return;
    }

    char buffer[4096];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("Cannot read log file");
        close(fd);
        return;
    }
    buffer[bytes_read] = '\0'; 

    int id, deposit, withdraw, balance;
    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        if (line[0] == '#') {
            line = strtok(NULL, "\n");
            continue;
        }
        if (sscanf(line, "BankID_%02d D %d W %d %d", &id, &deposit, &withdraw, &balance) == 4) {
            sh_ptr->accounts[id][0] = id;
            sh_ptr->accounts[id][1] = deposit;
            sh_ptr->accounts[id][2] = withdraw;
            sh_ptr->accounts[id][3] = balance;
            if (id > sh_ptr->active)
                sh_ptr->active = id;
        }
        line = strtok(NULL, "\n");
    }

    close(fd);
}

void assign_bankid(int idx) { // assign bank ID to client
    sem_wait(&sh_lock);
    char *idstr = sh_ptr->requests[idx].id;
    if (strcmp(idstr, "N") == 0) {
        int id = ++(sh_ptr->active);
        sh_ptr->accounts[id][0] = id;
        sh_ptr->b_id = id;
        sh_ptr->accounts[id][3] = 0;
    } else {
        int id = atoi(idstr + 7);
        if (id < 0 || id > sh_ptr->active) {
            snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d invalid bank ID, %d", idx + 1, id);
            reply_client((char *)sh_ptr->requests[idx].fifo);
            exit(EXIT_FAILURE);
        }
        sh_ptr->b_id = id;
    }
    sem_post(&sh_lock);
}

void process_transaction(void *arg, int is_deposit) { // deposit or withdraw process
    int idx = *(int *)arg;
    sem_wait(&sh_lock);
    request r = sh_ptr->requests[idx];
    int amount = atoi(r.credit);
    int id = sh_ptr->b_id;
    if (is_deposit) {
        sh_ptr->accounts[id][3] += amount;
        sh_ptr->accounts[id][1] += amount;
        snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d served.. BankID_%02d", idx + 1, id);
        reply_client((char *)r.fifo);
        snprintf(sh_ptr->response[idx], 256, "Client%02d deposited %d credits.. updating log", idx + 1, amount);
    } else {
        if (sh_ptr->accounts[id][3] >= amount) {
            sh_ptr->accounts[id][3] -= amount;
            sh_ptr->accounts[id][2] += amount;

            if (sh_ptr->accounts[id][3] == 0) {
                snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d served.. BankID_%02d... account closed", idx + 1, id);
                snprintf(sh_ptr->response[idx], 256, "Client%02d withdraws %d credits.. updating log Bye Client%02d", idx + 1, amount, idx+1);
            }
            else {
                snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d served.. BankID_%02d", idx + 1, id);
                snprintf(sh_ptr->response[idx], 256, "Client%02d withdraws %d credits.. updating log", idx + 1, amount);
            }
            reply_client((char *)r.fifo);
        } else {
            snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d something went WRONG..", idx + 1);
            reply_client((char *)r.fifo);
            snprintf(sh_ptr->response[idx], 256, "Client%02d withdraws %d credits.. operation not permitted", idx + 1, amount);
            sem_post(&sh_lock);
            exit(EXIT_FAILURE);
        }
    }
    sem_post(&sh_lock);
}

pid_t Teller(void *func, void *arg) { // create teller process
    pid_t p = fork();
    if (p == 0) {
        int id = *(int *)arg;
        printf("--Teller PID%d is active serving Client%02d\n", getpid(), id + 1);
        sem_wait(&sh_lock);
        int check = verify(sh_ptr->requests[id]);
        sem_post(&sh_lock);
        if (check) {
            sem_wait(&sh_lock);
            snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d served.. something went WRONG", id + 1);
            snprintf(sh_ptr->response[id], 256, "Client%02d %s %d credits.. something went WRONG", id + 1, sh_ptr->requests[id].op, atoi(sh_ptr->requests[id].credit));
            reply_client((char *)sh_ptr->requests[id].fifo);
            sem_post(&sh_lock);
            exit(EXIT_FAILURE);
        }
        assign_bankid(id);
        void (*operation)(void *) = func;
        operation(arg);
        exit(EXIT_SUCCESS);
    }
    return p;
}

int waitTeller(pid_t pid, int *status) { return waitpid(pid, status, 0) == pid ? 0 : -1;} // wait for teller process

void handle_deposit(void *arg) { process_transaction(arg, 1); } // deposit handler

void handle_withdraw(void *arg) { process_transaction(arg, 0); } // withdraw handler

void bank_loop() { // main server loop
    while (1) {
        int request_count, ppid;
        printf("Waiting for clients %s\n", fifo_path);

        if (read(srv.fifo_fd, &request_count, sizeof(int)) <= 0) continue;
        if (read(srv.fifo_fd, &ppid, sizeof(int)) <= 0) continue;

        printf("-Received %d requests from PID:%d \n", request_count, ppid);
        request req_arr[request_count];
        bzero(req_arr, sizeof(req_arr));
        int base_id = srv.client_count;

        for (int i = 0; i < request_count; i++) {
            if (read(srv.fifo_fd, &req_arr[i], sizeof(request)) <= 0) {
                perror("Cannot read client request");
                continue;
            }
            srv.client_count++;
            snprintf(msg_buffer, sizeof(msg_buffer), "Client%02d connected.. %sing %d credits", i + base_id + 1, req_arr[i].op, atoi(req_arr[i].credit));
            sem_wait(&sh_lock);
            sh_ptr->requests[i + base_id] = req_arr[i];
            sem_post(&sh_lock);
            reply_client(req_arr[i].fifo);
        }

        int tellers[request_count];
        for (int i = 0; i < request_count; i++) {
            int id = i + base_id;
            void (*task)(void *) = strcmp(req_arr[i].op, "deposit") == 0 ? handle_deposit : handle_withdraw;
            tellers[i] = Teller(task, &id);
        }

        for (int i = 0; i < request_count; i++) {
            int status;
            waitTeller(tellers[i], &status);
        }

        for (int i = 0; i < request_count; i++) {
            int id = i + base_id;
            printf("%s\n", sh_ptr->response[id]);
        }

        write_log();
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Try Again%s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sem_init(&acc_lock, 1, 1);
    sem_init(&teller_lock, 1, 1);
    sem_init(&req_lock, 1, 1);
    sem_init(&sh_lock, 1, 1);

    strncpy(fifo_path, argv[1], sizeof(fifo_path));
    create_shm("server.c");
    signal(SIGINT, sig_handler);

    if (mkfifo(fifo_path, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    srv.fifo_fd = open(fifo_path, O_RDWR);
    if (srv.fifo_fd == -1) {
        perror("Cannot open server FIFO");
        exit(EXIT_FAILURE);
    }

    printf("Bank is active..\n");
    load_db();
    bank_loop();
    cleanup_all();
    return 0;
}