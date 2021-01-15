#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

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
        fprintf(stderr, "Could not setup socket.\n");
        exit(EXIT_FAILURE);
    }

    // Binds the socket created above to the network interface and port defined in server_address
    if (bind(listening_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        // Error
        fprintf(stderr, "Could not bind socket.\n");
        exit(EXIT_FAILURE);
    }

    // Instructs OS to start listening
    if (listen(listening_file_descriptor, backlog) == -1) {
        // Error
        fprintf(stderr, "Could not listen on socket.\n");
        exit(EXIT_FAILURE);
    }

    while (true) {
        printf("Waiting for next connection...\n");
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);

        int connection_file_descriptor = accept(listening_file_descriptor, (struct sockaddr *) &client_address, &client_address_length);
        if (connection_file_descriptor == -1) {
            // Error
            fprintf(stderr, "Could not accept connection.\n");
            exit(EXIT_FAILURE);
        }
        // Connected Successfully.

        int flags = 0; // No flags set

        if (recv_message(connection_file_descriptor, flags) == -1) {
            // Couldn't receive whole message
            fprintf(stderr, "Error when receiving data\n");
            closeSockets(listening_file_descriptor, connection_file_descriptor);
            exit(EXIT_FAILURE);
        }

        char *data = "Hello, client!";
        int data_len = strlen(data);
        int send_flags = MSG_NOSIGNAL;

        if (send_message(connection_file_descriptor, data, &data_len, send_flags) == -1) {
            // Error
            fprintf(stderr, "Error when sending data.\nOnly sent %d bytes!\n", data_len);
            closeSockets(listening_file_descriptor, connection_file_descriptor);
            exit(EXIT_FAILURE);
        }

        // Final recv call to wait for client to close socket first
        char buffer[MAX_MSG_SIZE];
        if (recv(connection_file_descriptor, buffer, MAX_MSG_SIZE, 0) != 0) {
            // Expecting socket close
            fprintf(stderr, "Error: Client should have closed socket not sent new message");
            closeSockets(listening_file_descriptor, connection_file_descriptor);
            exit(EXIT_FAILURE);
        }

        close(connection_file_descriptor);
        printf("Closed connection.\n");
    }
}