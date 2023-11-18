#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

/**
 * Log a network request and response along with client's IP address and port.
 * @param ip_address The IP address of the client making the request.
 * @param port The port number used for the client's connection.
 * @param request The request message or information.
 * @param response The response message or information.
 */
void logger(const char* ip_address, const int port, const char *request, const char *response);

#endif