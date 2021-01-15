#include <sys/socket.h>

#include "common.h"

int send_all(int socket, void *buffer, int *length, int flags) {
    // Pointer as the return value is an error, so we pass bytes 
    // sent to the length memory address.
    int bytes_remaining = *length;
    int bytes_sent = 0; // total sent
    int current;

    while (bytes_sent < *length) {
        // Sends as many bytes as it can - offsetting the buffer by the amount
        // of bytes previously sent.
        current = send(socket, buffer + bytes_sent, bytes_remaining, flags);

        if (current == -1) {
            break;
        }

        bytes_sent += current;
        bytes_remaining -= current;
    }

    *length = bytes_sent;

    // Returns -1 on failure, 0 on success.
    return current == -1 ? -1 : 0;

}