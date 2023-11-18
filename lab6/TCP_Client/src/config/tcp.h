#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libgen.h>
#include "message.h"

/**
 * Connect to a server using the specified IP address and port number.
 * @param server_ip_address The IP address of the server to connect to.
 * @param server_port_number The port number on the server to establish the connection.
 * @return A socket descriptor representing the connection to the server, or -1 in case of an error.
 */
int connect_server(const char* server_ip_address, int server_port_number);


#endif