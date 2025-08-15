#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(){
    struct sockaddr_in address;
    socklen_t address_length = sizeof(address);
    int ret;

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0){
        perror("socket"); 
        close(client_fd);
        return -1;
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(SERVER_PORT);

    ret = connect(client_fd, (struct sockaddr*)&address, address_length);

    printf("connected\n");

    close(client_fd);    
    return 0;
}
