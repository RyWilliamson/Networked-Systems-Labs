#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

#define BUFLEN 1500

void closeSockets(int listening_file_descriptor, int connection_file_descriptor) {
    close(connection_file_descriptor);
    close(listening_file_descriptor);
}

int main()
{
    // IPv4 Struct
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; // Accept any incoming messages
    server_address.sin_port = htons(PORT);

    int backlog = 10;

    // Sets up the socket over IPv4 for TCP connection
    int listening_file_descriptor = socket(AF_INET /*IPv4*/, SOCK_STREAM /*TCP*/, 0 /*0 for Internet*/);
    if (listening_file_descriptor == -1) {
        // Error
        printf("Could not setup socket.\n");
        exit(EXIT_FAILURE);
    }

    // Binds the socket created above to the network interface and port defined in server_address
    if (bind(listening_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        // Error
        printf("Could not bind socket.\n");
        exit(EXIT_FAILURE);
    }

    // Instructs OS to start listening
    if (listen(listening_file_descriptor, backlog) == -1) {
        // Error
        printf("Could not listen on socket.\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    int connection_file_descriptor = accept(listening_file_descriptor, (struct sockaddr *) &client_address, &client_address_length);
    if (connection_file_descriptor == -1) {
        // Error
        printf("Could not accept connection.\n");
        exit(EXIT_FAILURE);
    }

    ssize_t receive_count; // In Bytes
    char buffer[BUFLEN]; // Data will go here
    int flags = 0; // No flags set

    while (true) {
        receive_count = recv(connection_file_descriptor, buffer, BUFLEN, flags);
        if (receive_count == -1) {
            // Error
            printf("Could not receive data\n");
            closeSockets(listening_file_descriptor, connection_file_descriptor);
            exit(EXIT_FAILURE);
        }

        if (receive_count == 0) {
            // Client closed connection because return value of 0 (Can't send zero data!)
            break;
        }

        for (int i = 0; i < receive_count; i++) {
            printf("%c", buffer[i]);
        }
    }
    printf("\n");
    

    closeSockets(listening_file_descriptor, connection_file_descriptor);
}