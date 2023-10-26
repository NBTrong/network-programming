#include "convert.h"

char* convert_domain_to_ipv4(const char *domain) {
    struct addrinfo request;
    struct addrinfo *result, *tmp;
    int status;

    memset(&request, 0, sizeof(struct addrinfo));
    request.ai_family = AF_INET;
    request.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain, NULL, &request, &result);
    if (status != 0) {
        // Error
        return NULL;
    }

    char *ip_list = malloc(STRING_LENGTH);
    ip_list[0] = '\0';

    for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)tmp->ai_addr;
        char ipstr[STRING_LENGTH];
        inet_ntop(tmp->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));

        if (ip_list[0] != '\0') {
            strcat(ip_list, " ");
        }
        strcat(ip_list, ipstr);
    }

    // Format ip_list
    char *response = malloc(STRING_LENGTH);
    sprintf(response, "+%s", ip_list);

    freeaddrinfo(result);

    return response;
}

char* convert_ipv4_to_domain(const char *ipv4) {
    struct sockaddr_in sa;
    char *host = malloc(STRING_LENGTH);

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, ipv4, &(sa.sin_addr));

    if (getnameinfo((struct sockaddr *)&sa, sizeof(struct sockaddr_in), host, STRING_LENGTH, NULL, 0, 0) != 0) {
        // Error
        return NULL;
    }

    // Format host
    char *response = malloc(STRING_LENGTH);
    sprintf(response, "+%s", host);

    free(host);
    host = NULL;

    return response;
}
