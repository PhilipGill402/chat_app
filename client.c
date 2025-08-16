#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_MESSAGE_LENGTH 1024
#define MAX_USERNAME_LENGTH 128
#define MAX_MESSAGE_BACKLOG 1024

int running = 1;
pthread_mutex_t tty_mu = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int sig){
    running = 0;
}

void* receive_thread(void* arg){
    int client_fd = *(int*)arg;

    while(running){
        char buffer[MAX_MESSAGE_LENGTH]; 
        
        int bytes_read = recv(client_fd, buffer, MAX_MESSAGE_LENGTH - 1, 0);
        if (bytes_read < 0){
            perror("recv");
            continue;
        }
            
        buffer[bytes_read] = '\0';

        pthread_mutex_lock(&tty_mu);
        fprintf(stdout, "\r\x1b[K");
        fputs(buffer, stdout);
        fputc('\n', stdout);
        fputs(">> ", stdout);
        fflush(stdout);
        pthread_mutex_unlock(&tty_mu);
    }
    running = 0;
    return NULL;
}

int main(){
    struct sockaddr_in address;
    socklen_t address_length = sizeof(address);
    int ret;
    int bytes_sent;
    pthread_t thread; 

    signal(SIGINT, signal_handler); 

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
    if (client_fd < 0){
        perror("connect");
        close(client_fd);
        return -1;
    }    
    
    printf("Connected to %s:%d\n", SERVER_IP, SERVER_PORT);

    char username[MAX_USERNAME_LENGTH];
    printf("Enter your username: ");
    fgets(username, MAX_USERNAME_LENGTH - 1, stdin);
    username[strlen(username) - 1] = '\0'; 
    bytes_sent = send(client_fd, username, strlen(username), 0);

    if (bytes_sent < 0){
        perror("send");
        close(client_fd);
        return -1;
    }
    
    pthread_create(&thread, NULL, receive_thread, &client_fd);
    pthread_detach(thread);

    while (running){
        char buffer[MAX_MESSAGE_LENGTH];
        pthread_mutex_lock(&tty_mu);
        printf(">> ");
        fflush(stdout);
        pthread_mutex_unlock(&tty_mu);
        fgets(buffer, MAX_MESSAGE_LENGTH - 2, stdin);
        
        buffer[strlen(buffer) - 1] = '\0';
        
        bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
        if (bytes_sent < 0){
            perror("send");
            break;
        }

    } 
    
    running = 0;
    close(client_fd);
    printf("Disconnected\n");
    return 0;
}
