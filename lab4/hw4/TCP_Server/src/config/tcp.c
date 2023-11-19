#include "tcp.h"

int send_with_error_handling(const int server_socket, char *buffer, const char *message, const char *error_message)
{
    input(buffer, message);
    if (send(server_socket, buffer, strlen(buffer), 0) < 0)
    {
        perror(error_message);
        memset(buffer, 0, strlen(buffer));
        return 0;
    }
    printf("Send: %s\n", buffer);
    return 1;
};

int recv_with_error_handling(const int client_socket, char *buffer, size_t size, const char *error_message)
{
    memset(buffer, 0, strlen(buffer));
    int result = recv(client_socket, buffer, size, 0);
    if (result < 0)
    {
        perror(error_message);
    }
    else if (result == 0)
    {
        printf("Client disconnect\n");
        close(client_socket);
    }
    return result;
}

int init_server(int port_number)
{
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding");
        exit(1);
    }

    listen(server_socket, 5);

    printf("Server is listening on port %d\n", port_number);

    return server_socket;
}
