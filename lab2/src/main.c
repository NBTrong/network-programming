#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./feature/Convert/convert.h"

int main(int argc, char *argv[]) {

    // ---------------------------------- Handle argv ---------------------------------------
    
    if (argc != 2) {
        printf("Usage: %s parameter\n", argv[0]);
        return 1;
    }
    const char *parameter = argv[1];

    // ---------------------------------- Processing ---------------------------------------

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int response;

    // Check if the buffer contains an IP address or a domain
    if (inet_pton(AF_INET, parameter, &server_addr.sin_addr) == 1) {
        response = convert_ipv4_to_domain(parameter);
    } else {
        response = convert_domain_to_ipv4(parameter);
    }

    // Response != 1 is not found
    if (response != 1) {
        char* error_response = "Not found information\0";
        printf("%s\n", error_response);
    }

    return 0;
}
