#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
//1 Req: Multiple Clients
#define CLIENTS 5

void *handle_client(void *client_socket_ptr) {
    int client_socket = *((int *)client_socket_ptr);
    char buffer[1024];
    int received_bytes;

    while (1) {
        //1 Req: Listen to clients
        received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received_bytes <= 0) {
            close(client_socket);
            pthread_exit(NULL);
        }

        // 1 Req: Echo back clients' messages
        send(client_socket, buffer, received_bytes, 0);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Try to create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("ERROR: Socket creation");
        exit(1);
    }

    // 1 Req: Bind socket to port 8080
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("ERROR: Binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, CLIENTS) == -1) {
        perror("ERROR: Listening");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept incoming connections
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("ERROR: Accepting connection");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client as necessary
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &client_socket) != 0) {
            perror("ERROR: Thread creation");
            close(client_socket);
        }
    }

    // Shouldn't reach this due to infinite loop, but good practice
    close(server_socket);

    return 0;
}