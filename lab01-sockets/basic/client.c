/*
 * SYSR - TP Sockets - Lab 01 (Basic)
 * TCP Chat Client - interactive tac-o-tac mode
 *
 * Usage: ./client
 * Connects to server at 127.0.0.1:6666.
 * Type a message, press Enter to send. The server replies before you can
 * send again. Type EXIT to terminate the session.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 6666
#define BUF_SIZE    255

int main(void)
{
    char phrase[BUF_SIZE];

    /* --- server address --- */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    /* --- create socket --- */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* --- connect --- */
    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    /* --- tac-o-tac loop --- */
    do {
        /* send */
        printf("\nEnter message: ");
        if (scanf(" %254s", phrase) != 1) break;

        if (send(sock, phrase, strlen(phrase), 0) == -1) {
            perror("send");
            break;
        }
        printf("Message sent.\n");

        /* receive (skip if we just sent EXIT) */
        if (strcmp(phrase, "EXIT") != 0) {
            memset(phrase, 0, BUF_SIZE);
            ssize_t n = recv(sock, phrase, BUF_SIZE - 1, 0);
            if (n <= 0) {
                printf("Server closed connection.\n");
                break;
            }
            printf("Received: %s\n", phrase);
        }

    } while (strcmp(phrase, "EXIT") != 0);

    shutdown(sock, SHUT_RDWR);
    return 0;
}
