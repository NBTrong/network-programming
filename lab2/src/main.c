#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Please enter command line parameters\n");
        return 0; 
    };

    struct addrinfo* result;
    struct addrinfo* res;
    struct sockaddr_in *address;
    int error;
    char ipStr[INET_ADDRSTRLEN];

    error = getaddrinfo(argv[1], NULL, NULL, &result);
    if (error != 0) {
        if (error == EAI_SYSTEM) {
            perror("getaddrinfo");
        }
        else {
            fprintf(stderr, "error: %s\n", gai_strerror(error)); 
        }
        exit(EXIT_FAILURE);
    }
    address = (struct sockaddr_in *) result->ai_addr;
    inet_ntop(AF_INET, &address->sin_addr, ipStr, sizeof(ipStr));
    printf("IPv4 address: %s\n", ipStr);
    freeaddrinfo(result);

    return 1;
}