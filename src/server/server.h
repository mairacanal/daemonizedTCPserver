#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syslog.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_LENGTH 5
#define MAX_FLOW_SIZE 1000

pthread_mutex_t mut;
int threadId;

int initialize_socket(int port);
int establish_connection(int s);
int socket_close(int s);

#endif
