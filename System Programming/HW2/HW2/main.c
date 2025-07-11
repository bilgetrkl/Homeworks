#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <syslog.h>

int child_counter = 0;
int num_children = 2;
int logfile;
int child1_pid, child2_pid;

int get_time(char *buffer, size_t size) {
    time_t now;
    struct tm *timeinfo;
    
    if (buffer == NULL || size < 20) {
        return -1;
    }
    
    time(&now);
    timeinfo = localtime(&now);
    
    if (timeinfo == NULL) {
        return -1;
    }
    
    if (strftime(buffer, size, "[%Y-%m-%d %H:%M:%S]", timeinfo) == 0) {
        return -1;
    }
    
    return 0;
}

void write_msg_to_log(char *msg) {
    char buffer[256];
    char time[30];
    get_time(time, sizeof(time));
    sprintf(buffer, "%s : %s\n", time, msg);
    write(logfile, buffer, strlen(buffer));
}

void sigchld_handler() {
    int status;
    pid_t pid;
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Child process %d exited with status %d", 
                pid, WEXITSTATUS(status));
        write_msg_to_log(log_msg);
        child_counter++;
    }
}

void daemon_signal_handler(int sig) {
    switch(sig) {
        case SIGUSR1:
            write_msg_to_log("Received SIGUSR1 signal");
            if (child1_pid > 0) kill(child1_pid, SIGTERM);
            if (child2_pid > 0) kill(child2_pid, SIGTERM);
            unlink("fifo1");
            unlink("fifo2");
            close(logfile);
            exit(0);
        case SIGHUP:
            write_msg_to_log("Received SIGHUP signal - Reloading configuration");
            if (child1_pid > 0) kill(child1_pid, SIGTERM);
            if (child2_pid > 0) kill(child2_pid, SIGTERM);
            unlink("fifo1");
            unlink("fifo2");
            close(logfile);
            exit(0);
        case SIGTERM:
            write_msg_to_log("Received SIGTERM signal - Shutting down");
            if (child1_pid > 0) kill(child1_pid, SIGTERM);
            if (child2_pid > 0) kill(child2_pid, SIGTERM);
            unlink("fifo1");
            unlink("fifo2");
            close(logfile);
            exit(0);
    }
}

void daemonize_the_process() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
        
    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0) {
        perror("setsid");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid > 0)
        exit(EXIT_SUCCESS);

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }
    umask(0);

    logfile = open("logfile", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (logfile < 0) {
        perror("open");
        exit(1);
    }

    int fd2 = open("/dev/null", O_RDWR);
    if (fd2 == -1) {
        close(logfile);
        perror("open");
        exit(1);
    }
    
    dup2(logfile, STDOUT_FILENO);
    dup2(logfile, STDERR_FILENO);
    dup2(fd2, STDIN_FILENO);
    close(fd2);
    write_msg_to_log("[SUCCESS]: Daemon process created successfully.");
}


