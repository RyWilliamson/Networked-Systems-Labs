#ifndef MYCOMMON
#define MYCOMMON

#include <time.h>
#include <sys/socket.h>

#define PORT_STR "80"
#define MAX_DATA_SIZE 5000

/* Best effort attempt to send all buffer data including size header */
int send_message(int socket, char *buffer, int *size, int flags);

/* Receives all data from a message (no timeout just waits forever if incomplete message) */
int recv_message(int socket, int flags);

/* Logs the given times for debug */
void print_times(struct timespec *before, struct timespec *after);

/* Prints the IP address */
void print_ip(int family, struct sockaddr *ip_info);

#endif