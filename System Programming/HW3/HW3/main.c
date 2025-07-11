#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SATELLITES 10
#define ENGINEERS 3
#define MAX_TIMEOUT 10

typedef struct {
    int id;
    int priority;
    const char* priority_type;
    time_t arrival_time;
    int being_served;  
    int engineer_id;  
} satellite_request;

pthread_mutex_t engineerMutex;
sem_t newRequest;
sem_t requestHandled;   

int availableEngineers = ENGINEERS;

satellite_request* requestQueue[SATELLITES];
int queue_size = 0;

void add_to_queue(satellite_request* req) {
    int i = queue_size - 1;

    while (i >= 0) {
        int p1 = requestQueue[i]->priority;
        int p2 = req->priority;
        int a1 = requestQueue[i]->arrival_time;
        int a2 = req->arrival_time;

        if (p1 < p2) break; // lower number means same priority

        if (p1 == p2 && a1 <= a2) break; // if the priority is the same

        requestQueue[i + 1] = requestQueue[i];
        i--;
    }

    requestQueue[i + 1] = req;
    queue_size++;
}

satellite_request* pop_from_queue() { // gets highest priority satellite request from the queue
    if (queue_size == 0) return NULL;
    satellite_request* req = requestQueue[0];

    for (int i = 1; i < queue_size; i++) {
        requestQueue[i - 1] = requestQueue[i];
    }

    queue_size--;
    return req;
}

int getSatellitePriority(int id) {
    int priority = rand() % SATELLITES + 1; 
    return priority;
}

void* satellite(void* arg) {
    int id = *((int*)arg);
    
    satellite_request* req = malloc(sizeof(satellite_request));
    req->id = id;
    req->priority = getSatellitePriority(id);
    req->arrival_time = time(NULL);
    req->being_served = 0;
    req->engineer_id = -1;
    
    pthread_mutex_lock(&engineerMutex);
    printf("[SATELLITE] Satellite %d requesting (priority %d)\n", id, req->priority);
    pthread_mutex_unlock(&engineerMutex);

    pthread_mutex_lock(&engineerMutex);
    add_to_queue(req);
    pthread_mutex_unlock(&engineerMutex);

    sem_post(&newRequest);

    int time_out = rand() % MAX_TIMEOUT + 1;
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += time_out;
    
    int connected = 0;
    int timed_out = 0;
    
    while (!connected && !timed_out) {
        pthread_mutex_lock(&engineerMutex);
        if (req->being_served) { // if this satellite has being served by an engineer
            connected = 1;
        }
        pthread_mutex_unlock(&engineerMutex);
        
        if (!connected) {
            struct timespec now;
            clock_gettime(CLOCK_REALTIME, &now);
            if (now.tv_sec >= timeout.tv_sec) { // checks if timeout has occurred
                timed_out = 1;
                printf("[TIMEOUT] Satellite %d timeout %d second.\n", id, time_out);
                
                pthread_mutex_lock(&engineerMutex);
                for (int i = 0; i < queue_size; i++) {
                    if (requestQueue[i]->id == id && !requestQueue[i]->being_served) {
                        free(requestQueue[i]);
                        requestQueue[i] = NULL;
                        for (int j = i; j < queue_size - 1; j++) {
                            requestQueue[j] = requestQueue[j + 1];
                        }
                        queue_size--;
                        break;
                    }
                }
                pthread_mutex_unlock(&engineerMutex);
            } else {
                usleep(100000); 
            }
        }
    }
    
    if (connected) {
        sleep(2); // Simulate update process
    }
    pthread_exit(NULL);
}

void* engineer(void* arg) {
    int id = *((int*)arg);
    
    while (1) {
        sem_wait(&newRequest); // waits for new request

        if (queue_size > 0 && availableEngineers > 0) {
            satellite_request* req = pop_from_queue(); // gets highest priority satellite request
            pthread_mutex_lock(&engineerMutex);
            printf("[ENGINEER %d] Handling Satellite %d (Priority %d)\n", id, req->id, req->priority);
            pthread_mutex_unlock(&engineerMutex);

            pthread_mutex_lock(&engineerMutex);
            availableEngineers--;
            req->being_served = 1;
            req->engineer_id = id;
            
            sem_post(&requestHandled); 
            pthread_mutex_unlock(&engineerMutex);

            sleep(3); 

            pthread_mutex_lock(&engineerMutex);
            availableEngineers++;
            pthread_mutex_unlock(&engineerMutex);
            pthread_mutex_lock(&engineerMutex);
            printf("[ENGINEER %d] Finished Satellite %d\n", id, req->id);
            pthread_mutex_unlock(&engineerMutex);
            free(req);
            
            if (queue_size == 0) { // if there are not more requests in the queue
                printf("[ENGINEER %d] Exiting...\n", id);
                sleep (1);
                pthread_mutex_lock(&engineerMutex);
                for (int i = 0; i < ENGINEERS - 1; i++) {
                    sem_post(&newRequest); // wakes up other engineers
                }
                pthread_mutex_unlock(&engineerMutex);
                break;
            }
        } else {
            if (queue_size == 0) { // if there are not more requests in the queue
                printf("[ENGINEER %d] Exiting...\n", id);
                sleep (1);
                break;
            }
        } 
    }
    pthread_exit(NULL);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL));

    pthread_t engineerThreads[ENGINEERS]; 
    pthread_t satelliteThreads[SATELLITES];

    int engineerIds[ENGINEERS];
    int satelliteIds[SATELLITES];

    // initializes semaphores and mutex
    pthread_mutex_init(&engineerMutex, NULL); 
    sem_init(&newRequest, 0, 0);
    sem_init(&requestHandled, 0, 0);

    for (int i = 0; i < ENGINEERS; i++) { // creates engineer threads
        engineerIds[i] = i;
        pthread_create(&engineerThreads[i], NULL, engineer, &engineerIds[i]);
    }
    
    for (int i = 0; i < SATELLITES; i++) { // creates satellite threads
        satelliteIds[i] = i;
        usleep(50000); 
        pthread_create(&satelliteThreads[i], NULL, satellite, &satelliteIds[i]);
    }

    for (int i = 0; i < SATELLITES; i++) { // waits for satellite threads to finish
        pthread_join(satelliteThreads[i], NULL);
    }

    for (int i = 0; i < ENGINEERS; i++) { // waits for engineer threads to finish
        pthread_join(engineerThreads[i], NULL);
    }

    pthread_mutex_destroy(&engineerMutex);
    sem_destroy(&newRequest);
    sem_destroy(&requestHandled);

    return 0;
}