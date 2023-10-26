#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define STRING_LENGTH 1024

/**
 * Convert a domain name to its corresponding IPv4 addresses.
 * @param domain The domain name to be resolved to IPv4 addresses.
 * @return A string containing the space-separated list of IPv4 addresses, or NULL in case of an error.
 */
char* convert_domain_to_ipv4(const char *domain);

/**
 * Convert an IPv4 address to its corresponding domain name.
 * @param ipv4 The IPv4 address to be resolved to a domain name.
 * @return A string containing the domain name, or NULL in case of an error.
 */
char* convert_ipv4_to_domain(const char *ipv4);

#endif
