#include "server.h"

int main(int argc, char *argv[]) {
    if (argc != 2) { // validates command line args
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number\n");
        exit(EXIT_FAILURE);
    }

    log_file = fopen(LOG_FILE, "a");
    if (!log_file) { // opens log file
        perror("fopen log file");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint);

    server_socket = socket(AF_INET, SOCK_STREAM, 0); // creates and configures server socket
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // sets up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // initializes file queue
    queue_size = 0;
    queue_head = 0;
    queue_tail = 0;
    for (int i = 0; i < MAX_FILE_QUEUE; i++) {
        file_queue[i].active = 0;
    }
    
    // initializes semaphores for file transfer queue
    sem_init(&empty_slots, 0, MAX_FILE_QUEUE);  
    sem_init(&filled_slots, 0, 0);             

    // initializes received files arrays for each client
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].received_files_count = 0;
        for (int j = 0; j < MAX_RECEIVED_FILES; j++) {
            clients[i].received_files[j][0] = '\0';
        }
    }

    // creates file processor thread
    if (pthread_create(&file_processor_thread, NULL, file_queue_processor, NULL) != 0) {
        perror("Failed to create file processor thread");
        exit(EXIT_FAILURE);
    }

    log_event("INFO", "File queue processor started");

    // log server startup
    printf("[INFO] Server listening on port %d...\n", port);
    char log_msg[512];
    snprintf(log_msg, sizeof(log_msg), "Server started on port %d", port);
    log_event("INFO", log_msg);

    // main server loop
    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            if (!server_running) {
                break; 
            }
            perror("accept");
            continue;   // waits for new connection
        }

        pthread_mutex_lock(&clients_mutex);
        
        int client_index = -1;
        for (int i = 0; i < MAX_CLIENTS; i++) { // finds an empty client slot
            if (!clients[i].active) {
                client_index = i;
                break;
            }
        }

        if (client_index == -1) { // if no empty slot
            pthread_mutex_unlock(&clients_mutex);

            printf("[ERROR] Server is full, rejecting connection from %s\n", inet_ntoa(client_addr.sin_addr));
            send_to_client(client_socket, "SERVER_FULL");
            log_event("LOGIN_ERROR", "Server is full, client connection refused.");
            close(client_socket);

            continue;
        }

        // creates new client
        Client *new_client = &clients[client_index];
        new_client->socket = client_socket;
        new_client->active = 1;
        new_client->address = client_addr;
        new_client->username[0] = '\0'; 
        new_client->current_room[0] = '\0';
        
        client_count++;
        pthread_mutex_unlock(&clients_mutex);

        if (pthread_create(&new_client->thread_id, NULL, client_handler, new_client) != 0) { // creates the client thread
            perror("pthread_create");
            cleanup_client(new_client);
        } else {
            // detach the thread so its resources are automatically released when it terminates
            pthread_detach(new_client->thread_id);
        }
    }
    
    if (!server_running) // if the server not running shutdown the server
        shutdown_server();
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active)
            pthread_cancel(clients[i].thread_id);
    }
    
    log_event("INFO", "Server shutdown.");
    if (log_file) //closes the log file
        fclose(log_file);
    
    // destroys all mutexes
    pthread_mutex_destroy(&clients_mutex);
    pthread_mutex_destroy(&rooms_mutex);
    pthread_mutex_destroy(&log_mutex);
    
    printf("[INFO] Server shutdown complete\n");
    return 0;
}

