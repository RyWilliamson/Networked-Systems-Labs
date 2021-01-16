#ifndef MYCOMMON
#define MYCOMMON

#define PORT 80
#define PORT_STR "80"
#define HEADER_SIZE 4 // 4 byte integer length
#define MAX_DATA_SIZE 5000
#define MAX_MSG_SIZE HEADER_SIZE + MAX_DATA_SIZE

/* Best effort attempt to send all buffer data including size header */
int send_message(int socket, char *buffer, int *size, int flags);

/* Receives all data from a message (no timeout just waits forever if incomplete message) */
int recv_message(int socket, int flags);

#endif