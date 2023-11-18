#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "global.h"
#include "./feature/Receiver/receiver.h"
#include "./config/tcp.h"

int main(int argc, char *argv[]) {
    // Handle argv
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [Port_Number]\n", argv[0]);
        exit(1);
    }

    int port_number = atoi(argv[1]);

    // Init server
    int server_socket = init_server(port_number);

    // Communicate with clients
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    client_len = sizeof(client_addr);
    while(1) {
        char buffer[STRING_LENGTH];

        // Accept connect with client
        printf("\nWaiting client connect ....\n");
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Get client information
        char client_ip[STRING_LENGTH];
        int client_port;
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, STRING_LENGTH);
        client_port = ntohs(client_addr.sin_port);

        // Send welcome message
        send_with_error_handling(
            client_socket,
            buffer, 
            "+OK Welcome to file server",
            "Send welcome message failed"
        );
        logger(client_ip, client_port, " ", "+OK Welcome to file server");

        // Start receive file
        file_receiving_protocol(client_socket, client_ip, client_port);
    }

    // Close server
    close(server_socket);
    return 0;
}
