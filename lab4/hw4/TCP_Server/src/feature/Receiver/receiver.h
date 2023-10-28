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

int file_receiving_protocol(int sender_socket);

#endif