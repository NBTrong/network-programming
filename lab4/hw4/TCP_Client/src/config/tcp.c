#include "tcp.h"

int connect_server(const char* server_ip_address, int server_port_number) {
    int client_socket;
    struct sockaddr_in server_addr;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip_address);
    server_addr.sin_port = htons(server_port_number);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to the server");
        exit(1);
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
        perror("Error receiving welcome message");
    }
    handle_message(buffer);

    return client_socket;
}
