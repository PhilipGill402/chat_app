#include "server.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_CLIENTS 64
#define MAX_MESSAGE_LENGTH 1024

int running = 1;

void* handle_client(void* arg){
    int* client_fd_ptr = (int*)arg;
    int client_fd = *client_fd_ptr;
    free(arg);

    while (1){
        char* buffer = malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
        int bytes_read = recv(client_fd, buffer, MAX_MESSAGE_LENGTH - 1, 0);
        if (bytes_read < 0){
            perror("recv");
            return NULL; 
        } else if (bytes_read == 0){
            printf("Client %d disconnected\n", client_fd);
            close(client_fd);
            return NULL; 
        }
        
        buffer[bytes_read - 1] = '\0';
        printf("Client %d: %s\n", client_fd, buffer);
        free(buffer);
        buffer = NULL;

    }

    close(client_fd);
    return NULL;
}

int main(){
    struct sockaddr_in address;
    socklen_t address_len = sizeof(address);
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int ret;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0){
        perror("socket");
        close(server_fd);
        return -1;
    } 

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
        
        printf("Client %d connected\n", *client_fd);
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}