void *client_handler(void *arg) { // handles client connections
    Client *client = (Client *)arg;
    char buffer[BUFFER_SIZE];
    char log_msg[512];

    int username_accepted = 0;
    
    while (!username_accepted && server_running) { // waits for a valid username
        int bytes_received = recv(client->socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            cleanup_client(client);
            return NULL;
        }
        buffer[bytes_received] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;

        if (!is_valid_username(buffer)) { // checks if the username is valid
            printf("[ERROR] Invalid username format received: '%s'\n", buffer);
            send_to_client(client->socket, "INVALID_USERNAME");
            log_event("LOGIN_ERROR", "Invalid username format received.");

            continue;
        }

        pthread_mutex_lock(&clients_mutex);
        int username_taken = 0;
        int i = 0;
        for (i = 0; i < MAX_CLIENTS; i++) { // checks if the username is already taken
            if (clients[i].active && strcmp(clients[i].username, buffer) == 0) {
                username_taken = 1;
                break;
            }
        }
        
        if (username_taken) { // if username is taken
            pthread_mutex_unlock(&clients_mutex);
            
            printf("[ERROR] Duplicate username attempted: '%s'\n", buffer);
            send_to_client(client->socket, "USERNAME_TAKEN");
            snprintf(log_msg, sizeof(log_msg), "Duplicate username attempted: %.*s", MAX_USERNAME_LEN, buffer);
            log_event("REJECTED", log_msg);
            
            continue;
        }
        
        strcpy(client->username, buffer);
        username_accepted = 1;
        pthread_mutex_unlock(&clients_mutex);
    }
    
    if (!username_accepted) { // if username was not accepted, clean up and exit
        cleanup_client(client);
        return NULL;
    }
    
    printf("[CONNECT] New client connected: '%s' from %s\n", client->username, inet_ntoa(client->address.sin_addr));
    send_to_client(client->socket, "WELCOME");
    snprintf(log_msg, sizeof(log_msg), "user '%s' connected from %s", client->username, inet_ntoa(client->address.sin_addr));
    log_event("LOGIN", log_msg);

    while (server_running && client->active) { // waits for commands from the client
        int bytes_received = recv(client->socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            lost_connection = 1; 
            cleanup_client(client);
            break;
        }
        
        buffer[bytes_received] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;


        if (strncmp(buffer, "/join ", 6) == 0) { // handles the /join command
            char room_name[BUFFER_SIZE];
            strcpy(room_name, buffer + 6);
            
            if (!is_valid_room_name(room_name)) { // checks if the room name is valid
                printf("[ERROR] Invalid room name: '%s'\n", room_name);
                send_to_client(client->socket, "INVALID_ROOM_NAME");
                snprintf(log_msg, sizeof(log_msg), "Invalid room name attempted: %.100s", room_name);
                log_event("REJECTED", log_msg);
                continue;
            }

            if(strcmp(client->current_room, room_name) == 0) { // checks if the client is already in the room
                printf("[ERROR] Client '%s' tried to join the same room '%s'\n", client->username, room_name);
                send_to_client(client->socket, "ALREADY_IN_ROOM");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to join the same room '%.32s'", client->username, room_name);
                log_event("REJECTED", log_msg);
                continue;
            }

            pthread_mutex_lock(&rooms_mutex);
            Room *room = find_room(room_name);
            if (room && room->user_count >= MAX_USERS_PER_ROOM) { // checks if the room is full
                pthread_mutex_unlock(&rooms_mutex);
                printf("[ERROR] Room '%s' is full, client '%s' rejected\n", room_name, client->username);
                send_to_client(client->socket, "ROOM_IS_FULL");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to join full room '%.32s'", client->username, room_name);
                log_event("REJECTED", log_msg);
                continue;
            }
            pthread_mutex_unlock(&rooms_mutex);
            
            if (join_room(client, room_name)) { // joins the room
                char join_msg[BUFFER_SIZE];
                snprintf(join_msg, sizeof(join_msg), "JOIN_ROOM %.32s", room_name);
                send_to_client(client->socket, join_msg);
            } else {
                send_to_client(client->socket, "INVALID_ROOM");
            }
        } else if (strcmp(buffer, "/leave") == 0) { // handles the /leave command
            if (strlen(client->current_room) == 0) { // checks if the client is in a room
                printf("[ERROR] Client '%s' tried to leave a room but is not in one\n", client->username);
                send_to_client(client->socket, "NOT_IN_ROOM");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to leave a room but is not in one", client->username);
                log_event("REJECTED", log_msg);
                continue;
            } 

            char old_room[MAX_ROOM_NAME_LEN + 1];
            strncpy(old_room, client->current_room, MAX_ROOM_NAME_LEN);
            old_room[MAX_ROOM_NAME_LEN] = '\0';
            
            if (leave_room(client))
                send_to_client(client->socket, "LEAVE_ROOM");

        } else if (strncmp(buffer, "/broadcast ", 11) == 0) { // handles the /broadcast command
            if (strlen(client->current_room) == 0) {
                printf("[ERROR] Client '%s' tried to broadcast but is not in a room\n", client->username);
                send_to_client(client->socket, "NOT_IN_ROOM");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to broadcast but is not in a room", client->username);
                log_event("REJECTED", log_msg);
                continue;
            } 
            const char *message_content = buffer + 11;
            char formatted_message[MAX_MESSAGE_LEN + 5];
            snprintf(formatted_message, sizeof(formatted_message), "[%.32s] %.16s: %s", client->current_room, client->username, message_content);

            // broadcasts the message to all users in the room
            broadcast_to_room(client->current_room, formatted_message, client->socket);
            
            printf("[COMMAND] '%s' broadcasted to '%s'\n", client->username, client->current_room);
            send_to_client(client->socket, "MESSAGE_BROADCASTED");
            snprintf(log_msg, sizeof(log_msg), "user '%.32s': %.256s", client->username, message_content);
            log_event("BROADCAST", log_msg);
        }
        else if (strncmp(buffer, "/whisper ", 9) == 0) { // handles the /whisper command
            char *message_start = strchr(buffer + 9, ' ');
            size_t username_len = message_start - (buffer + 9);
            if (username_len > MAX_USERNAME_LEN) username_len = MAX_USERNAME_LEN;
            
            char target_user[MAX_USERNAME_LEN + 1];
            strncpy(target_user, buffer + 9, username_len);
            target_user[username_len] = '\0';
            
            message_start++;

            if (strcmp(target_user, client->username) == 0) { // if the target user is the same as the sender
                printf("[ERROR] Client '%s' tried to whisper to themselves\n", client->username);
                send_to_client(client->socket, "CANNOT_WHISPER_SELF");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to whisper to themselves", client->username);
                log_event("REJECTED", log_msg);
                continue;
            }
            
            Client *target = find_client_by_username(target_user); // finds the client
            if (!target) { // if client does not exist
                printf("[ERROR] Client '%s' tried to whisper to non-existent user '%s'\n", client->username, target_user);
                send_to_client(client->socket, "USER_NOT_FOUND");
                snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to whisper to non-existent user '%.16s'", client->username, target_user);
                log_event("REJECTED", log_msg);
                continue;
            }

            // sends the whisper message to the target client
            char whisper_msg[BUFFER_SIZE];
            snprintf(whisper_msg, sizeof(whisper_msg), "WHISPER_MESSAGE %s %s", client->username, message_start);
            send_to_client(target->socket, whisper_msg);
            
            printf("[COMMAND] %s sent whisper to '%s'\n", client->username, target_user);
            snprintf(whisper_msg, sizeof(whisper_msg), "WHISPER_SENT %s", target_user);
            send_to_client(client->socket, whisper_msg);
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' to '%.16s': %.256s", client->username, target_user, message_start);
            log_event("WHISPER", log_msg);
        
        } else if (strncmp(buffer, "/sendfile ", 10) == 0) {
            char *params = buffer + 10; // Skip "/sendfile "
            char filename[BUFFER_SIZE] = {0};
            char target_username[MAX_USERNAME_LEN + 1] = {0};
            long file_size = 0;
            
            // Parse the command parameters
            sscanf(params, "%s %s %ld", filename, target_username, &file_size);
            
            // Call the function that will handle the file transfer (to be implemented later)
            handle_file_transfer(client, filename, target_username, file_size);
        } else if (strcmp(buffer, "/exit") == 0) { // handles the /exit command
            lost_connection = 0;
            printf("[COMMAND] Client '%s' disconnected\n", client->username);
            send_to_client(client->socket, "LOGOUT");
            snprintf(log_msg, sizeof(log_msg), "user '%s' logged out with '/exit'", client->username);
            log_event("LOGOUT",log_msg);
            close(client->socket);
            break;
        }
    }

    cleanup_client(client);
    return NULL;
}