void setup_signal_handler(int signum, void (*handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(signum, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}


void child_process1() {
    sleep(10);
    char log_msg[256];
    char cmd[20];
    int num1, num2, larger;
    
    int fifo1 = open("fifo1", O_RDWR | O_NONBLOCK);
    if (fifo1 < 0) {
        write_msg_to_log("[ERROR]: Child 1 can not open FIFO1");
        _exit(1);
    }
    
    if (read(fifo1, &num1, sizeof(int)) < 0) {
        close(fifo1);
        write_msg_to_log("[ERROR]: Child 1 can not read first number from FIFO1");
        _exit(1);
    }
    if (read(fifo1, &num2, sizeof(int)) < 0) {
        close(fifo1);
        write_msg_to_log("[ERROR]: Child 1 can not read second number from FIFO1");
        _exit(1);
    }
    if (read(fifo1, cmd, sizeof(cmd)) < 0) {
        close(fifo1);
        write_msg_to_log("[ERROR]: Child 1 can not read command from FIFO1");
        _exit(1);
    }
    if (strcmp(cmd, "larger") != 0) {
        close(fifo1);
        write_msg_to_log("[ERROR]: Child 1 received invalid command");
        _exit(1);
    }
    snprintf(log_msg, sizeof(log_msg), "[SUCCESS]: Child 1 Read numbers (%d, %d) from FIFO1, and command (%s)", 
             num1, num2, cmd);
    write_msg_to_log(log_msg);

  
    larger = (num1 > num2) ? num1 : num2;

    int fifo2 = open("fifo2", O_RDWR | O_NONBLOCK);
    if (fifo2 < 0) {
        write_msg_to_log("[ERROR]: Child 1 can not open FIFO2");
        close(fifo1);
        _exit(1);
    }

    if (write(fifo2, &larger, sizeof(int)) != sizeof(int)) {
        close(fifo2);
        write_msg_to_log("[ERROR]: Child 1 can not write the larger number to FIFO2");
        _exit(1);
    }
    
    snprintf(log_msg, sizeof(log_msg), "[SUCCESS]: Child 1 Wrote larger number (%d) to FIFO2", larger);
    write_msg_to_log(log_msg);
    close(fifo1);
    close(fifo2);
    _exit(0);
}

void child_process2() {
    sleep(10);
    char log_msg[256];
    int larger;
    
    int fifo2 = open("fifo2", O_RDWR);
    if (fifo2 < 0) {
        write_msg_to_log("[ERROR]: Child 2 can not open FIFO2");
        _exit(1);
    }
    
    write_msg_to_log("[SUCCESS]: Child 2 opened FIFO2");
    
    if (read(fifo2, &larger, sizeof(int)) != sizeof(int)) {
        close(fifo2);
        write_msg_to_log("[ERROR]: Child 2 can not read larger number from FIFO2");
        _exit(1);
    }
    
    snprintf(log_msg, sizeof(log_msg), "[SUCCESS]: Child 2 Read the larger number from FIFO2, larger number is: %d", larger);
    write_msg_to_log(log_msg);
    close(fifo2);
    _exit(0);
}

void setup_the_fifos() {
    if (mkfifo("fifo1", 0666) == -1) {
        write_msg_to_log("[ERROR]: FIFO1 could not created successfully.");
        _exit(1);
    }
    if (mkfifo("fifo2", 0666) == -1) {
        write_msg_to_log("[ERROR]: FIFO2 could not created successfully.");
        unlink("fifo1");
        _exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <integer1> <integer2>\n", argv[0]);
        return 1;
    }
    
    daemonize_the_process();
    setup_the_fifos();

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    char cmd[] = "larger";
    
    int fifo1 = open("fifo1", O_RDWR | O_NONBLOCK);
    if (fifo1 < 0) {
        write_msg_to_log("[ERROR]: Parent can not open FIFO1");
        unlink("fifo1");
        unlink("fifo2");
        exit(1);
    }
    
    write_msg_to_log("[SUCCESS]: Parent opened FIFO1");
    
    if (write(fifo1, &num1, sizeof(int)) != sizeof(int)) {
        write_msg_to_log("[ERROR]: Parent can not write first number to FIFO1");
        close(fifo1);
        unlink("fifo1");
        unlink("fifo2");
        exit(1);
    }
    if (write(fifo1, &num2, sizeof(int)) != sizeof(int)) {
        write_msg_to_log("[ERROR]: Parent can not write second number to FIFO1");
        close(fifo1);
        unlink("fifo1");
        unlink("fifo2");
        exit(1);
    }
    if (write(fifo1, cmd, sizeof(cmd)) != sizeof(cmd)) {
        write_msg_to_log("[ERROR]: Parent can not write command to FIFO1");
        close(fifo1);
        unlink("fifo1");
        unlink("fifo2");
        exit(1);
    }
    
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "[SUCCESS]: Parent wrote numbers (%d, %d) to FIFO1, and send command %s", 
             num1, num2, cmd);
    write_msg_to_log(log_msg);

    // Create first child
    child1_pid = fork();
    if (child1_pid < 0) {
        write_msg_to_log("[ERROR]: Error creating child 1");
        unlink("fifo1");
        unlink("fifo2");
        exit(1);
    }
    if (child1_pid == 0) {
        write_msg_to_log("[SUCCESS]: Child 1 has started");
        child_process1();
    }

    // Create second child
    child2_pid = fork();
    if (child2_pid < 0) {
        write_msg_to_log("[ERROR]: creating child 2");
        unlink("fifo1");
        unlink("fifo2");
        exit(EXIT_FAILURE);
    }
    if (child2_pid == 0) {
        write_msg_to_log("[SUCCESS]: Child 2 has started");
        child_process2();
    }

    setup_signal_handler(SIGHUP, daemon_signal_handler);
    setup_signal_handler(SIGTERM, daemon_signal_handler);
    setup_signal_handler(SIGUSR1, daemon_signal_handler);
    setup_signal_handler(SIGCHLD, sigchld_handler);

    // Parent process loop
    time_t start_time = time(NULL);
    while (child_counter < num_children) {
        sleep(2);
        write_msg_to_log("Proceeding...");
        
        if (time(NULL) - start_time > 30) {
            write_msg_to_log("[TIMEOUT]: Timeout reached, terminating children");
            if (child1_pid > 0) kill(child1_pid, SIGTERM);
            if (child2_pid > 0) kill(child2_pid, SIGTERM);
            unlink("fifo1");
            unlink("fifo2");
            close(logfile);
            exit(1);
        }
    }

    // Cleanup
    write_msg_to_log("[SUCCESS]: The process has done successfully. Daemon shutting down");
    close(logfile);
    close(fifo1);
    unlink("fifo1");
    unlink("fifo2");
    return 0;
}