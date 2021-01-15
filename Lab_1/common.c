#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "common.h"

int add_header(void *destination, void *source, int size) {
    int network_order_size = htonl(size);
    memcpy(destination, &network_order_size, HEADER_SIZE);
    memcpy(destination+HEADER_SIZE, source, size);
    return size + HEADER_SIZE;
}

int send_message(int socket, char *buffer, int *size, int flags) {
    // Pointer as the return value is an error, so we pass bytes 
    // sent to the size memory address.
    char buffer_with_header[HEADER_SIZE + *size];
    int new_length = add_header(buffer_with_header, buffer, *size);
    int bytes_remaining = new_length;
    int bytes_sent = 0; // total sent
    int current;

    while (bytes_sent < new_length) {
        // Sends as many bytes as it can - offsetting the buffer by the amount
        // of bytes previously sent.
        current = send(socket, buffer_with_header + bytes_sent, bytes_remaining, flags);

        if (current == -1) {
            break;
        }

        bytes_sent += current;
        bytes_remaining -= current;
    }

    *size = bytes_sent;

    // Returns -1 on failure, 0 on success.
    return current == -1 ? -1 : 0;
}

// int recv_error_checking(int socket, char *buffer, int size, int flags) {
//     ssize_t receive_count = recv(socket, buffer, size, flags);
//     if (receive_count == -1) {

//     }
// }

int recv_check(ssize_t receive_count) {
    if (receive_count == -1) {
        fprintf(stderr, "Could not receive data\n");
        return -1;
    }
    if (receive_count == 0) {
        // Client closed connection because return value of 0 (Can't send zero data!)
        fprintf(stderr, "Client Closed Connection\n");
        return -1;
    }
    return 0;
}

void print_buffer(char *buffer, int size, int offset) {
    for (int i = offset; i < size; i++) {
        printf("%c", buffer[i]);
    }
}

int recv_message(int socket, int flags) {
    int message_size, total_received;
    int buffer_size = 1500;
    char buffer[buffer_size]; // Data will go here
    ssize_t receive_count; // In Bytes
    bool message_incomplete;

    // Need to receive atleast one packet so we can retrieve header info.
    receive_count = recv(socket, buffer, buffer_size, flags);
    if (recv_check(receive_count) == -1) {return -1;}
    total_received = receive_count - HEADER_SIZE;

    // Get message size
    memcpy(&message_size, buffer, HEADER_SIZE);
    message_size = ntohl(message_size);
    message_incomplete = message_size > total_received;

    // Need to print buffer atleast once (offset by HEADER_SIZE so that we don't print non-characters)
    print_buffer(buffer, receive_count, HEADER_SIZE);

    // Loop until we have the whole message
    while (message_incomplete) {
        receive_count = recv(socket, buffer, buffer_size, flags);
        if (recv_check(receive_count) == -1) {return -1;}
        total_received += receive_count;
        message_incomplete = message_size > total_received;
        print_buffer(buffer, receive_count, 0);
    }
    printf("\n");
    return 0;
}