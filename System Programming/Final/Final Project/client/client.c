#include "client.h"

int main(int argc, char* argv[]) {
    if (argc != 3) { // validates command line args
        printf(COLOR_RED "Usage: %s <server_ip> <port>\n" COLOR_RESET, argv[0]);
        return 1;
    }

    // Initialize received files array
    for (int i = 0; i < MAX_RECEIVED_FILES; i++) {
        received_files[i][0] = '\0';
    }
    received_files_count = 0;

    char* server_ip = argv[1];
    int port = atoi(argv[2]);

    if (port <= 0 || port > 65535) {
        printf(COLOR_RED "Error: Invalid port number\n" COLOR_RESET);
        return 1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);

    client_socket = socket(AF_INET, SOCK_STREAM, 0); // creates and configures client socket
    if (client_socket == -1) {
        perror(COLOR_RED "Socket creation failed" COLOR_RESET);
        return 1;
    }

    // sets up server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf(COLOR_RED "Error: Invalid IP address\n" COLOR_RESET);
        close(client_socket);
        return 1;
    }

    // try to connect to the server
    printf(COLOR_YELLOW "Connecting to server %s:%d...\n" COLOR_RESET, server_ip, port);
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { // if connection failed
        perror(COLOR_RED "Connection failed" COLOR_RESET);
        close(client_socket);
        return 1;
    }

    printf(COLOR_GREEN "Connected to server successfully!\n" COLOR_RESET);

    int username_accepted = 0;
    
    while (!username_accepted) { // loop until a valid username is accepted
        printf("Enter your username: ");
        if (fgets(username, sizeof(username), stdin) == NULL) { // reads username from stdin
            cleanup_and_exit();
        }
        username[strcspn(username, "\n")] = 0;
        
        send(client_socket, username, strlen(username), 0); // send username to server for validation

        // waits for server response
        char response[MAX_BUFFER];
        int bytes_received = recv(client_socket, response, sizeof(response) - 1, 0);
        if (bytes_received <= 0) { // if connection is lost
            printf(COLOR_RED "[Error]: Server disconnected during authentication.\n" COLOR_RESET);
            close(client_socket);
            return 1;
        }
        
        response[bytes_received] = '\0';
        
        if (strcmp(response, "USERNAME_TAKEN") == 0) { // checks if username is already taken
            printf(COLOR_RED "[Error] Username already taken. Choose another.\n" COLOR_RESET);
            continue;
        } else if (strcmp(response, "INVALID_USERNAME") == 0) { // checks if username is valid
            printf(COLOR_RED "[Error]: Invalid username!\n" COLOR_RESET);
            continue;
        } else if (strcmp(response, "WELCOME") == 0) { // checks if the server accepted the username
            printf(COLOR_GREEN "Login successful! Welcome %s\n" COLOR_RESET, username);
            username_accepted = 1;

            display_command_menu();
        } else if (strcmp(response, "SERVER_FULL") == 0) {
            printf(COLOR_RED "[Error]: Server is full!\n" COLOR_RESET);
            close(client_socket);
            return 1;
        } else {
            printf(COLOR_RED "[Error]: Unexpected server response: %s\n" COLOR_RESET, response);
            close(client_socket);
            return 1;
        }
    }

    // starts the receive thread to handle incoming messages
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror(COLOR_RED "Failed to create receive thread" COLOR_RESET);
        cleanup_and_exit();
    }

    char input[MAX_BUFFER];
    printf(COLOR_BOLD "> " COLOR_RESET);
    fflush(stdout);

    // sets stdin to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    while (running) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        struct timeval tv = {0, 10000};

        // checks if input is available
        int ready = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);

        if (ready > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
            if (fgets(input, sizeof(input), stdin) == NULL) // if input is avaliable, read 
                continue;

            input[strcspn(input, "\n")] = 0;
            
            if (strlen(input) == 0) {
                printf(COLOR_BOLD "> " COLOR_RESET);
                fflush(stdout);
                continue;
            }

            if (strncmp(input, "/", 1) == 0) { // checks if the input starts with a command
                char* command = strtok(input, " ");
                
                if (strcmp(command, "/exit") == 0) { // handles the /exit command
                    printf(COLOR_YELLOW "Disconnecting...\n" COLOR_RESET);
                    send_command(input); 
                    running = 0;
                    exit_flag = 1;
                    break;
                } else if (strcmp(command, "/join") == 0) { // handles the /join command
                    char* rest_of_command = input + strlen("/join ");
                    
                    while (*rest_of_command && isspace(*rest_of_command)) {
                        rest_of_command++;
                    }
                    
                    if (*rest_of_command == '\0') {
                        printf(COLOR_RED "Usage: /join <room_name>\n" COLOR_RESET);
                    } else {
                        char formatted_command[MAX_BUFFER];
                        snprintf(formatted_command, sizeof(formatted_command), "/join %s", rest_of_command);
                        send_command(formatted_command);
                    }
                } else if (strcmp(command, "/sendfile") == 0) { // handles the /sendfile command
                    char* filename = strtok(NULL, " ");
                    char* target_user = strtok(NULL, " ");
                    
                    if (filename == NULL || target_user == NULL) {
                        printf(COLOR_RED "Usage: /sendfile <filename> <username>\n" COLOR_RESET);
                    } else {
                        handle_file_send(filename, target_user);
                    }
                } else if (strcmp(command, "/leave") == 0) { // handles the /leave command
                    send_command(input);
                } else if (strcmp(command, "/broadcast") == 0) { // handles the /broadcast command
                    char* message = input + strlen("/broadcast ");
                    if (strlen(message) == 0) {
                        printf(COLOR_RED "Usage: /broadcast <message>\n" COLOR_RESET);
                    } else {
                        char formatted_command[MAX_BUFFER];
                        snprintf(formatted_command, sizeof(formatted_command), "/broadcast %s", message);
                        send_command(formatted_command);
                    }
                } else if (strcmp(command, "/whisper") == 0) { // handles the /whisper command
                    char* target_user = strtok(NULL, " ");
                    char* message = strtok(NULL, "");
                    
                    if (target_user == NULL || message == NULL) {
                        printf(COLOR_RED "Usage: /whisper <username> <message>\n" COLOR_RESET);
                    } else {
                        char formatted_command[MAX_BUFFER];
                        snprintf(formatted_command, sizeof(formatted_command), "/whisper %s %s", target_user, message);
                        send_command(formatted_command);
                    }
                } else if (strcmp(command, "/menu") == 0) { // handles the /menu command
                    display_command_menu();
                } else { // handles unknown commands
                    printf(COLOR_RED "[Error]: Unknown command: %s\n" COLOR_RESET, command);
                }
            } else {
                printf(COLOR_RED "[Error]: Commands must start with '/'.\n" COLOR_RESET);
            }

            if (running) {
                printf(COLOR_BOLD "> " COLOR_RESET);
                fflush(stdout);
            }
        }
        
        usleep(10000); 
    }

    fcntl(STDIN_FILENO, F_SETFL, flags); // restore original flags

    // waits for the receive thread to finish
    running = 0;
    pthread_join(receive_thread, NULL);
    if (client_socket != -1)
        close(client_socket);

    return 0;
}

