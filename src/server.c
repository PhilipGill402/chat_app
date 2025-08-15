#include "server.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_CLIENTS 64
int running = 1;

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
        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (client_fd < 0){
            perror("accept");
            close(server_fd);
            close(client_fd);
            return -1;
        }
        
        printf("Connected!\n");
    }

    close(server_fd);
    return 0;
}
