#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8000
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address,
                sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while (1) {
        printf("Enter text to send (or type 'exit' to quit): ");
        fgets(input, BUFFER_SIZE, stdin);

        // Remove newline character at the end of input
        input[strcspn(input, "\n")] = 0;

        // Check if user wants to exit
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Send user input to server
        send(client_socket, input, strlen(input), 0);

        // Receive response from server
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Error receiving data from server");
            break;
        }

        // Null-terminate the received data
        buffer[bytes_received] = '\0';

        printf("Server response: %s\n", buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
