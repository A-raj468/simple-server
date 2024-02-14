#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8000
#define BUFFER_SIZE 1024
#define MAX_CLIENT 2

int main(int argc, char* argv[]) {
    int server_sockfd, client_sockfd;
    // create server sokcet
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket!\n");
        return 1;
    }

    // setup server address
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind the socket to the server address
    if (bind(server_sockfd, (struct sockaddr*)&server_addr,
             sizeof(server_addr)) == -1) {
        perror("Error binding socket!\n");
        return 1;
    }

    // listen for incoming connections
    if (listen(server_sockfd, MAX_CLIENT) == -1) {
        perror("Error listening!\n");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (true) {
        // Accept connections
        if ((client_sockfd = accept(
                 server_sockfd, (struct sockaddr*)&client_addr, &client_len)) ==
            -1) {
            perror("Error accepting connection");
            return 1;
        }
        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        char buffer[BUFFER_SIZE];

        // Echo back the data
        while (1) {
            int bytes_recieved = recv(client_sockfd, buffer, sizeof(buffer), 0);
            if (bytes_recieved <= 0) {
                // Connection closed or error
                // close(client_sockfd);
                printf("Client disconnected: %s:%d\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port));
                break;
            }

            // Echo back the recieved data
            send(client_sockfd, buffer, bytes_recieved, 0);
        }
    }

    // Close the server socket
    close(server_sockfd);

    return 0;
}
