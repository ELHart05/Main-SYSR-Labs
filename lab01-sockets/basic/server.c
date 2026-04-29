/*
 * SYSR - TP Sockets - Lab 01 (Basic)
 * TCP Chat Server - interactive tac-o-tac mode
 *
 * Usage: ./server
 * Listens on port 6666, accepts one client, then enters a loop where it
 * first waits for a message from the client, then sends its own reply.
 * The session ends when either side sends EXIT.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SERVER_PORT 6666
#define BUF_SIZE    255

int main(void)
{
    char phrase[BUF_SIZE];

    /* --- create listening socket --- */
    int srv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (srv_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* allow fast restart without "address already in use" */
    int opt = 1;
    setsockopt(srv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* --- bind --- */
    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family      = AF_INET;
    srv_addr.sin_port        = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(srv_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* --- listen --- */
    if (listen(srv_sock, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", SERVER_PORT);

    /* --- accept one client --- */
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    memset(&cli_addr, 0, sizeof(cli_addr));

    int conn = accept(srv_sock, (struct sockaddr *)&cli_addr, &cli_len);
    if (conn == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted from %s\n", inet_ntoa(cli_addr.sin_addr));

    /* --- tac-o-tac loop --- */
    do {
        /* receive */
        printf("\nWaiting for client message...\n");
        memset(phrase, 0, BUF_SIZE);
        ssize_t n = recv(conn, phrase, BUF_SIZE - 1, 0);
        if (n <= 0) {
            printf("Client closed connection.\n");
            break;
        }
        printf("Received: %s\n", phrase);

        if (strcmp(phrase, "EXIT") == 0) break;

        /* send */
        printf("Enter reply: ");
        if (scanf(" %254s", phrase) != 1) break;

        if (send(conn, phrase, strlen(phrase), 0) == -1) {
            perror("send");
            break;
        }
        printf("Reply sent.\n");

    } while (strcmp(phrase, "EXIT") != 0);

    shutdown(srv_sock, SHUT_RDWR);
    return 0;
}
