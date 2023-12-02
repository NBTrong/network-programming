#include "article.h"

// -------------------------------------- Post function ------------------------------------------------

void postArticle(int client_socket, char *article)
{
    char buffer[STRING_LENGTH];

    // Check login status
    if (checkLoginStatus(client_socket) == NOT_LOGGED_IN)
    {
        send_with_error_handling(
            client_socket,
            buffer,
            int_to_string(NOT_HAVE_ACCESS),
            "Send message login status error");
        return;
    }
    send_with_error_handling(
        client_socket,
        buffer,
        int_to_string(POST_SUCCESSFULLY),
        "Send message login status error");

    printf("Client post article: %s\n", article);
}