#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_CLIENTS 1000

typedef struct { // struct to hold client request data
    char id[32];
    char op[32];
    char credit[32];
    char fifo[64];
} request;

request *global_req = NULL; 
int request_count = 0; 
int fifo_fd = 0; 

void client_fifo_rm (request req[]) { // remove client FIFOs
    for (int i = 0; i < request_count; i++) {
        unlink(req[i].fifo);
    }
}
void cleanup(int signum) { // cleanup function to handle signals
    printf("\nSignal %d received, cleaning up...\n", signum);

    if(fifo_fd != -1) 
        close (fifo_fd);
    if(global_req != NULL && request_count > 0) 
        client_fifo_rm (global_req);

    exit(EXIT_FAILURE);
}

void sig_setup(void (*handler)(int)) { // setup signal handlers
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
}

void server_fifo_open (const char *fifo_name) { // open server FIFO
    fifo_fd = open(fifo_name, O_WRONLY);
    if (fifo_fd == -1) {
        printf("Cannot connect %s\n", fifo_name);
        printf("exiting...\n");
        exit(EXIT_FAILURE);
    }
}

void read_file (const char *filename, request req[]) { // read client data from file
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    char line[256];

    printf("Reading %s\n", filename);
    while (fgets(line, sizeof(line), file) && request_count < MAX_CLIENTS) {
        if (sscanf(line, "%s %s %s", req[request_count].id, req[request_count].op, req[request_count].credit) == 3) {
            snprintf(req[request_count].fifo, sizeof(req[request_count].fifo), "client_fifo_%d", request_count);
            request_count++;
        }
    }
    fclose(file);

    printf("%d clients to connect...creating clients...\n", request_count);
}

void send_request (request req[]) { // create and send requests to server
    if (write(fifo_fd, &request_count, sizeof(int)) == -1) {
        perror("Cannot write request count to server FIFO");
        exit(EXIT_FAILURE);
    }

    int pid = getpid();
    if (write(fifo_fd, &pid, sizeof(int)) == -1) {
        perror("Cannot write client PID to server FIFO");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < request_count; i++) {
        if (mkfifo(req[i].fifo, 0666) == -1) {
            if (errno != EEXIST) {
                perror("Cannot create client FIFO");
                exit(EXIT_FAILURE);
            }
        }
        if (write(fifo_fd, &req[i], sizeof(request)) == -1) {
            perror("Cannot write client request to server FIFO");
            exit(EXIT_FAILURE);
        }
    }
}

void receive_response(request req[]) { // read responses from server
    for (int i = 0; i < request_count; i++) {
        int fd = open (req[i].fifo, O_RDONLY);
        if (fd == -1) {
            perror("Cannot open client FIFO for reading");
            continue;
        }

        char response_message[256];
        ssize_t n = read(fd, response_message, sizeof(response_message));
        if (n > 0) {
            response_message[n > 0 ? n - 1 : 0] = '\0';
            printf("%s\n", response_message);
        }
        else
            perror ("Cannot read response");

        close (fd);
    }

    for (int i = 0; i < request_count; i++) {
        int fd = open (req[i].fifo, O_RDONLY);
        if (fd == -1) {
            perror("Cannot open client FIFO for reading");
            continue;
        }

        char response_message[256];
        ssize_t n = read(fd, response_message, sizeof(response_message));
        if (n > 0) {
            response_message[n > 0 ? n - 1 : 0] = '\0';
            printf("%s\n", response_message);
        }
        else
            perror ("Cannot read response");

        close (fd);
    }
}

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ClientFile> <ServerFifoName>\n", argv[0]);
        return EXIT_FAILURE;
    }

    request req[MAX_CLIENTS];
    global_req = req;
    sig_setup(cleanup);

    read_file(argv[1], req);
    server_fifo_open(argv[2]);
    send_request(req);
    receive_response(req);
    client_fifo_rm(req);

    close(fifo_fd);

    return EXIT_SUCCESS;
}