void log_event(const char *type, const char *message) { // logs events to the log file
    pthread_mutex_lock(&log_mutex);
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(log_file, "%s - [%s] %s\n", timestamp, type, message);
    fflush(log_file);
    
    pthread_mutex_unlock(&log_mutex);
}

void broadcast_to_room(const char *room_name, const char *message, int sender_socket) {  // broadcasts a message to all users in a room
    pthread_mutex_lock(&rooms_mutex);

    Room *room = find_room(room_name);
    if (room) {
        Client *sender = find_client_by_socket(sender_socket);
        char sender_name[MAX_USERNAME_LEN + 1] = {0};
        
        if (sender) {
            strcpy(sender_name, sender->username);
        }
        
        char formatted_message[1050];
        for (int i = 0; i < room->user_count; i++) {
            if (room->users[i]->socket != sender_socket) {
                snprintf(formatted_message, sizeof(formatted_message), "CHAT_MESSAGE %s", message);
                send_to_client(room->users[i]->socket, formatted_message);
            }
        }
    }
    
    pthread_mutex_unlock(&rooms_mutex);
}

Client *find_client_by_username(const char *username) { // finds a client by username
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && strcmp(clients[i].username, username) == 0) {
            pthread_mutex_unlock(&clients_mutex);
            return &clients[i];
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

Client *find_client_by_socket(int socket) { // finds a client by socket
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && clients[i].socket == socket) {
            pthread_mutex_unlock(&clients_mutex);
            return &clients[i];
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

Room *find_room(const char *room_name) { // finds a room by name
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].active && strcmp(rooms[i].name, room_name) == 0) {
            return &rooms[i];
        }
    }
    return NULL;
}

