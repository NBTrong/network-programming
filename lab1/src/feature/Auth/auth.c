#include "auth.h"

int checkLoginStatus() {
    if (strcmp(currentUser, "\0") != 0) {
        return LOGGED_IN;
    };
    return NOT_LOGGED_IN;
};

// --------------------------------------- Login function ----------------------------------------------

int verifyAccount(char* account) {
    if (checkLoginStatus() == LOGGED_IN) {
        return LOGGED_IN;
    };

    // Read file
    FILE *file = fopen("./database/account.txt", "r"); // Open the file in read mode ("r")

    if (file == NULL) {
        printf("Unable to open the file.\n");
        exit(1); // Exit the program with an error code
    }

    char username[STRING_LENGTH]; // Variable to store the username
    int status; // Variable to store the status

    // Read and display information from the file
    while (fscanf(file, "%s %d", username, &status) == 2) {
        // Banned account
        if (strcmp(account, username) == 0 && status == BAN) {
            return ACCOUNT_BANNED;
        }

        // Valid account
        if (strcmp(account, username) == 0 && status == ACTIVE) {
            return ACCOUNT_VALID;
        }
    }

    fclose(file); // Close the file after usage

    return ACCOUNT_NOT_EXIST;
};

void login() {
    char username[STRING_LENGTH];
    
    // ----------------------------------------------- Input username ------------------------------------------------------------
    printf("Enter username: ");
    input(username, "string");

    // ---------------------------------------------- Verify account -------------------------------------------------------------
    int result = verifyAccount(username);

    switch (result)
    {
    case LOGGED_IN:
        logger("1", username, "-ERROR", "You have already logged in\n");
        break;

    case ACCOUNT_BANNED: 
        logger("1", username, "-ERROR", "Account is banned \n");
        break;
    
    case ACCOUNT_NOT_EXIST: 
        logger("1", username, "-ERROR", "Account is not exist\n");
        break;
    
    case ACCOUNT_VALID:
        // Save username 
        strcpy(currentUser, username);

        // Logger
        char message[STRING_LENGTH*2];
        snprintf(message, sizeof(message), "Hello %s\n", username);
        logger("1", username, "+OK", message);
        break;

    default:
        break;
    }
}

// --------------------------------------- Logout function ---------------------------------------------

void logout() {
    int result = checkLoginStatus();

    switch (result)
    {
    case LOGGED_IN:
        // Remove currentUser value
        strcpy(currentUser, "\0");

        logger("3", "", "+OK", "Successful log out\n");
        break;

    case NOT_LOGGED_IN:
        logger("3", "", "-ERROR", "You have not logged in.\n");
        break;

    default:
        break;
    }

}