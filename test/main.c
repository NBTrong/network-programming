#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    const char *targetIP = "127.0.0.1"; // Địa chỉ IP mục tiêu
    struct hostent *host;
    char **aliases;
    char **ip_addresses;
    struct in_addr ip;
    
    inet_aton(targetIP, &ip);
    host = gethostbyaddr((const void *) &ip, sizeof(struct in_addr), AF_INET);
    
    if (host == NULL) {
        printf("Không thể xác định tên miền cho %s\n", targetIP);
        return 1;
    }

    printf("Tên miền chính: %s\n", host->h_name);

    printf("Các bí danh (aliases):\n");
    aliases = host->h_aliases;
    while (*aliases != NULL) {
        printf("%s\n", *aliases);
        aliases++;
    }

    return 0;
}