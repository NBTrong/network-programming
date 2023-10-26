#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./feature/Convert/convert.h"
#include "./config/logger.h"

#define STRING_LENGTH 1024

int main(int argc, char *argv[]) {

    // ---------------------------------- Handle argv ---------------------------------------
    
    if (argc != 2) {
        printf("Usage: %s PortNumber\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);

    // ---------------------------------- Start server ---------------------------------------

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Server is ready to receive requests...\n");

    // ---------------------------------- Processing ---------------------------------------

    while (1) {
        addr_size = sizeof(client_addr);
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
        buffer[n] = '\0';

        printf("Received request: %s\n", buffer);

        char *response = NULL;

        // Check if the buffer contains an IP address or a domain
        if (inet_pton(AF_INET, buffer, &server_addr.sin_addr) == 1) {
            // Buffer contains an IP address
            response = convert_ipv4_to_domain(buffer);
        } else {
            // Buffer contains a domain
            response = convert_domain_to_ipv4(buffer);
        }

        if (response) {
            logger(buffer, response);
            sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, addr_size);
            free(response);
        } else {
            char* error_response = "–Not found information\0";
            logger(buffer, error_response);
            sendto(sockfd, error_response, strlen(error_response), 0, (struct sockaddr *)&client_addr, addr_size);
        }
    }

    close(sockfd);

    return 0;
}
