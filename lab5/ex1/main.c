#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void handle_client(int client_socket) {
    char buffer[1024];
    int bytes_received;


    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received < 0) {
            error("Error reading from client");
        }

        if (bytes_received == 0) {
            break;
        }

        printf("Client: %s\n", buffer);

    
    
        if (send(client_socket, buffer, bytes_received, 0) < 0) {
            error("Error sending data to client");
        }
    }

    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [Port_Number]\n", argv[0]);
        exit(1);
    }

    int port_number = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("Error creating socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error binding");
    }

    listen(server_socket, 5);
    printf("Server is listening on port %d\n", port_number);

    while (1) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (client_socket < 0) {
            error("Error accepting connection");
        }

        pid_t pid = fork();
        if (pid == 0) {
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else if (pid < 0) {
            error("Error forking process");
        }

        close(client_socket);
    }

    close(server_socket);

    return 0;
}
