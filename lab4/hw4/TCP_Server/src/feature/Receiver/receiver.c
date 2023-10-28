#include "receiver.h"

int file_receiving_protocol(int sender_socket) {
    while(1) {
        char buffer[1024];
        // Receive the filename and size from the client
        if (recv_with_error_handling(sender_socket, buffer, sizeof(buffer),
            "Error Receive filename and size failed") <= 0)
            return 0;

        // Get file name, file size
        char file_name[1024];
        int file_size;
        sscanf(buffer, "UPLD %s %d", file_name, &file_size);
        printf("File name: %s, file size: %d\n", file_name, file_size);

        // Ask client to send files 
        send_with_error_handling(
            sender_socket,
            buffer,
            "+OK Please send file",
            "Error sending file request message"
        );

        // Receive file
        char file_path[1024*2];
        sprintf(file_path, "%s/%s", full_directory, file_name);
        FILE *file = fopen(file_path, "wb");
        if (file == NULL) {
            perror("Error opening file for writing");
            return 0;
        }

        int received = 0;
        while (received < file_size) {
            int bytes_received = recv_with_error_handling(
                sender_socket, 
                buffer, 
                sizeof(buffer), 
                "Error receiving data from the client"
            );
            if(bytes_received <= 0) {
                break;
            }
            fwrite(buffer, 1, bytes_received, file);
            received += bytes_received;
        }

        fclose(file);
        send_with_error_handling(
            sender_socket,
            buffer,
            "+OK Successful upload",
            "Error sending success message"
        );
    }
}
