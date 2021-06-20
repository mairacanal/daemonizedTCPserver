#include "server.h"

void* server_thread (void* args) {

    int id, sentBytes, recvBytes;
    char buf[MAX_FLOW_SIZE];

    pthread_mutex_lock(&mut);
    id = ++threadId;
    pthread_mutex_unlock(&mut);

    do {

        memset(buf, 0, sizeof(buf));
        recvBytes = recv((int) args, buf, MAX_FLOW_SIZE, 0);
        if (recvBytes <= 0) {
            if (recvBytes < 0)
                syslog(LOG_INFO, "Thread %d: connection lost\n", id);
            else
                syslog(LOG_INFO, "Thread %d: ending connection\n", id);
            id = --threadId;
        } else {
            sentBytes = send((int) args, buf, strlen(buf), 0);

            if (recvBytes < 0) {
                syslog(LOG_INFO, "Thread %d: connection lost\n", id);
                id = --threadId;
            }
            else
                syslog(LOG_INFO, "Thread %d - message send: [%s]\n", id, buf);
        }

    } while (recvBytes > 0);

    close((int) args);
    pthread_exit(0);

}

int initialize_socket (int port) {

    int s;
    unsigned int sockLen;
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        syslog(LOG_INFO, "Failed opening socket\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if (port <= 0) {
        syslog(LOG_INFO, "Invalid port\n");
        close(s);
        return 1;
    }
    else server.sin_port = htons(port);

    if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        syslog(LOG_INFO, "Bind failed\n");
        close(s);
        return 1;
    }

    sockLen = sizeof(server);
    if (getsockname(s, (struct sockaddr *) &server, &sockLen) < 0) {
        syslog(LOG_INFO, "Failed getting socket name\n");
        close(s);
        return 1;
    }

    listen(s, QUEUE_LENGTH);
    return s;

}

int establish_connection (int s) {

    int id;
    unsigned int sockLen;
    struct sockaddr_in client;

    sockLen = sizeof(client);
    id = accept(s, (struct sockaddr *) &client, &sockLen);
    if (id < 0)
        syslog(LOG_INFO, "Socket can't accept connections\n");
    else {
        pthread_t thd;
        pthread_create(&thd, 0, (void *)server_thread, (void *) id);
    }

}

int socket_close (int s) {

    return close(s);

}
