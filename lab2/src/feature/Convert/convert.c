#include "convert.h"

int convert_domain_to_ipv4(const char *domain) {
    struct addrinfo request;
    struct addrinfo *result, *tmp;
    int status;

    memset(&request, 0, sizeof(struct addrinfo));
    request.ai_family = AF_INET;
    request.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain, NULL, &request, &result);
    if (status != 0) {
        // Error
        return 0;
    }

    printf("Result: \n");
    for (tmp = result; tmp != NULL; tmp = tmp->ai_next) {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)tmp->ai_addr;
        char ipstr[STRING_LENGTH];
        inet_ntop(tmp->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        printf("%s\n", ipstr);
    }

    freeaddrinfo(result);

    return 1;
}

int convert_ipv4_to_domain(const char *ipv4) {
    struct in_addr ip_addr;
    if (inet_pton(AF_INET, ipv4, &ip_addr) <= 0) {
        return 0;
    }

    struct hostent *host;
    host = gethostbyaddr(&ip_addr, sizeof(struct in_addr), AF_INET);
    if (host == NULL) {
        return 0;
    }

    printf("Result: \n%s\n", host->h_name);
    char **alias;
    for (alias = host->h_aliases; *alias != NULL; alias++) {
        printf("%s\n", *alias);
    }
    return 1;
}
