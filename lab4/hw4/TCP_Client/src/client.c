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
        fprintf(stderr, "Usage: %s [IP_Address] [Port_Number]\n", argv[0]);
        exit(1);
    }

    char *ip_address = argv[1];
    int port_number = atoi(argv[2];

    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        error("Error creating socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port_number);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error connecting to the server");
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
        error("Error receiving welcome message");
    }
    printf("Server: %s\n", buffer);

    while (1) {
        char file_path[256];
        printf("Enter file path (empty to exit): ");
        fgets(file_path, sizeof(file_path), stdin);
        file_path[strcspn(file_path, "\n")] = 0; // Remove newline character

        if (strlen(file_path) == 0) {
            break; // Exit the loop if the user enters an empty path
        }

        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", file_path);
            continue;
        }

        fseek(file, 0, SEEK_END);
        int file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "UPLD %s %d", file_path, file_size);

        if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
            error("Error sending file request message");
        }

        if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
            error("Error receiving file request message from the server");
        }
        if (strcmp(buffer, "+OK Please send file") != 0) {
            fprintf(stderr, "Error: Server did not accept the file request\n");
            fclose(file);
            continue;
        }

        while (!feof(file)) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = fread(buffer, 1, sizeof(buffer), file);
            if (send(client_socket, buffer, bytes_read, 0) < 0) {
                error("Error sending the file to the server");
            }
        }

        if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
            error("Error receiving the result from the server");
        }
        printf("Server: %s\n", buffer);

        fclose(file);
    }

    close(client_socket);

    return 0;
}
