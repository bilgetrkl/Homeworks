#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include <signal.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    char **lines;
    int capacity;
    int size;             
    int head;          
    int tail;        
    pthread_mutex_t mutex;
    pthread_cond_t not_full;    
    pthread_cond_t not_empty;  
    int eof; // if EOF has been reached
} Buffer; // circular buffer structure

void buffer_init(Buffer *buffer, int capacity);
void buffer_put(Buffer *buffer, char *line);
char *buffer_get(Buffer *buffer);
void buffer_destroy(Buffer *buffer);
void buffer_set_eof(Buffer *buffer);

#endif