#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <ctype.h>

#define MAX_CLIENTS 100
#define MAX_ROOMS 50
#define MAX_USERS_PER_ROOM 15
#define MAX_USERNAME_LEN 16
#define MAX_ROOM_NAME_LEN 32
#define MAX_MESSAGE_LEN 1024
#define BUFFER_SIZE 4096
#define LOG_FILE "log.txt"
#define MAX_FILE_QUEUE 5
#define MAX_FILE_WAIT_TIME 20
#define MAX_RECEIVED_FILES 50

typedef struct { // client information
    int socket;
    char username[MAX_USERNAME_LEN + 1];
    char current_room[MAX_ROOM_NAME_LEN + 1];
    char previously_joined_rooms[MAX_ROOMS][MAX_ROOM_NAME_LEN + 1]; // Track rooms a client has previously joined
    pthread_t thread_id;
    int active;
    struct sockaddr_in address;
    int num_previously_joined_rooms;
    
    // Track received files
    char received_files[MAX_RECEIVED_FILES][BUFFER_SIZE];
    int received_files_count;
} Client;

typedef struct { // room information
    char name[MAX_ROOM_NAME_LEN + 1];
    Client *users[MAX_USERS_PER_ROOM];
    int user_count;
    int active;
} Room;

typedef struct { // file transfer request structure
    Client *sender;
    Client *receiver;
    char filename[BUFFER_SIZE];
    long file_size;
    time_t queue_time;
    int active;
} FileTransferRequest;

Client clients[MAX_CLIENTS];
Room rooms[MAX_ROOMS];

FileTransferRequest file_queue[MAX_FILE_QUEUE];
int queue_size = 0;
int queue_head = 0;
int queue_tail = 0;

sem_t empty_slots;   
sem_t filled_slots; 
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_t file_processor_thread;

int server_socket;
int client_count = 0;
int room_count = 0;
volatile int server_running = 1;
int lost_connection = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rooms_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
FILE *log_file;

void *client_handler(void *arg);
void *file_queue_processor(void *arg);
void log_event(const char *type, const char *message);
void broadcast_to_room(const char *room_name, const char *message, int sender_socket);
Client *find_client_by_username(const char *username);
Client *find_client_by_socket(int socket);
Room *find_room(const char *room_name);
Room *create_room(const char *room_name);
int join_room(Client *client, const char *room_name);
int leave_room(Client *client);
int is_valid_username(const char *username);
int is_valid_room_name(const char *room_name);
void handle_sigint(int sig);
void shutdown_server();
void cleanup_client(Client *client);
void send_to_client(int socket, const char *message);
void handle_file_transfer(Client *client, const char *filename, const char *target_username, long file_size);
int add_to_file_queue(Client *sender, Client *receiver, const char *filename, long file_size);
void process_file_transfer(FileTransferRequest *request);
int file_exists_for_receiver(Client *receiver, const char *filename);
void add_file_to_receiver(Client *receiver, const char *filename);

#endif