#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>

#define MAX_BUFFER 4096
#define MAX_USERNAME 17
#define MAX_ROOM_NAME 33
#define MAX_FILE_SIZE (3 * 1024 * 1024) 
#define MAX_FILE_QUEUE 5 
#define MAX_MESSAGE_SIZE 1024
#define MAX_RECEIVED_FILES 50

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_BOLD "\033[1m"

int client_socket = -1;
char username[MAX_BUFFER];
char current_room[MAX_ROOM_NAME];
volatile int running = 1;
int exit_flag = 0;
pthread_t receive_thread;

// array to track received files
char received_files[MAX_RECEIVED_FILES][MAX_BUFFER];
int received_files_count = 0;

void* receive_messages(void* arg);
void send_command(const char* command);
void cleanup_and_exit();
void signal_handler(int sig);
void handle_file_send(const char* filename, const char* target_user);
int file_already_received(const char* filename);
void add_received_file(const char* filename);
void display_command_menu();

#endif