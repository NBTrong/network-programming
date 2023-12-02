#include "auth.h"

int checkLoginStatus()
{
    if (strcmp(currentUser, "\0") != 0)
    {
        return LOGGED_IN;
    }
    return NOT_LOGGED_IN;
};

// --------------------------------------- Login function ----------------------------------------------

int verifyAccount(const char *account)
{
    FILE *file = fopen("./TCP_Server/database/account.txt", "r");

    if (file == NULL)
    {
        printf("Unable to open the file.\n");
        exit(1);
    }

    char username[STRING_LENGTH + 1]; // +1 for null terminator
    int status;

    while (fscanf(file, "%100s %d", username, &status) == 2)
    {
        if (strcmp(account, username) == 0)
        {
            fclose(file);

            if (status == BAN)
            {
                return ACCOUNT_BANNED;
            }
            else if (status == ACTIVE)
            {
                return ACCOUNT_VALID;
            }
        }
    }

    // If in database have username >= 100 ==> Database error
    if (strlen(username) >= STRING_LENGTH)
    {
        printf("Error: Username length exceeds maximum allowed length.\n");
        fclose(file);
        return UNDEFINED;
    }

    fclose(file);
    return ACCOUNT_NOT_EXIST;
}

void login(int client_socket, const char *username)
{
    char buffer[STRING_LENGTH];

    // Check the client's login status
    if (checkLoginStatus() == LOGGED_IN)
    {
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(ACCOUNT_ALREADY_LOGGED_IN),
            "Send message login status error");
        return;
    }

    // Verify account
    int result = verifyAccount(username);
    switch (result)
    {
    case ACCOUNT_BANNED:
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(ACCOUNT_LOCKED),
            "Send message login status error");
        break;

    case ACCOUNT_NOT_EXIST:
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(ACCOUNT_NOT_FOUND),
            "Send message login status error");
        break;

    case ACCOUNT_VALID:
        strcpy(currentUser, username);
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(ACCOUNT_EXISTS_AND_ACTIVE),
            "Send message login status error");
        break;
    case UNDEFINED:
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(DATABASE_ERROR),
            "Send message login status error");
        break;
    }
}

// --------------------------------------- Logout function ---------------------------------------------

void logout(int client_socket)
{
    char buffer[STRING_LENGTH];

    int result = checkLoginStatus();
    switch (result)
    {
    case LOGGED_IN:
        strcpy(currentUser, "\0");
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(LOGOUT_SUCCESSFULLY),
            "Send message login status error");
        break;

    case NOT_LOGGED_IN:
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(NOT_HAVE_ACCESS),
            "Send message login status error");
        break;

    default:
        break;
    }
}