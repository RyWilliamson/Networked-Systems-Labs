#define _GNU_SOURCE // Needed or vscode complains about things from netdb.h and time.h not being defined.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT_STR "80"
#define MAX_DATA_SIZE 5000

// Prints IP in the format hostname IPvX ip_address
void print_ip(int family, struct sockaddr *ip_info, char* hostname) {
    if (family == AF_INET) {
        // IPv4
        struct sockaddr_in *info = (struct sockaddr_in *) ip_info;
        char buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &info->sin_addr, buffer, INET_ADDRSTRLEN);
        printf("%s IPv4 %s", hostname, buffer);
    } else {
        // IPv6
        struct sockaddr_in6 *info = (struct sockaddr_in6 *) ip_info;
        char buffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &info->sin6_addr, buffer, INET6_ADDRSTRLEN);
        printf("%s IPv6 %s", hostname, buffer);
    }
}

int main(int argc, char *argv[]) {

    // Not enough arguments provided
    if (argc < 2) {
        fprintf(stderr, "Usage: dnslookup <hostname(s)>\n");
        exit(EXIT_FAILURE);
    }

    for (int j = 1; j < argc; j++) {
        struct addrinfo hints, *address_info_head, *address_info;
        int i;
        int file_descriptor;
        char * ip_string;

        // Getting the jth ip argument given in command
        ip_string = argv[j];

        // Setup hints
        memset(&hints, 0, sizeof(hints)); // Fills hints with 0 value bytes.
        hints.ai_family = PF_UNSPEC; // Unspecified protocol so can be either IPv4 OR IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP Socket

        // Performs the dns lookup - head of linked list with possible addresses is set to address_info_head
        if ((i = getaddrinfo(ip_string, PORT_STR, &hints, &address_info_head)) != 0) {
            // Error
            fprintf(stderr, "Could not find address for %s. Make sure the address was typed correctly.\n", ip_string);
            exit(EXIT_FAILURE);
        }

        for (address_info = address_info_head; address_info != NULL; address_info = address_info->ai_next) {
            file_descriptor = socket(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);

            if (file_descriptor == -1) {
                // Can't create socket try next node
                continue;
            }

            print_ip(address_info->ai_family, address_info->ai_addr, ip_string);

            close(file_descriptor);
            printf("\n");
        }
    }
}