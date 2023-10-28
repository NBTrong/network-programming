#include "./sender.h"

void file_sending_protocol(int client_socket) {
    while (1) {
        // Input file
        char file_path[256];
        printf("Enter file path (empty to exit): ");
        fgets(file_path, sizeof(file_path), stdin);
        file_path[strlen(file_path) - 1] = '\0'; // Remove newline character
        if (strlen(file_path) == 0) {
            break;
        }

        // Open file
        FILE *file = fopen(file_path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", file_path);
            continue;
        }

        // Get file size
        fseek(file, 0, SEEK_END);
        int file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Send file information
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        char *file_name = basename(file_path);
        sprintf(buffer, "UPLD %s %d", file_name, file_size);
        if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
            perror("Error sending file request message");
        }

        // Receive request send file from server
        memset(buffer, 0, strlen(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
            perror("Error receiving file request message from the server");
        }
        handle_message(buffer);

        // Send file to server
        printf("Sending ...\n");
        while (!feof(file)) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = fread(buffer, 1, sizeof(buffer), file);
            if (send(client_socket, buffer, bytes_read, 0) < 0) {
                perror("Error sending the file to the server");
            }
        }

        // Receive result
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
            perror("Error receiving the result from the server");
        }
        handle_message(buffer);
        fclose(file);
    }
}