void* receive_messages(void* arg) { // receives messages from the server
    (void)arg; 
    
    char buffer[MAX_BUFFER];
    int bytes_received;
    char message[MAX_MESSAGE_SIZE];

    while (running) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
        if (bytes_received <= 0) {
            if (running) {
                printf(COLOR_RED "[Error]: Server disconnected\n" COLOR_RESET);
                running = 0;
            }
            break;
        }

        buffer[bytes_received] = '\0';
        
        if (strncmp(buffer, "CHAT_MESSAGE ", 13) == 0) { // sends the chat messages
            sscanf(buffer + 13, "%[^\n]", message);
            printf(COLOR_GREEN "[Server]: You received a chat message.\n" COLOR_RESET);
            printf(COLOR_MAGENTA "%s\n" COLOR_RESET, message);        
        } else if (strcmp(buffer, "INVALID_ROOM_NAME") == 0) {
            printf(COLOR_RED "[Error]: Invalid room name!\n" COLOR_RESET);
        } else if (strncmp(buffer, "JOIN_ROOM", 9) == 0) { // successfully joined a room
            char room_name[MAX_ROOM_NAME] = {0};
            sscanf(buffer + 9, " %s", room_name);
            printf(COLOR_GREEN "[Server]: You joined the room '%s'\n" COLOR_RESET, room_name);
        } else if (strcmp(buffer, "ROOM_IS_FULL") == 0) { 
            printf(COLOR_RED "[Error]: The room is full!\n" COLOR_RESET);
        } else if (strcmp(buffer, "INVALID_ROOM") == 0) { 
            printf(COLOR_RED "[Error]: Room is invalid\n" COLOR_RESET);
        } else if (strcmp(buffer, "NOT_IN_ROOM") == 0) { 
            printf(COLOR_RED "[Error]: You are not in any room\n" COLOR_RESET);
        } else if (strcmp(buffer, "LEAVE_ROOM") == 0) { // successfully left the room
            printf(COLOR_GREEN "[Server]: You left the room\n" COLOR_RESET);
        } else if (strncmp(buffer, "MESSAGE_BROADCASTED", 19) == 0) { // message successfully broadcasted
            printf(COLOR_GREEN "[Server]: Message sent to room\n" COLOR_RESET);
        } else if (strcmp(buffer, "CANNOT_WHISPER_SELF") == 0) {
            printf(COLOR_RED "[Error]: You cannot whisper to yourself!\n" COLOR_RESET);
        } else if (strncmp(buffer, "WHISPER_MESSAGE", 15) == 0) { // sends a whisper message
            char sender[MAX_USERNAME];
            char *message_content = NULL;
            
            sscanf(buffer, "WHISPER_MESSAGE %s %[^\n]", sender, message);
            message_content = strchr(buffer + 16, ' ');
            if (message_content) message_content++;

            printf(COLOR_GREEN "[Server]: You received a whisper from '%s'\n" COLOR_RESET, sender);
            printf(COLOR_MAGENTA "[%s]: %s\n" COLOR_RESET, sender, message_content ? message_content : "");
        } else if (strncmp(buffer, "WHISPER_SENT", 12) == 0) { // whisper message sent successfully
            char target[MAX_USERNAME] = {0};
            sscanf(buffer + 12, " %s", target);
            printf(COLOR_GREEN "[Server]: Message sent to '%s'\n" COLOR_RESET, target);
        } else if (strcmp(buffer, "USER_NOT_FOUND") == 0) {
            printf(COLOR_RED "[Error]: User not found!\n" COLOR_RESET);
        } else if (strcmp(buffer, "LOGOUT") == 0) { // user logged out with /exit command
            printf(COLOR_BLUE "[Server]: Disconnected. Goodbye!\n" COLOR_RESET);
            running = 0;
            break;
        } else if (strcmp(buffer, "ALREADY_IN_ROOM") == 0) {
            printf(COLOR_RED "[Error]: You are already in this room.\n" COLOR_RESET);
        } else if (strcmp(buffer, "SERVER_SHUTDOWN") == 0) { // server is shutting down
            printf(COLOR_BLUE "[Info]: Server is shutting down.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Disconnecting...\n" COLOR_RESET);
            running = 0;
            break;
        } else if (strncmp(buffer, "USER_JOINED", 11) == 0) { // user joined a room notification
            char username[MAX_USERNAME] = {0};
            char room_name[MAX_ROOM_NAME] = {0};
            
            sscanf(buffer + 12, "%s %s", username, room_name);
            printf(COLOR_CYAN "[%s]: '%s' has joined the room\n" COLOR_RESET, room_name, username);
        } else if (strncmp(buffer, "USER_REJOINED", 13) == 0) { // user rejoined a room notification
            char username[MAX_USERNAME] = {0};
            char room_name[MAX_ROOM_NAME] = {0};
            
            sscanf(buffer + 14, "%s %s", username, room_name);
            printf(COLOR_CYAN "[%s]: '%s' has rejoined the room\n" COLOR_RESET, room_name, username);
        } else if (strncmp(buffer, "USER_LEFT", 9) == 0) { // user left a room notification
            char username[MAX_USERNAME] = {0};
            char room_name[MAX_ROOM_NAME] = {0};
            
            sscanf(buffer + 10, "%s %s", username, room_name);
            printf(COLOR_CYAN "[%s]: '%s' has left the room\n" COLOR_RESET, room_name, username);
        } else if (strcmp(buffer, "WRONG_EXTENSION") == 0) {
            printf(COLOR_RED "[Error]: File has an invalid extension.\n" COLOR_RESET);
        } else if (strncmp(buffer, "FILE_IS_LARGE", 13) == 0) {
            long file_size = 0;
            sscanf(buffer + 14, "%ld", &file_size);
            double size_in_mb = (double)file_size / (1024 * 1024);
            printf(COLOR_RED "[Error]: File is too large (%.2f MB).\n" COLOR_RESET, size_in_mb);
        } else if (strcmp(buffer, "SEND_TO_SELF") == 0) {
            printf(COLOR_RED "[Error]: You cannot send files to yourself.\n" COLOR_RESET);
        } else if (strcmp(buffer, "ALREADY_IN_ROOM") == 0) {
            printf(COLOR_RED "[Error]: You are already in this room.\n" COLOR_RESET);
        } else if (strcmp(buffer, "SERVER_SHUTDOWN") == 0) { // server is shutting down
            printf(COLOR_BLUE "[Info]: Server is shutting down.\n" COLOR_RESET);
            printf(COLOR_YELLOW "Disconnecting...\n" COLOR_RESET);
            running = 0;
            break;
        } else if (strcmp(buffer, "FILE_QUEUE_FULL") == 0) {
            printf(COLOR_YELLOW "[File Transfer]: Queue is currently full. Please wait...\n" COLOR_RESET);
        } else if (strcmp(buffer, "FILE_QUEUE_STILL_FULL") == 0) {
            printf(COLOR_RED "[File Transfer]: Queue is still full, please try again later.\n" COLOR_RESET);
        } else if (strncmp(buffer, "FILE_QUEUED", 11) == 0) {
            int position = 0;
            sscanf(buffer + 12, "%d", &position);
            printf(COLOR_CYAN "[File Transfer]: Your file has been added to the queue. Position: %d/%d\n" COLOR_RESET, position, MAX_FILE_QUEUE);
        } else if (strncmp(buffer, "FILE_TRANSFER_STARTED", 21) == 0) {
            char filename[MAX_BUFFER] = {0};
            int seconds = 0;
            sscanf(buffer + 22, "%s %d", filename, &seconds);
            printf(COLOR_GREEN "[File Transfer]: Your file '%s' started upload after %d seconds in queue.\n" COLOR_RESET, filename, seconds);
        } else if (strncmp(buffer, "FILE_RECEIVED", 13) == 0) {
            char sender[MAX_USERNAME] = {0};
            char filename[MAX_BUFFER] = {0};
            sscanf(buffer + 14, "%s %s", sender, filename);
            
            if (file_already_received(filename)) {
                printf(COLOR_YELLOW "[File Transfer]: You received file '%s' from '%s' (Note: You already have a file with this name)\n" COLOR_RESET, filename, sender);
            } else {
                printf(COLOR_GREEN "[File Transfer]: You received file '%s' from '%s'\n" COLOR_RESET, filename, sender);
                add_received_file(filename);
            }
        } else if (strncmp(buffer, "FILE_TRANSFER_COMPLETE", 22) == 0) {
            char filename[MAX_BUFFER] = {0};
            char receiver[MAX_USERNAME] = {0};
            sscanf(buffer + 23, "%s %s", filename, receiver);
            printf(COLOR_GREEN "[File Transfer]: File '%s' successfully sent to '%s'\n" COLOR_RESET,filename, receiver);
        } else if (strncmp(buffer, "FILE_TRANSFER_FAILED", 20) == 0) {
            char filename[MAX_BUFFER] = {0};
            char receiver[MAX_USERNAME] = {0};
            char reason[MAX_BUFFER] = {0};
            
            // extract filename and receiver, and possibly reason
            sscanf(buffer + 21, "%s %s %[^\n]", filename, receiver, reason);
            
            printf(COLOR_RED "[File Transfer]: Failed to send file '%s' to '%s'\n" COLOR_RESET, filename, receiver);
            if (strlen(reason) > 0) {
                printf(COLOR_RED "[Reason]: %s\n" COLOR_RESET, reason);
            }
        }

        if (running) {
            printf(COLOR_BOLD "> " COLOR_RESET);
            fflush(stdout);
        }
    }

    return NULL;
}

