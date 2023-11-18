#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Handle a server response message by extracting the status and message content.
 * @param buffer The string containing the message from the server.
 * @return A pointer to a string representing the status of the message (+OK or -ERR).
 */
char* handle_message(const char* buffer);

#endif