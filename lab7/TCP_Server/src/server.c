#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "global.h"
#include "./config/tcp.h"
#include "./feature/Auth/auth.h"
#include "./feature/Session/session.h"
#include "./feature/Article/article.h"

void router(int client_socket, const char *message);
int handle_client(void *arg);

#define BACKLOG 20

int main(int argc, char *argv[])
{
    // ----------------------------------- Handle argv -----------------------------------
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [Port_Number]\n", argv[0]);
        exit(1);
    }
    int port_number = atoi(argv[1]);

    // ----------------------------------- Initialize -----------------------------------
    int *client_socket;
    int i, maxi, max_fd, server_socket, conn_fd, sock_fd;
    int ready_count, client[FD_SETSIZE];
    fd_set readfds, all_set;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    server_socket = init_server(port_number);
    max_fd = server_socket;

    // ----------------------------------- Setup multiplexing --------------------------
    maxi = -1;
    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1; /* -1 indicates available entry */
    FD_ZERO(&all_set);
    FD_SET(server_socket, &all_set);

    // Step 4: Communicate with clients
    while (1)
    {
        readfds = all_set; /* structure assignment */
        ready_count = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (ready_count < 0)
        {
            perror("\nError: ");
            return 0;
        }

        if (FD_ISSET(server_socket, &readfds))
        {
            client_len = sizeof(client_addr);
            if ((conn_fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) < 0)
                perror("\nError: ");
            else
            {
                char sendMessage[STRING_LENGTH];
                printf("Client %d request connect\n", conn_fd);
                send_with_error_handling(conn_fd, sendMessage, int_to_string(CONNECTED_SUCCESSFULLY), "Send message failed");
                for (i = 0; i < FD_SETSIZE; i++)
                    if (client[i] < 0)
                    {
                        client[i] = conn_fd; /* save descriptor */
                        break;
                    }
                if (i == FD_SETSIZE)
                    printf("\nToo many clients");
                FD_SET(conn_fd, &all_set);
                if (conn_fd > max_fd)
                    max_fd = conn_fd;
                if (i > maxi)
                    maxi = i;
                if (--ready_count <= 0)
                    continue;
            }
        }

        // --------------------------------- Handle request from client ------------------------------
        for (i = 0; i <= maxi; i++)
        {
            if ((sock_fd = client[i]) < 0)
                continue;
            if (FD_ISSET(sock_fd, &readfds))
            {
                client_socket = malloc(sizeof(int));
                *client_socket = sock_fd;
                int result = handle_client(client_socket);
                if (result == 0)
                {
                    close(sock_fd);
                    FD_CLR(sock_fd, &all_set);
                    client[i] = -1;
                }
                if (--ready_count <= 0)
                    break;
            }
        }
    }
    return 0;
}

int handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    char recvMessage[STRING_LENGTH];
    free(arg);

    printf("%d\n", client_socket);

    int ret = recv_with_error_handling(
        client_socket,
        recvMessage,
        sizeof(recvMessage),
        "Error receiving data from the client");
    if (ret > 0)
    {
        router(client_socket, recvMessage);
        return 1;
    }
    delete_session_by_socket_id(client_socket);
    close(client_socket);
    return 0;
}

void router(int client_socket, const char *message)
{
    printf("Recv from client %d: %s\n", client_socket, message);
    char keyword[STRING_LENGTH];
    char parameter[STRING_LENGTH];
    char buffer[STRING_LENGTH];
    sscanf(message, "%s %[^\n]", keyword, parameter);

    if (strcmp(keyword, "USER") == 0)
    {
        login(client_socket, parameter);
    }
    else if (strcmp(keyword, "POST") == 0)
    {
        postArticle(client_socket, parameter);
    }
    else if (strcmp(keyword, "BYE") == 0)
    {
        logout(client_socket);
    }
    else
    {
        send_with_error_handling(client_socket, buffer, "300", "Send message failed");
    }
};
