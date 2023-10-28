#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define STRING_LENGTH 1024

/**
 * Convert a domain name to its corresponding IPv4 address.
 * 
 * This function takes a domain name as input and attempts to resolve it to its
 * corresponding IPv4 address using DNS (Domain Name System) lookup.
 * 
 * @param domain A string containing the domain name to be resolved to an IPv4 address.
 * @return An integer result of the conversion:
 *         - 1 for success (IPv4 address determined).
 *         - 0 for failure (unable to determine the IPv4 address).
 */
int convert_domain_to_ipv4(const char *domain);

/**
 * Convert an IPv4 address to its corresponding domain name.
 * 
 * This function takes an IPv4 address in string format (e.g., "8.8.8.8") and
 * resolves it to its corresponding domain name using DNS (Domain Name System) lookup.
 * 
 * @param ipv4 A string containing the IPv4 address to be resolved to a domain name.
 * @return An integer result of the conversion:
 *         - 1 for success (domain name determined).
 *         - 0 for failure (unable to determine the domain name).
 */
int convert_ipv4_to_domain(const char *ipv4);

#endif
