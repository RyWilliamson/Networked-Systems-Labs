#define _GNU_SOURCE // Needed or vscode complains about things from netdb.h not being defined.
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

#include "common.h"

int main(int argc, char *argv[]) {

    // Allows testing simultaneous connections on server.
    sleep(2);

    struct addrinfo hints, *address_info_head, *address_info;
    int i;
    int file_descriptor;
    char * ip_string;

    if (argc < 2) {
        fprintf(stderr, "Usage: hello_client <hostname>\n");
        exit(EXIT_FAILURE);
    }

    ip_string = argv[1];

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

        if (connect(file_descriptor, address_info->ai_addr, address_info->ai_addrlen) == -1) {
            // Can't connect to address. Try next node
            close(file_descriptor);
            continue;
        }
        break; // Successful connection
    }

    if (address_info == NULL) {
        fprintf(stderr, "Could not connect to %s\n", ip_string);
        exit(EXIT_FAILURE);
    }

    // Successfully connected to server so can now use the connection
    char data[MAX_DATA_SIZE];
    for (int i = 0; i < MAX_DATA_SIZE - 1; i++) {
        data[i] = 'a';
    }
    data[MAX_DATA_SIZE - 1] = '\0';
    int data_len = strlen(data);
    int send_flags = MSG_NOSIGNAL;

    if (send_message(file_descriptor, data, &data_len, send_flags) == -1) {
        // Error
        fprintf(stderr, "Error when sending data to %s\nOnly sent %d bytes!\n",
            ip_string, data_len);
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    int recv_flags = 0;
    if (recv_message(file_descriptor, recv_flags)) {
        fprintf(stderr, "Error when receiving data from %s\n", ip_string);
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    close(file_descriptor);
}