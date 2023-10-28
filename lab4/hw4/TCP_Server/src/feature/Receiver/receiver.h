#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../config/tcp.h"
#include "../../global.h"
#include "../../config/logger.h"
#include "../../utils/utils.h"

/**
 * Implement a file receiving protocol on the specified sender socket.
 * This function handles the process of receiving a file from the client.
 * @param sender_socket The socket descriptor for the sender (client).
 * @param client_ip The IP address of the client sending the file.
 * @param client_port The port number used for the client's connection.
 * @return 0 on successful file reception, or -1 in case of an error.
 */
int file_receiving_protocol(int sender_socket, const char* client_ip, const int client_port);


#endif