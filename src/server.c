#include "server.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_CLIENTS 64
#define MAX_MESSAGE_LENGTH 1024
#define MAX_USERNAME_LENGTH 128

int server_fd;
int running = 1;
int active_users[MAX_CLIENTS];
int num_clients = 0;

void* handle_client(void* arg){
    int* client_fd_ptr = (int*)arg;
    int client_fd = *client_fd_ptr;
    free(arg);
    int bytes_read; 
    int bytes_sent;

    //adds client to active_users array
    active_users[num_clients] = client_fd;
    num_clients++;

    char* username = malloc(sizeof(char) * MAX_USERNAME_LENGTH);
    bytes_read = recv(client_fd, username, MAX_USERNAME_LENGTH - 1, 0);
    

    if (bytes_read < 0){
        perror("recv");
        close(client_fd);
        return NULL;
    }
    
    username[bytes_read] = '\0';
    printf("%s connected\n", username);
    
    while (1){
        //receives message 
        char* buffer = malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
        bytes_read = recv(client_fd, buffer, MAX_MESSAGE_LENGTH - 1, 0);
        if (bytes_read < 0){
            perror("recv");
            continue;
        } else if (bytes_read == 0){
            printf("%s disconnected\n", username);
            
            for (int i = 0; i < num_clients; i++){
                if (active_users[i] == client_fd){
                    active_users[i] = -1;
                } 
            }

            close(client_fd);
            return NULL; 
        }
        
        buffer[bytes_read] = '\0';
        char* msg = malloc(sizeof(char) * (MAX_USERNAME_LENGTH + 2 + MAX_MESSAGE_LENGTH));
        strcpy(msg, username);
        strcat(msg, ": ");
        strcat(msg, buffer);
        msg[strlen(msg)] = '\0'; 
        printf("%s", msg);

        //sends to everyone
        for (int i = 0; i < num_clients; i++){
            if (active_users[i] == -1){
                continue;
            }
            bytes_sent = send(active_users[i], msg, strlen(msg), 0);
            if (bytes_sent < 0){
                perror("send");
                continue;
            }
        } 
        
        free(buffer);
        free(msg);
        buffer = NULL;
        msg = NULL;

    }
    
    free(username);
    username = NULL;
    close(client_fd);
    return NULL;
}

int main(){
    struct sockaddr_in address;
    socklen_t address_len = sizeof(address);
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int ret;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        perror("socket");
        close(server_fd);
        return -1;
    } 
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);
   
    ret = bind(server_fd, (struct sockaddr*)&address, address_len);
    if (ret < 0){
        perror("bind");
        close(server_fd);
        return -1;
    }
    

    ret = listen(server_fd, MAX_CLIENTS);
    if (ret < 0){
        perror("listen");
        close(server_fd);
        return -1;
    }
    
    while (running){
        pthread_t thread; 
        int* client_fd = malloc(sizeof(int)); 
        *client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (*client_fd < 0){
            perror("accept");
            close(server_fd);
            close(*client_fd);
            return -1;
        }
        
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}
