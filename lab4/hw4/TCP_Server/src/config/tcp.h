#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../utils/utils.h"

int send_with_error_handling(const int server_socket, char* buffer, const char* message, const char* error_message);

int recv_with_error_handling(const int client_socket, char* buffer, size_t size, const char* error_message);

int init_server(int port_number);
#endif