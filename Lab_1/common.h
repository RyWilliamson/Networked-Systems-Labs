#ifndef MYCOMMON
#define MYCOMMON

#define PORT 5000
#define PORT_STR "5000"

/* Best effort attempt to send all buffer data */
int send_all(int socket, void *buffer, int *length, int flags);

#endif