Room *create_room(const char *room_name) { // creates a new room with the given name
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (!rooms[i].active) {
            strcpy(rooms[i].name, room_name);
            rooms[i].user_count = 0;
            rooms[i].active = 1;
            room_count++;
            return &rooms[i];
        }
    }
    return NULL;
}

int join_room(Client *client, const char *room_name) { // joins a client to a room
    char old_room[MAX_ROOM_NAME_LEN + 1] = "";
    char log_msg[512];
    int was_in_room = 0;
    int rejoining = 0;
    
    // Check if client previously joined this room
    for (int i = 0; i < client->num_previously_joined_rooms; i++) {
        if (strcmp(client->previously_joined_rooms[i], room_name) == 0) {
            rejoining = 1;
            break;
        }
    }
    
    pthread_mutex_lock(&clients_mutex);
    if (strlen(client->current_room) > 0) { // if the client is already in a room
        strncpy(old_room, client->current_room, MAX_ROOM_NAME_LEN);
        old_room[MAX_ROOM_NAME_LEN] = '\0';
        was_in_room = 1;
    }
    pthread_mutex_unlock(&clients_mutex);
    
    if (was_in_room) { // if the client was in a room, leave that room first
        leave_room(client);
    }
    pthread_mutex_lock(&rooms_mutex);
    
    Room *room = find_room(room_name); // finds the room by name
    if (!room) { // if the room does not exist, create it
        room = create_room(room_name);
        if (!room) {
            pthread_mutex_unlock(&rooms_mutex);
            return 0;
        }
    }
    
    // adds the client to the room
    room->users[room->user_count++] = client;
    strncpy(client->current_room, room_name, MAX_ROOM_NAME_LEN);
    client->current_room[MAX_ROOM_NAME_LEN] = '\0';
    
    // Add room to previously joined rooms if it's not already there
    if (!rejoining) {
        if (client->num_previously_joined_rooms < MAX_ROOMS) {
            strncpy(client->previously_joined_rooms[client->num_previously_joined_rooms], room_name, MAX_ROOM_NAME_LEN);
            client->previously_joined_rooms[client->num_previously_joined_rooms][MAX_ROOM_NAME_LEN] = '\0';
            client->num_previously_joined_rooms++;
        }
    }
    
    char join_notification[BUFFER_SIZE];
    if (rejoining) {
        snprintf(join_notification, sizeof(join_notification), "USER_REJOINED %s %s", client->username, room_name);
    } else {
        snprintf(join_notification, sizeof(join_notification), "USER_JOINED %s %s", client->username, room_name);
    }

    int clients_to_notify = room->user_count - 1;
    int client_sockets[MAX_USERS_PER_ROOM];
    
    // Collect the sockets of all other users in the room
    for (int i = 0; i < room->user_count; i++) {
        if (room->users[i]->socket != client->socket) {
            client_sockets[i] = room->users[i]->socket;
        }
    }
    
    pthread_mutex_unlock(&rooms_mutex);

    if (!was_in_room) { // if the client was not in a room before
        if (rejoining) {
            printf("[ROOM] User '%s' rejoined room '%s'\n", client->username, room_name);
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' rejoined room '%.32s'", client->username, room_name);
            log_event("ROOM", log_msg);
        } else {
            printf("[COMMAND] %s joined room '%s'\n", client->username, room_name);
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' joined room '%.32s'", client->username, room_name);
            log_event("JOIN", log_msg);
        }
    } else { // if the client was in a room before
        if (rejoining) {
            printf("[ROOM] User '%s' moved from room '%s' and rejoined '%s'\n", client->username, old_room, room_name);
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' left room '%.32s', rejoined '%.32s'", client->username, old_room, room_name);
        } else {
            printf("[ROOM] User '%s' moved from room '%s' to '%s'\n", client->username, old_room, room_name);
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' left room '%.32s', joined '%.32s'", client->username, old_room, room_name);
        }
        log_event("ROOM", log_msg);
    }

    // sends join notification to all users in the room except the one joining
    int notified_users = 0;
    for (int i = 0; i < clients_to_notify; i++) {
        send_to_client(client_sockets[i], join_notification);
        notified_users++;
    }

    if (notified_users > 0) { // if there are other users in the room
        if (rejoining) {
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' rejoined room '%.32s'", client->username, room_name);
        } else {
            snprintf(log_msg, sizeof(log_msg), "user '%.16s' joined room '%.32s'", client->username, room_name);
        }
        log_event("NOTIFICATION", log_msg);
    }
    
    return 1;
}

