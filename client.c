#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <getopt.h>

#define IPADDR "127.0.0.1" // Default
#define PORT 8080 // Default
// 2b Req: Periodically every second
#define INTERVAL 1

void send_messages(int client_socket) {
    char message[1024];
    int message_count = 0;

    while (1) {
        // 2b Req: Send a message (count) to the server
        sprintf(message, "Message is count %d\n", ++message_count);
        send(client_socket, message, strlen(message), 0);
        printf("Sent: %s", message);

        // 2b Req: Print the server's response
        char buffer[1024];
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Received: %s", buffer);

        //Send/print periodically
        sleep(INTERVAL);
    }
}

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;

    char *server_ip = IPADDR;
    int server_port = PORT;

    int opt;
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch (opt) {
            case 'i':
                server_ip = optarg;
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -i <server_ip> -p <server_port>\n", argv[0]);
                exit(1);
        }
    }

    //Try to create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("ERROR: Socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("ERROR: Invalid server IP address");
        exit(1);
    }

    // Try to connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("ERROR: Connection failed");
        exit(1);
    }

    printf("Connected to server %s:%d\n", server_ip, server_port);

    // Sending messages
    send_messages(client_socket);

    // Shouldn't reach this due to infinite loop, but good practice
    close(client_socket);

    return 0;
}