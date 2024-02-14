#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8000
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    int server_sockfd, client_sockfd;
    // create server sokcet
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket!\n");
        return 1;
    }

    // setup server address
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind the socket to the server address
    if (bind(server_sockfd, (struct sockaddr*)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("Error binding socket!\n");
        return 1;
    }

    return 0;
}