void send_command(const char* command) { // sends a command to the server
    if (send(client_socket, command, strlen(command), 0) < 0) {
        printf(COLOR_RED "[Error]: Failed to send command to server\n" COLOR_RESET);
    }
}

void cleanup_and_exit() { // cleans up resources and exits the client
    running = 0;
    
    if (client_socket != -1) { // if the socket is valid
        shutdown(client_socket, SHUT_RDWR);
    }
    
    if (receive_thread) { // wait for the receive thread to finish
        pthread_join(receive_thread, NULL);
    }
    
    exit(0);
}

void signal_handler(int sig) { // handles signals like SIGINT, SIGHUP, and SIGTERM
    if (sig == SIGINT) {
        printf(COLOR_YELLOW "\n\nReceived interrupt signal. Disconnecting...\n" COLOR_RESET);
        running = 0;
        cleanup_and_exit();
    } else if (sig == SIGHUP) {
        printf(COLOR_YELLOW "\n\nReceived hangup signal. Disconnecting...\n" COLOR_RESET);
        running = 0;
        cleanup_and_exit(); 
    } else if (sig == SIGTERM) {
        printf(COLOR_YELLOW "\n\nReceived termination signal. Disconnecting...\n" COLOR_RESET);
        running = 0;
        cleanup_and_exit();
    }
}