int leave_room(Client *client) { // removes a client from the current room
    char log_msg[512];
    char room_name[MAX_ROOM_NAME_LEN];
    strncpy(room_name, client->current_room, MAX_ROOM_NAME_LEN);
    room_name[MAX_ROOM_NAME_LEN] = '\0';
    
    char leave_notification[BUFFER_SIZE];
    snprintf(leave_notification, sizeof(leave_notification), "USER_LEFT %.16s %.32s", client->username, room_name);
    
    int sockets_to_notify[MAX_USERS_PER_ROOM];
    int notify_count = 0;

    pthread_mutex_lock(&rooms_mutex);

    Room *room = find_room(room_name);
    if (room) { // if the room exists
        for (int i = 0; i < room->user_count; i++) {
            if (room->users[i] == client) { // if the current user is the one leaving
                for (int j = i; j < room->user_count - 1; j++) { // shifts users left
                    room->users[j] = room->users[j + 1];
                }
                room->user_count--;
                
                i--;
            } else { // if the user is not leaving, add their socket to the notification list
                sockets_to_notify[notify_count++] = room->users[i]->socket;
            }
        }
        
        if (room->user_count == 0) { // if the room will be empty after this user leaves
            room->active = 0;
            room_count--;
        }
    }
    strcpy(client->current_room, "");
    
    pthread_mutex_unlock(&rooms_mutex);

    printf("[COMMAND] %s left room '%s'\n", client->username, room_name);
    snprintf(log_msg, sizeof(log_msg), "user '%.16s' left room '%.32s'", client->username, room_name);
    log_event("LEAVE", log_msg);
    
    for (int i = 0; i < notify_count; i++) { // sends leave notification to all remaining users in the room
        send_to_client(sockets_to_notify[i], leave_notification);
    }
    
    if (notify_count > 0) { // if there are users left in the room
        char log_msg[512];
        snprintf(log_msg, sizeof(log_msg), "user '%.32s' left room '%.32s'", client->username, room_name);
        log_event("NOTIFICATION", log_msg);
    }
    
    return 1;
}

