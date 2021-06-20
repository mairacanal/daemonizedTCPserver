#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FLOW_SIZE 1000

int main(int argc, char** argv) {

    int s, port;
    char buf[MAX_FLOW_SIZE];
    struct sockaddr_in client;
    struct hostent *hp;

    hp = gethostbyname(argv[1]);
    if (((char *) hp) == NULL) {
        printf("Invalid host\n");
        return 1;
    } 

    memcpy((char*)&client.sin_addr, (char*)hp->h_addr, hp->h_length);
    client.sin_family = AF_INET;
    port = atoi(argv[2]);
    if (port <= 0) {
        printf("Invalid port\n");
        return 1;
    }
    client.sin_port = htons(port);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("Failed opening socket\n");
        return 1;
    }

    if (connect(s, (struct sockaddr *)&client, sizeof(client)) == -1) {
        printf("Connection failed\n");
        close(s);
        return 1;
    }

    while (1) {

        printf("Type message (type END to close): ");
        scanf("%s", buf);

        if (!strcmp(buf, "END")) {
            close(s);
            return 0;
        }

        printf("Typed message: [%s]\n", buf);
        printf("Buffer size: %zu\n", strlen(buf));

        if (send(s, buf, strlen(buf), 0) < 0) {
            close(s);
            printf("Connection lost");
            return 1;
        }
        memset(buf, 0, sizeof(buf));

        if(recv(s, buf, MAX_FLOW_SIZE, 0) <= 0) {
            close(s);
            return 1;
        }

        printf("Server returned %zu char: [%s]\n", strlen(buf), buf);
        memset(buf, 0, sizeof(buf));

    } 
    
}
