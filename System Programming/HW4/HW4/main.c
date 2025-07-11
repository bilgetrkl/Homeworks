#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include "buffer.h"

volatile sig_atomic_t running = 1; // flag for signal handling
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for running flag

typedef struct {
    int id; // worker thread id
    char *term; // search term
    Buffer *buffer;
    int matches; // number of matches
    pthread_barrier_t *barrier;
    int worker_num; // number of workers
} w_args; // worker thread arguments

typedef struct {
    char *log_file;
    Buffer *buffer;
} m_args; // manager thread arguments

int read_line (int fd, char *buffer, int max_len) { // read a line from file descriptor
    char c;
    int bytes_read, i;
    i = 0;
    
    while (i < max_len - 1) {
        do {
            bytes_read = read(fd, &c, 1);
        } while (bytes_read == -1 && errno == EINTR);
        
        if (bytes_read <= 0) {
            if (i == 0) return bytes_read;
            break;
        }
        
        buffer[i++] = c;

        if (c == '\n') break;
    }
    
    buffer[i] = '\0';
    return i; 
}

void *worker_thread(void *arg) { // worker thread function
    w_args *args = (w_args *)arg;
    int local_matches = 0;
    char *line;
    bool still_running;

    // Wait for all worker threads to be created
    while (1) {
        pthread_mutex_lock(&running_mutex);
        still_running = running;
        pthread_mutex_unlock(&running_mutex);
        
        if (!still_running) break;
        
        line = buffer_get(args->buffer);
        
        if (line == NULL) break;
        
        char *found = strstr(line, args->term);
        while (found != NULL) {
            local_matches++;
            found = strstr(found + 1, args->term);
        }
        
        free(line); 
    }
    
    args->matches = local_matches;

    pthread_barrier_wait(args->barrier);
    
    printf("Worker %d found %d matches.\n", args->id, args->matches);

    // print the number of matches found by this worker thread
    if (pthread_barrier_wait(args->barrier) == PTHREAD_BARRIER_SERIAL_THREAD) { // only one thread will execute this
        int total_matches = 0;
        w_args *all_args = (w_args *)arg - args->id;
        for (int i = 0; i < args->worker_num; i++) {
            total_matches += all_args[i].matches;
        }
        printf("\nTotal matches found: %d\n", total_matches);
    }
    
    return NULL;
}

void *manager_thread(void *arg) { // manager thread function
    m_args *manager_args = (m_args *)arg;
    char *log_file = manager_args->log_file;
    Buffer *buffer = manager_args->buffer;
    bool still_running;
    
    int log_fd = open(log_file, O_RDONLY);
    if (log_fd < 0) { // 
        perror("Cannot open log file");
        buffer_set_eof(buffer);
        return NULL;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_length;

    // read lines from log file and put them into the buffer
    while (1) {
        pthread_mutex_lock(&running_mutex);
        still_running = running;
        pthread_mutex_unlock(&running_mutex);
        
        if (!still_running) break;
        
        line_length = read_line(log_fd, line, MAX_LINE_LENGTH);
        
        if (line_length <= 0) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        buffer_put(buffer, line);
    }
    
    buffer_set_eof(buffer);
    close(log_fd);
    return NULL;
}

void handle_sigint(int sig) { // signal handler for SIGINT
    (void) sig;
    pthread_mutex_lock(&running_mutex);
    running = 0;
    pthread_mutex_unlock(&running_mutex);
    printf("\nReceived SIGINT, shutting down...\n");
}

void setup_signal_handler() { // setup signal handler for SIGINT
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);
}

int main (int argc, char *argv[]) {
    if (argc != 5) { // check for correct number of arguments
        printf("Usage: %s <buffer_size> <num_workers> <log_file> <search_term>\n", argv[0]);
        return EXIT_FAILURE;
    }
 
    int buffer_size = atoi(argv[1]);
    int worker_num = atoi(argv[2]);
    char *log_file = argv[3];
    char *search_term = argv[4];
    
    if (buffer_size <= 0 || worker_num <= 0) { // error handling for invalid arguments
        fprintf(stderr, "Buffer size and number of workers must be positive integers\n"); 
        printf("Usage: %s <buffer_size> <num_workers> <log_file> <search_term>\n", argv[0]);
        return EXIT_FAILURE;
    }

    setup_signal_handler();
    
    Buffer buffer;
    buffer_init(&buffer, buffer_size); // buffer initialization

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, worker_num); // barrier initialization
    
    // some manager arguments initialization
    m_args manager_args;
    manager_args.log_file = log_file;
    manager_args.buffer = &buffer;

    // create manager thread
    pthread_t manager;
    if (pthread_create(&manager, NULL, manager_thread, &manager_args) != 0) { // if manager thread creation fails
        perror("Cannot create manager thread");
        buffer_destroy(&buffer);
        pthread_barrier_destroy(&barrier);
        return EXIT_FAILURE;
    }
    
    // create worker thread array
    pthread_t *workers = (pthread_t *)malloc(worker_num * sizeof(pthread_t));
    if (workers == NULL) { // if worker thread array creation fails
        perror("Cannot allocate worker threads array");
        buffer_destroy(&buffer);
        pthread_barrier_destroy(&barrier);
        return EXIT_FAILURE;
    }
    
    // create worker arguments array 
    w_args *worker_args = (w_args *)malloc(worker_num * sizeof(w_args));
    if (worker_args == NULL) { // if worker arguments array creation fails
        perror("Cannot allocate worker arguments array");
        buffer_destroy(&buffer);
        pthread_barrier_destroy(&barrier);
        free(workers);
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < worker_num; i++) {
        // initialize worker arguments
        worker_args[i].id = i;
        worker_args[i].term = search_term;
        worker_args[i].buffer = &buffer;
        worker_args[i].matches = 0;
        worker_args[i].barrier = &barrier;
        worker_args[i].worker_num = worker_num;
        
        // create worker threads
        if (pthread_create(&workers[i], NULL, worker_thread, &worker_args[i]) != 0) { // if worker thread creation fails
            perror("Cannot create worker thread");
            pthread_mutex_lock(&running_mutex);
            running = 0;
            pthread_mutex_unlock(&running_mutex);
            break;
        }
    }

    pthread_join(manager, NULL); // wait for manager thread to finish

    for (int i = 0; i < worker_num; i++) { // wait for worker threads to finish
        pthread_join(workers[i], NULL);
    }
    
    // cleanup
    buffer_destroy(&buffer);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&running_mutex);
    free(workers);
    free(worker_args);
    
    return EXIT_SUCCESS;
}