int is_valid_username(const char *username) { // checks if the username is valid
    if (!username || strlen(username) > MAX_USERNAME_LEN || strlen(username) == 0) // checks if the username is empty or too long
        return 0;

    for (int i = 0; username[i]; i++) { // checks if the username contains only alphanumeric characters
        if (!isalnum((unsigned char)username[i])) {
            return 0;
        }
    }
    return 1;
}

int is_valid_room_name(const char *room_name) { // checks if the room name is valid
    if (strlen(room_name) == 0 || strlen(room_name) > MAX_ROOM_NAME_LEN) // checks if the room name is empty or too long
        return 0;
    
    for (int i = 0; room_name[i]; i++) { // checks if the room name contains only alphanumeric characters
        if (!isalnum((unsigned char)room_name[i])) {
            return 0;
        }
    }
    return 1;
}

void handle_sigint(int sig) { // handles the SIGINT signal (Ctrl+C)
    (void)sig;
    printf("\n[INFO] Shutdown signal received.\n");
    server_running = 0;
    
    if (server_socket != -1) // closes the socket
        close(server_socket);

}

void shutdown_server() { // handles the shutdown process
    log_event("SHUTDOWN", "SIGINT received. Disconnecting clients, saving logs.");
    
    // Stop the file processor thread
    server_running = 0;
    sem_post(&filled_slots); // Wake up the processor thread if it's waiting
    
    if (file_processor_thread) {
    pthread_join(file_processor_thread, NULL);
}
    
    // Destroy semaphores
    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            send_to_client(clients[i].socket, "SERVER_SHUTDOWN");
            close(clients[i].socket);
            clients[i].active = 0;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    printf("[INFO] All clients notified.\n");
}

