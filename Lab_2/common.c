#define _GNU_SOURCE // Needed or vscode complains about things from netdb.h and time.h not being defined.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "common.h"

void print_time(char const* label, struct timespec *time) {
    printf("%s: %ld.%09ld\n", label, (long) time->tv_sec, time->tv_nsec);
}

void print_times(struct timespec *before, struct timespec *after) {
    print_time("Before", before);
    print_time("After", after);
}

int send_message(int socket, char *buffer, int *size, int flags) {
    // Pointer as the return value is an error, so we pass bytes 
    // sent to the size memory address.
    int bytes_remaining = *size;
    int bytes_sent = 0; // total sent
    int current;
    struct timespec time;

    while (bytes_sent < *size) {
        // Sends as many bytes as it can - offsetting the buffer by the amount
        // of bytes previously sent.
        clock_gettime(CLOCK_MONOTONIC, &time);
        current = send(socket, buffer + bytes_sent, bytes_remaining, flags);
        print_time("Pre-send", &time);

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

int recv_check(ssize_t receive_count) {
    if (receive_count == -1) {
        fprintf(stderr, "Could not receive data\n");
        return -1;
    }
    if (receive_count == 0) {
        // Client closed connection because return value of 0 (Can't send zero data!)
        printf("\n");
        printf("Client Closed Connection\n");
        return 0;
    }
    return 1;
}

void print_buffer(char *buffer, int size, int offset) {
    for (int i = offset; i < size; i++) {
        printf("%c", buffer[i]);
    }
}

int recv_message(int socket, int flags) {
    int total_received = 0;
    int buffer_size = 1500;
    char buffer[buffer_size]; // Data will go here
    ssize_t receive_count; // In Bytes
    struct timespec time;

    int recv_err;

    // Loop until we have the whole message
    while (true) {
        clock_gettime(CLOCK_MONOTONIC, &time);
        receive_count = recv(socket, buffer, buffer_size, flags);
        print_time("Post-recv", &time);
        recv_err = recv_check(receive_count);
        if (recv_err == -1 || recv_err == 0) {return recv_err;}
        total_received += receive_count;
        // print_buffer(buffer, receive_count, 0);
    }
    return 0;
}