#include "message.h"

// -------------------------------------- Post function ------------------------------------------------

void postMessage() {
    char message[STRING_LENGTH];

    printf("Post message: ");
    input(message, "string");

    int result = checkLoginStatus();
    if (result == NOT_LOGGED_IN) {
        logger("2", message, "-ERROR", "You have not logged in.\n");
        return;
    }

    logger("2", message, "+OK", "Successful post \n");
}