void cleanup_client(Client *client) { // cleans up a client when they disconnect
    if (!client->active) return;
    
    pthread_mutex_lock(&clients_mutex);
    
    if (strlen(client->current_room) > 0) // if the client is in a room, leave that room
        leave_room(client);
    
    if (client->socket != -1) // closes the client's socket
        close(client->socket);
    
    if (lost_connection) { // if the client lost connection
        printf("[DISCONNECT] Client '%s' lost connection\n", client->username);
        char log_msg[512];
        snprintf(log_msg, sizeof(log_msg), "user '%s' lost connection. Cleaned up resources.", client->username);
        log_event("DISCONNECT", log_msg);
    }

    client->active = 0;
    strcpy(client->username, "");
    strcpy(client->current_room, "");
    client_count--;
    
    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(int socket, const char *message) { // sends a message to a client
    if (send(socket, message, strlen(message), MSG_NOSIGNAL) == -1) {
        // handle broken pipe silently
    }
}

void handle_file_transfer(Client *client, const char *filename, const char *target_username, long file_size) {
    char log_msg[512];
    
    if (strcmp(client->username, target_username) == 0) { // if the target username is the same as the client's username
        printf("[ERROR] Client '%s' tried to send a file to themselves\n", client->username);
        send_to_client(client->socket, "SEND_TO_SELF");
        snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to send a file to themselves", client->username);
        log_event("REJECTED", log_msg);
        return;
    }
    
    if (file_size > (3 * 1024 * 1024)) { // checks if the file size exceeds 3MB
        printf("[ERROR] File '%s' from user '%s' exceeds size limit.\n", filename, client->username);
        
        char response[BUFFER_SIZE];
        snprintf(response, sizeof(response), "FILE_IS_LARGE %ld", file_size);
        send_to_client(client->socket, response);
        
        snprintf(log_msg, sizeof(log_msg), "File '%s' from user '%s' exceeds size limit.", filename, client->username);
        log_event("ERROR", log_msg);
        return;
    }
    
    int has_valid_extension = 0;
    const char *extension = strrchr(filename, '.');
    
    if (extension) { // if the filename has an extension
        extension++;
        if (strcasecmp(extension, "txt") == 0 || 
            strcasecmp(extension, "pdf") == 0 || 
            strcasecmp(extension, "jpg") == 0 || 
            strcasecmp(extension, "png") == 0) {
            has_valid_extension = 1;
        }
    }
    
    if (!has_valid_extension) { // if the file has an invalid extension
        printf("[ERROR] Client '%s' tried to send a file with invalid extension: %s\n", client->username, filename);
        send_to_client(client->socket, "WRONG_EXTENSION");
        snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to send a file with invalid extension: %.64s", client->username, filename);
        log_event("REJECTED", log_msg);
        return;
    }
    
    Client *target = find_client_by_username(target_username);
    if (!target) { // if the target user does not exist
        printf("[ERROR] Client '%s' tried to send file to non-existent user '%s'\n", client->username, target_username);
        send_to_client(client->socket, "USER_NOT_FOUND");
        snprintf(log_msg, sizeof(log_msg), "user '%.16s' tried to send file to non-existent user '%.16s'", client->username, target_username);
        log_event("REJECTED", log_msg);
        return;
    }
    
    snprintf(log_msg, sizeof(log_msg), "user '%.16s' requested to send file '%.64s' (%ld bytes) to user '%.16s'", client->username, filename, file_size, target_username);
    log_event("FILE_TRANSFER_REQUEST", log_msg);
    
    add_to_file_queue(client, target, filename, file_size);
}

void* file_queue_processor(void* arg) { // processes the file transfer queue
    (void)arg;
    
    while(server_running) {
        // waits until there's at least one item in the queue
        sem_wait(&filled_slots);
        
        if(!server_running) {
            break;
        }
        
        pthread_mutex_lock(&queue_mutex);
        
        // gets the file from the queue
        FileTransferRequest request = file_queue[queue_head];
        file_queue[queue_head].active = 0;
        
        // moves pointer
        queue_head = (queue_head + 1) % MAX_FILE_QUEUE;
        queue_size--;
        
        pthread_mutex_unlock(&queue_mutex);
        
        // signal that there's an empty slot in the queue
        sem_post(&empty_slots);
        
        // process the file transfer
        process_file_transfer(&request);
    }
    
    return NULL;
}

int add_to_file_queue(Client *sender, Client *receiver, const char *filename, long file_size) { // adds a file transfer request to the queue
    char log_msg[512];
    
    if (sem_trywait(&empty_slots) == -1) {
        send_to_client(sender->socket, "FILE_QUEUE_FULL");
        printf("[FILE QUEUE] Queue is full. User '%s' waiting to send '%s' to '%s'\n", sender->username, filename, receiver->username);
        log_event("INFO", "File queue is full, user waiting to send file.");
        
        // if queue is full, we need to wait for an empty slot with a timeout
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += MAX_FILE_WAIT_TIME;
        
        if (sem_timedwait(&empty_slots, &ts) == -1) { // timed out and queue is still full
            send_to_client(sender->socket, "FILE_QUEUE_STILL_FULL");
            printf("[FILE QUEUE] User '%.16s' timed out waiting to send '%s' to '%.16s'\n", sender->username, filename, receiver->username);
            snprintf(log_msg, sizeof(log_msg), "File transfer from '%.16s' to '%.16s' cannot be performed, queue is full", sender->username, receiver->username);
            log_event("ERROR", log_msg);
            
            return 0;
        }
    }
    
    pthread_mutex_lock(&queue_mutex);
    
    // adds the file transfer request to the queue
    FileTransferRequest *request = &file_queue[queue_tail];
    request->sender = sender;
    request->receiver = receiver;
    strncpy(request->filename, filename, BUFFER_SIZE - 1);
    request->filename[BUFFER_SIZE - 1] = '\0';
    request->file_size = file_size;
    request->queue_time = time(NULL);
    request->active = 1;
    
    // updates the queue state
    queue_tail = (queue_tail + 1) % MAX_FILE_QUEUE;
    queue_size++;
    
    // notifies sender that file was added to queue
    char queue_msg[BUFFER_SIZE];
    snprintf(queue_msg, sizeof(queue_msg), "FILE_QUEUED %d", queue_size);
    send_to_client(sender->socket, queue_msg);
    printf("[FILE QUEUE] Upload '%s' from user '%s' added to queue.\n", filename, sender->username);
    snprintf(log_msg, sizeof(log_msg), "Upload '%s' from %.16s added to queue.", filename, sender->username);
    log_event("FILE-QUEUE", log_msg);
    
    pthread_mutex_unlock(&queue_mutex);
    
    // signal that there's a new item in the queue
    sem_post(&filled_slots);
    
    return 1;
}

void process_file_transfer(FileTransferRequest *request) { // processes a file transfer request from the queue
    char log_msg[512];
    
    // calculates time in queue
    time_t now = time(NULL);
    int seconds_in_queue = (int)difftime(now, request->queue_time);
    
    char start_msg[BUFFER_SIZE];
    snprintf(start_msg, sizeof(start_msg), "FILE_TRANSFER_STARTED %.256s %d", request->filename, seconds_in_queue);
    send_to_client(request->sender->socket, start_msg);
    printf("[FILE] '%s' from user '%s' started upload after %d seconds in queue.\n", request->filename, request->sender->username, seconds_in_queue);
    snprintf(log_msg, sizeof(log_msg), "'%.256s' from user '%.16s' started upload after %d seconds in queue.", request->filename, request->sender->username, seconds_in_queue);
    log_event("FILE", log_msg);
    
    sleep(3);
    
    // cchecks if the receiver already has a file with this name
    if (file_exists_for_receiver(request->receiver, request->filename)) {
        printf("[FILE] %s' already exists for user '%s', but still allowing transfer\n", request->filename, request->receiver->username);
        
        // Notify receiver about the file conflict
        char conflict_msg[BUFFER_SIZE];
        snprintf(conflict_msg, sizeof(conflict_msg), "FILE_RECEIVED %.16s %.256s", request->sender->username, request->filename);
        send_to_client(request->receiver->socket, conflict_msg);
        
        // Still notify sender of successful transfer
        char complete_msg[BUFFER_SIZE];
        snprintf(complete_msg, sizeof(complete_msg), "FILE_TRANSFER_COMPLETE %.256s %.16s", request->filename, request->receiver->username);
        send_to_client(request->sender->socket, complete_msg);
        
        // Log the conflict but proceed with transfer
        snprintf(log_msg, sizeof(log_msg), "Conflict: '%.256s' received twice", request->filename);
        log_event("FILE", log_msg);
        
        printf("[FILE] '%s' sent from '%s' to '%s' (despite name conflict).\n", request->filename, request->sender->username, request->receiver->username);
        snprintf(log_msg, sizeof(log_msg), "'%.256s' sent from '%s' to '%s'.", request->filename, request->sender->username, request->receiver->username);
        log_event("SEND_FILE", log_msg);
        
        return;
    }
    
    // no conflict, proceed with the file transfer
    add_file_to_receiver(request->receiver, request->filename);
    
    // notify receiver of incoming file
    char receive_msg[BUFFER_SIZE];
    snprintf(receive_msg, sizeof(receive_msg), "FILE_RECEIVED %.16s %.256s", request->sender->username, request->filename);
    send_to_client(request->receiver->socket, receive_msg);
    
    // notify sender of successful transfer
    char complete_msg[BUFFER_SIZE];
    snprintf(complete_msg, sizeof(complete_msg), "FILE_TRANSFER_COMPLETE %.256s %.16s", request->filename, request->receiver->username);
    send_to_client(request->sender->socket, complete_msg);
    
    printf("[FILE] '%s' sent from '%s' to '%s'.\n", request->filename, request->sender->username, request->receiver->username);
    snprintf(log_msg, sizeof(log_msg), "'test_files/%.256s' sent from '%s' to '%s'.", request->filename, request->sender->username, request->receiver->username);
    log_event("SEND_FILE", log_msg);
}

// checks if a file with the given name already exists in the receiver's received files
int file_exists_for_receiver(Client *receiver, const char *filename) {
    for (int i = 0; i < receiver->received_files_count; i++) { 
        if (strcmp(receiver->received_files[i], filename) == 0) {
            return 1;
        }
    }
    return 0; 
}

// adds a filename to the receiver's received files list
void add_file_to_receiver(Client *receiver, const char *filename) {
    if (receiver->received_files_count < MAX_RECEIVED_FILES) {
        strncpy(receiver->received_files[receiver->received_files_count], filename, BUFFER_SIZE - 1);
        receiver->received_files[receiver->received_files_count][BUFFER_SIZE - 1] = '\0';
        receiver->received_files_count++;
    }
}