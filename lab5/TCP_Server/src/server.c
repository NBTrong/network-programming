#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>

#include "global.h"
#include "./config/tcp.h"
#include "./feature/Auth/auth.h"
#include "./feature/Article/article.h"

/**
 * Handle the SIGCHLD signal, called when a child process terminates.
 *
 * This function is responsible for collecting termination status of child processes
 * and printing information about terminated child processes.
 *
 * @param signo The signal number (SIGCHLD) that triggers the function.
 */
void sig_chld(int signo);

/**
 * Handle communication with a client.
 *
 * This function manages the communication with a connected client. It sends a success
 * message upon connection and continuously receives and processes messages from the client.
 *
 * @param client_socket The socket descriptor for the connected client.
 */
void handleClient(int client_socket);

/**
 * Route incoming messages and perform corresponding actions.
 *
 * This function parses the incoming message, extracts the keyword and parameter,
 * and routes the message based on the keyword to specific functions.
 *
 * @param client_socket The socket descriptor for the connected client.
 * @param message The incoming message from the client.
 */
void router(int client_socket, const char *message);

int main(int argc, char *argv[])
{
    // Handle argv
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [Port_Number]\n", argv[0]);
        exit(1);
    }
    int port_number = atoi(argv[1]);

    // Init server
    int server_socket = init_server(port_number);

    // Communicate with clients
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    client_len = sizeof(client_addr);
    signal(SIGCHLD, sig_chld);
    while (1)
    {
        // Accept connect with client
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                perror("Error accepting connection");
                return 0;
            }
        }

        // Fork process
        pid_t pid = fork();
        if (pid < 0)
        {
            // Fork error
            perror("Error in fork");
            close(client_socket);
        }
        else if (pid == 0)
        {
            // In child process
            close(server_socket);
            handleClient(client_socket);
            close(client_socket);
            exit(0);
        }
        else
        {
            // In parent process
            close(client_socket);
        }
    }

    // Close server
    close(server_socket);
    return 0;
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("Child %d terminated\n", pid);
    }
}

void handleClient(int client_socket)
{
    char sendMessage[STRING_LENGTH];
    char recvMessage[STRING_LENGTH];

    send_with_error_handling(client_socket, sendMessage, int_to_string(CONNECTED_SUCCESSFULLY), "Send message failed");
    while (recv_with_error_handling(
        client_socket,
        recvMessage,
        sizeof(recvMessage),
        "Error receiving data from the client"))
    {
        router(client_socket, recvMessage);
    }

    return;
}

void router(int client_socket, const char *message)
{
    printf("Recv from client: %s\n", message);
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
