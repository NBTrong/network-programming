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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [Port_Number] [Directory_name]\n", argv[0]);
        exit(1);
    }

    int port_number = atoi(argv[1]);
    char *directory_name = argv[2];

    // Create the storage directory if it doesn't exist
    if (mkdir(directory_name, 0777) == -1) {
        error("Error creating storage directory");
    }

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

    client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

    if (client_socket < 0) {
        error("Error accepting connection");
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "+OK Welcome to file server\n");

    if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
        error("Error sending welcome message");
    }

    memset(buffer, 0, sizeof(buffer));

    // Receive the filename and size from the client
    if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
        error("Error receiving filename and size");
    }

    char file_name[256];
    int file_size;
    sscanf(buffer, "%s %d", file_name, &file_size);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "+OK Please send file\n");

    if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
        error("Error sending file request message");
    }

    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        error("Error opening file for writing");
    }

    int received = 0;
    while (received < file_size) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            error("Error receiving data from the client");
        }
        fwrite(buffer, 1, bytes_received, file);
        received += bytes_received;
    }

    fclose(file);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "+OK Successful upload\n");

    if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
        error("Error sending success message");
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
