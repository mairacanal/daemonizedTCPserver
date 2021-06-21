#include "server.h"

/*
 * @brief Behavior of the server thread receiving and sending message from an to the client
 * @params id Client unique file descriptor (id) 
 */

void* server_thread (void* id) {

    int sentBytes, recvBytes;
    char buf[MAX_FLOW_SIZE];

    do {

        memset(buf, 0, sizeof(buf));
        recvBytes = recv((int) id, buf, MAX_FLOW_SIZE, 0);
        if (recvBytes <= 0) {
            if (recvBytes < 0)
                syslog(LOG_INFO, "Thread %d: connection lost\n", (int) id);
            else
                syslog(LOG_INFO, "Thread %d: ending connection\n", (int) id);
        } else {
            sentBytes = send((int) id, buf, strlen(buf), 0);

            if (recvBytes < 0) {
                syslog(LOG_INFO, "Thread %d: connection lost\n", (int) id);
            }
            else
                syslog(LOG_INFO, "Thread %d - message send: [%s]\n", (int) id, buf);
        }

    } while (recvBytes > 0);

    close((int) id);
    pthread_exit(0);

}

/*
 * @brief Initialize TCP socket
 * @params port TCP port number
 */

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

/*
 * @brief Accept client connection and creates a client thread
 * @params s Socket file descriptor
 */

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

/*
 * @brief Closes TCP socket
 * @params s Socket file descriptor
 */

int socket_close (int s) {

    return close(s);

}
