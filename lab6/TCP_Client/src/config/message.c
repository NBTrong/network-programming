#include "message.h"

char* handle_message(const char* buffer) {
    char status[10];
    char message[1024];
    if (sscanf(buffer, "%9s %[^\n]", status, message) == 2) {
        if (strcmp(status, "+OK") == 0 || strcmp(status, "-ERR") == 0) {
            printf("Server: \"%s\", \"%s\"\n", status, message);
        } else {
            printf("Server: Invalid status.\n");
        }
    } else {
        printf("Server: Invalid format.\n");
    }
    return strdup(status);
}
