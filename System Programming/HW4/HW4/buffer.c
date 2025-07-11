#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "buffer.h"

void buffer_init(Buffer *buffer, int capacity) { // buffer initialization
    buffer->capacity = capacity;
    buffer->size = 0;
    buffer->head = 0;
    buffer->tail = 0;
    buffer->eof = 0;
    
    // Allocate memory for the buffer
    buffer->lines = (char **)malloc(capacity * sizeof(char *));
    if (buffer->lines == NULL) { // if memory allocation fails
        perror("Cannot allocate buffer memory");
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
}

void buffer_put(Buffer *buffer, char *line) { // put line into buffer
    pthread_mutex_lock(&buffer->mutex);

    while (buffer->size == buffer->capacity && !buffer->eof) { // wait if buffer is full
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);
    }
    
    if (buffer->eof) { // if EOF has been reached, do not add more lines
        pthread_mutex_unlock(&buffer->mutex);
        return;
    }
    // Add line to the buffer
    buffer->lines[buffer->tail] = strdup(line);
    buffer->tail = (buffer->tail + 1) % buffer->capacity;
    buffer->size++;
    
    pthread_cond_signal(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}

char *buffer_get(Buffer *buffer) { // get line from buffer
    pthread_mutex_lock(&buffer->mutex);
    
    while (buffer->size == 0 && !buffer->eof) { // wait if buffer is empty
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
    }
    
    if (buffer->size == 0 && buffer->eof) { // if EOF has been reached and buffer is empty
        pthread_mutex_unlock(&buffer->mutex);
        return NULL;
    }
    
    // Get line from the buffer
    char *line = buffer->lines[buffer->head];
    buffer->head = (buffer->head + 1) % buffer->capacity;
    buffer->size--;
    
    pthread_cond_signal(&buffer->not_full);
    pthread_mutex_unlock(&buffer->mutex);
    
    return line;
}

void buffer_destroy(Buffer *buffer) { // destroy buffer
    pthread_mutex_lock(&buffer->mutex);
    
    // Free all lines in the buffer
    for (int i = 0; i < buffer->size; i++) {
        int idx = (buffer->head + i) % buffer->capacity;
        free(buffer->lines[idx]);
    }
    
    free(buffer->lines);
    
    pthread_mutex_unlock(&buffer->mutex);
    
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->not_full);
    pthread_cond_destroy(&buffer->not_empty);
}

void buffer_set_eof(Buffer *buffer) { // set EOF flag
    pthread_mutex_lock(&buffer->mutex);
    buffer->eof = 1;
    pthread_cond_broadcast(&buffer->not_empty); 
    pthread_cond_broadcast(&buffer->not_full); 
    pthread_mutex_unlock(&buffer->mutex);
}