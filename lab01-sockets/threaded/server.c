/**
 * Serveur de chat par socket - Version parallèle avec lecture depuis fichier
 * Programme permettant à un serveur d'accepter une connexion d'un client et
 * d'échanger des messages automatiquement depuis un fichier texte SYSR - TP2
 * Sockets - 2025
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

// Variable globale pour indiquer l'état de la connexion
volatile bool connexion_active = true;

// Structure pour passer des arguments au thread
typedef struct {
    int socket;
    char *fichier_messages;  // Chemin vers le fichier de messages
} thread_args_t;

// Fonction exécutée par le thread de réception
void *thread_reception(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int connexion = args->socket;
    char message[255];

    printf("Thread de réception démarré\n");

    // Boucle de réception des messages
    while (connexion_active) {
        // Réinitialisation du buffer
        memset(message, 0, sizeof(message));

        // Réception d'un message
        int bytes_received = recv(connexion, message, sizeof(message) - 1, 0);

        if (bytes_received <= 0) {
            // Erreur ou connexion fermée
            if (bytes_received == 0) {
                printf("\nClient déconnecté\n");
            } else {
                perror("\nErreur lors de la réception");
            }
            connexion_active = false;
            break;
        }

        // Affichage du message reçu avec timestamp
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_buffer[20];
        strftime(time_buffer, 20, "%H:%M:%S", tm_info);

        printf("\n[%s] Message reçu: %s\n", time_buffer, message);

        // Vérification si le client a envoyé EXIT
        if (strcmp(message, "EXIT") == 0) {
            printf("Le client a terminé la conversation\n");
            connexion_active = false;
            break;
        }
    }

    pthread_exit(NULL);
}

// Fonction exécutée par le thread d'envoi
void *thread_envoi(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int connexion = args->socket;
    char message[255];

    printf("Thread d'envoi démarré - Lecture depuis le fichier: %s\n",
           args->fichier_messages);

    // Ouverture du fichier de messages
    FILE *fichier = fopen(args->fichier_messages, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier de messages");
        connexion_active = false;
        pthread_exit(NULL);
    }

    // Boucle d'envoi des messages
    while (connexion_active &&
           fgets(message, sizeof(message), fichier) != NULL) {
        // Suppression du caractère de nouvelle ligne
        message[strcspn(message, "\n")] = 0;

        // Ignore les lignes vides
        if (strlen(message) == 0) {
            continue;
        }

        // Affichage du message qui sera envoyé
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_buffer[20];
        strftime(time_buffer, 20, "%H:%M:%S", tm_info);

        printf("[%s] Envoi du message: %s\n", time_buffer, message);

        // Envoi du message
        if (send(connexion, message, strlen(message), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
            connexion_active = false;
            break;
        }

        // Vérification si l'utilisateur veut quitter
        if (strcmp(message, "EXIT") == 0) {
            printf("Fin de la conversation (EXIT envoyé)\n");
            connexion_active = false;
            break;
        }

        // Pause entre les messages pour éviter l'envoi trop rapide
        sleep(2);
    }

    // Si on est sorti de la boucle sans EXIT, on envoie EXIT automatiquement
    if (connexion_active) {
        printf(
            "\nTous les messages du fichier ont été envoyés. Envoi de "
            "EXIT...\n");
        strcpy(message, "EXIT");
        send(connexion, message, strlen(message), 0);
        connexion_active = false;
    }

    fclose(fichier);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc != 2) {
        printf("Usage: %s <fichier_messages>\n", argv[0]);
        exit(-1);
    }

    char *fichier_messages = argv[1];

    // Vérification que le fichier existe
    FILE *test_fichier = fopen(fichier_messages, "r");
    if (test_fichier == NULL) {
        perror("Erreur: le fichier de messages n'existe pas");
        exit(-1);
    }
    fclose(test_fichier);

    // Déclaration des structures pour les informations de connexion
    struct sockaddr_in
        information_server;  // Structure pour les informations du serveur
    struct sockaddr_in
        information_client;  // Structure pour les informations du client

    // Création du socket
    int socketID = socket(AF_INET, SOCK_STREAM, 0);
    int connexion = 0;  // Descripteur de la connexion avec le client

    // Variable nécessaire pour la fonction accept
    socklen_t len = sizeof(struct sockaddr_in);

    // Vérification de la création du socket
    if (socketID == -1) {
        perror("Erreur lors de la création du socket");
        exit(-1);
    }

    printf("Socket créé avec succès\n");

    // Initialisation de la structure d'informations du serveur
    memset(&information_server, 0, sizeof(struct sockaddr_in));
    information_server.sin_port = htons(6666);  // Port d'écoute
    information_server.sin_family = AF_INET;    // Protocole IP
    information_server.sin_addr.s_addr =
        htonl(INADDR_ANY);  // Accepte les connexions de toutes les interfaces

    // Option pour permettre la réutilisation immédiate de l'adresse
    int opt = 1;
    if (setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
        -1) {
        perror("Erreur lors de la configuration du socket (setsockopt)");
        close(socketID);
        exit(-1);
    }

    // Attachement du socket à l'adresse et au port spécifiés
    if (bind(socketID, (struct sockaddr *)&information_server,
             sizeof(struct sockaddr)) == -1) {
        perror("Erreur lors de l'attachement du socket");
        close(socketID);
        exit(-1);
    }

    printf("Socket attaché avec succès\n");

    // Mise en écoute du socket (file d'attente de 5 connexions maximum)
    if (listen(socketID, 5) == -1) {
        perror("Erreur lors de la mise en écoute du socket");
        close(socketID);
        exit(-1);
    }

    printf("Serveur en écoute sur le port 6666...\n");
    printf("Messages automatiques depuis: %s\n", fichier_messages);

    // Préparation à l'acceptation d'une connexion
    memset(&information_client, 0, sizeof(struct sockaddr_in));

    // Acceptation d'une connexion entrante
    printf("En attente d'une connexion client...\n");
    connexion = accept(socketID, (struct sockaddr *)&information_client, &len);

    // Vérification de l'acceptation de la connexion
    if (connexion == -1) {
        perror("Erreur lors de l'acceptation de la connexion");
        close(socketID);
        exit(-1);
    }

    printf("Connexion acceptée de %s\n",
           inet_ntoa(information_client.sin_addr));

    // Préparation des arguments pour les threads
    thread_args_t args;
    args.socket = connexion;
    args.fichier_messages = fichier_messages;

    // Déclaration des threads
    pthread_t thread_recv, thread_send;

    // Création des threads
    if (pthread_create(&thread_recv, NULL, thread_reception, &args) != 0) {
        perror("Erreur lors de la création du thread de réception");
        close(connexion);
        close(socketID);
        exit(-1);
    }

    if (pthread_create(&thread_send, NULL, thread_envoi, &args) != 0) {
        perror("Erreur lors de la création du thread d'envoi");
        connexion_active = false;
        pthread_cancel(thread_recv);
        close(connexion);
        close(socketID);
        exit(-1);
    }

    // Attente de la fin des threads
    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);

    // Fermeture des connexions
    printf("Fermeture des connexions\n");
    shutdown(connexion, SHUT_RDWR);
    close(connexion);
    close(socketID);

    return 0;
}