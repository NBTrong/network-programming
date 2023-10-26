#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // ---------------------------------- Handle argv ---------------------------------------

    if (argc != 3) {
        printf("Usage: %s IPAddress PortNumber\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    // ---------------------------------- Setup ---------------------------------------

    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char buffer[10000];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    // ---------------------------------- Process ---------------------------------------

    while (1) {
        printf("Enter an IP address or hostname (or press Enter to exit): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove the newline character

        if (strlen(buffer) <= 1) {
            break;  // Exit on an empty input
        }

        // Send the input to the server
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

        // Receive and display the response
        addr_size = sizeof(serverAddr);
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addr_size);
        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(sockfd);

    return 0;
}
