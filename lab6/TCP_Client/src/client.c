#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libgen.h>
#include "./config/tcp.h"
#include "./feature/Sender/sender.h"

int main(int argc, char *argv[]) {
    // Handle argv
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [IP_Address] [Port_Number]\n", argv[0]);
        exit(1);
    }
    const char *ip_address = argv[1];
    int port_number = atoi(argv[2]);

    // Connect server
    int client_socket = connect_server(ip_address, port_number);

    // Start send file
    file_sending_protocol(client_socket);
    close(client_socket);

    return 0;
}
