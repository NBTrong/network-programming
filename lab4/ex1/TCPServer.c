#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 12345
#define BUFF_SIZE 1024

int main() {
    int listenfd, connfd;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in servAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int rcvBytes, sendBytes;

    // Step 1: Construct socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Bind address to socket
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Error: ");
        return 1;
    }

    // Step 3: Listen for client requests
    if (listen(listenfd, 10) < 0) {
        perror("Error! Cannot listen.");
        return 1;
    }

    printf("Server started!\n");

    while (1) {
        // Step 4: Accept incoming connection
        connfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);

        // Receive message from client
        while(1) {
            rcvBytes = recv(connfd, buff, BUFF_SIZE, 0);
            if (rcvBytes < 0) {
                perror("Error: ");
            }
            else if (rcvBytes == 0) {
                printf("Client disconnected.\n");
                break;
            }
            else {
                buff[rcvBytes] = '\0';

                char *token = strtok(buff, "\r\n");
                while (token != NULL) {
                    printf("Received from client[%s:%d]: %s\n",
                        inet_ntoa(clientAddr.sin_addr),
                        ntohs(clientAddr.sin_port),
                        token);
                    sendBytes = send(connfd, token, strlen(token), 0);
                    if (sendBytes < 0) {
                        perror("Error: ");
                        break;
                    } else if (sendBytes == 0) {
                        printf("Client disconnected.\n");
                        break;
                    }
                    token = strtok(NULL, "\r\n");
                }
            }
        }
        close(connfd);
    }

    close(listenfd); // Close the listening socket when done
    return 0;
}
