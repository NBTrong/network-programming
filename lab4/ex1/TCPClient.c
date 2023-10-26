#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_ADDR "127.0.0.1"
#define SERV_PORT 12345
#define BUFF_SIZE 1024

int main() {
    int clientfd;
    char buff[BUFF_SIZE + 1];
    struct sockaddr_in servaddr;
    int ret;

    // Step 1: Construct socket
    clientfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Specify server's address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERV_ADDR);
    servaddr.sin_port = htons(SERV_PORT);

    // Step 4: Connect to the server
    if (connect(clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error: ");
        return 1;
    }

    // Step 5: Communicate with the server
    // Send a message to the server
    printf("Send to server: ");
    fgets(buff, BUFF_SIZE, stdin);
    buff[strlen(buff) - 1] = '\0'; // Remove the newline character
    sprintf(buff + strlen(buff), "\r\n");
    ret = send(clientfd, buff, strlen(buff), 0);
    if (ret < 0) {
        perror("Error: ");
        return 1;
    }

    printf("Send to server: ");
    fgets(buff, BUFF_SIZE, stdin);
    buff[strlen(buff) - 1] = '\0'; // Remove the newline character
    sprintf(buff + strlen(buff), "\r\n");
    ret = send(clientfd, buff, strlen(buff), 0);
    if (ret < 0) {
        perror("Error: ");
        return 1;
    }
    while(1) {
        // Receive the echoed message from the server
        ret = recv(clientfd, buff, BUFF_SIZE, 0);
        if (ret < 0) {
            perror("Error: ");
            return 1;
        }
        printf("Receive from server: %s", buff);
    }


    close(clientfd);
    return 0;
}