void handle_file_send(const char* filename, const char* target_user) { // handles the /sendfile command
    FILE* file = fopen(filename, "rb");
    if (file == NULL) { 
        printf(COLOR_RED "[Error]: File '%s' does not exist or cannot be read\n" COLOR_RESET, filename);
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fclose(file);
    
    char command_str[MAX_BUFFER];
    snprintf(command_str, sizeof(command_str), "/sendfile %s %.16s %ld", filename, target_user, file_size);
    
    send_command(command_str);
    
    printf(COLOR_CYAN "[File Transfer]: Requested to send '%s' (%ld Bytes) to '%s'\n" COLOR_RESET, filename, file_size, target_user);
}

int file_already_received(const char* filename) {
    for (int i = 0; i < received_files_count; i++) {
        if (strcmp(received_files[i], filename) == 0) {
            return 1; // File already exists
        }
    }
    return 0; // File doesn't exist yet
}

void add_received_file(const char* filename) {
    if (received_files_count < MAX_RECEIVED_FILES) {
        strncpy(received_files[received_files_count], filename, MAX_BUFFER - 1);
        received_files[received_files_count][MAX_BUFFER - 1] = '\0';
        received_files_count++;
    }
}

void display_command_menu() {
    printf("\n%sAvailable Commands%s\n", COLOR_CYAN, COLOR_RESET);
    printf(" %s/join%s <room_name>                      - Join a chat room\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/leave%s                                 - Leave current room\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/broadcast%s <message>                   - Send message to room\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/whisper%s <username> <message>          - Private message\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/sendfile%s <file> <user>                - Send file\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/menu%s                                  - Show this menu\n", COLOR_GREEN, COLOR_RESET);
    printf(" %s/exit%s                                  - Disconnect\n", COLOR_GREEN, COLOR_RESET);
}