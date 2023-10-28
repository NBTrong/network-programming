#include "receiver.h"

int file_receiving_protocol(int sender_socket, const char* client_ip, const int client_port) {
    while(1) {
        char request[1024];
        char response[1024];

        // Receive the filename and size from the client
        if (recv_with_error_handling(sender_socket, request, sizeof(request),
            "Error Receive filename and size failed") <= 0)
            return 0;

        // Get file name, file size
        char file_name[1024];
        int file_size;
        sscanf(request, "UPLD %s %d", file_name, &file_size);
        printf("File name: %s, file size: %d\n", file_name, file_size);
        if (file_size > FILE_SIZE_LIMIT) {
            char message[1024] = "";
            sprintf(message, "-ERR The file is too large, please make sure the file is smaller than %d byte", FILE_SIZE_LIMIT);
            send_with_error_handling(
                sender_socket,
                response,
                message,
                "Error sending file request message"
            );
            continue;
        }

        // Ask client to send files
        send_with_error_handling(
            sender_socket,
            response,
            "+OK Please send file",
            "Error sending file request message"
        );

        // Receive file
        char file_path[1024*2];
        sprintf(file_path, "%s/%s", full_directory, file_name);
        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            perror("Error opening file for writing");
            logger(client_ip, client_port, request, "-ERR Error opening file for writing");
            return 0;
        }

        int received = 0;
        printf("Receiving ...\n");
        char buffer[1024];
        while (received < file_size) {
            int bytes_received = recv_with_error_handling(
                sender_socket, 
                buffer, 
                sizeof(buffer), 
                "Error receiving data from the client"
            );
            if(bytes_received <= 0) {
                logger(client_ip, client_port, request, "-ERR Error receiving data from the client");
                return 0;
            }
            fwrite(buffer, 1, bytes_received, file);
            received += bytes_received;
        }

        fclose(file);
        send_with_error_handling(
            sender_socket,
            response,
            "+OK Successful upload",
            "Error sending success message"
        );
        logger(client_ip, client_port, request, response);
    }
}
