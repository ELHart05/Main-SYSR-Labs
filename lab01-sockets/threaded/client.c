/**
 * Client de chat par socket - Version parallèle avec lecture depuis fichier
 * Programme permettant à un client de se connecter à un serveur et d'échanger
 * des messages automatiquement depuis un fichier texte SYSR - TP2 Sockets -
 * 2025
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
#include <unistd.h>

// Variable globale pour indiquer l'état de la connexion
volatile bool connexion_active = true;
int socketID;  // Socket global pour être accessible par les deux threads

// Structure pour passer des arguments au thread
typedef struct {
    int socket;
    char *fichier_messages;  // Chemin vers le fichier de messages
} thread_args_t;

// Fonction exécutée par le thread de réception
void *thread_reception(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int socketID = args->socket;
    char message[255];

    printf("Thread de réception démarré\n");

    // Boucle de réception des messages
    while (connexion_active) {
        // Réinitialisation du buffer
        memset(message, 0, sizeof(message));

        // Réception d'un message
        int bytes_received = recv(socketID, message, sizeof(message) - 1, 0);

        if (bytes_received <= 0) {
            // Erreur ou connexion fermée
            if (bytes_received == 0) {
                printf("\nServeur déconnecté\n");
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

        // Vérification si le serveur a envoyé EXIT
        if (strcmp(message, "EXIT") == 0) {
            printf("Le serveur a terminé la conversation\n");
            connexion_active = false;
            break;
        }
    }

    pthread_exit(NULL);
}

// Fonction exécutée par le thread d'envoi
void *thread_envoi(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    int socketID = args->socket;
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
        if (send(socketID, message, strlen(message), 0) == -1) {
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
        send(socketID, message, strlen(message), 0);
        connexion_active = false;
    }

    fclose(fichier);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc != 3) {
        printf("Usage: %s <adresse_ip_serveur> <fichier_messages>\n", argv[0]);
        exit(-1);
    }

    char *ip_serveur = argv[1];
    char *fichier_messages = argv[2];

    // Vérification que le fichier existe
    FILE *test_fichier = fopen(fichier_messages, "r");
    if (test_fichier == NULL) {
        perror("Erreur: le fichier de messages n'existe pas");
        exit(-1);
    }
    fclose(test_fichier);

    // Déclaration et initialisation de la structure contenant les informations
    // du serveur
    struct sockaddr_in informations;
    informations.sin_family = AF_INET;    // Utilisation du protocole IP
    informations.sin_port = htons(6666);  // Port du serveur
    informations.sin_addr.s_addr =
        inet_addr(ip_serveur);  // Adresse IP du serveur

    // Création du socket
    socketID = socket(AF_INET, SOCK_STREAM, 0);
    if (socketID == -1) {
        perror("Erreur lors de la création du socket");
        exit(-1);
    }

    printf("Socket créé avec succès\n");

    // Connexion au serveur
    if (connect(socketID, (struct sockaddr *)&informations,
                sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(socketID);
        exit(-1);
    }

    printf("Connexion au serveur %s réussie\n", ip_serveur);
    printf("Messages automatiques depuis: %s\n", fichier_messages);

    // Préparation des arguments pour les threads
    thread_args_t args;
    args.socket = socketID;
    args.fichier_messages = fichier_messages;

    // Déclaration des threads
    pthread_t thread_recv, thread_send;

    // Création des threads
    if (pthread_create(&thread_recv, NULL, thread_reception, &args) != 0) {
        perror("Erreur lors de la création du thread de réception");
        close(socketID);
        exit(-1);
    }

    if (pthread_create(&thread_send, NULL, thread_envoi, &args) != 0) {
        perror("Erreur lors de la création du thread d'envoi");
        connexion_active = false;
        pthread_cancel(thread_recv);
        close(socketID);
        exit(-1);
    }

    // Attente de la fin des threads
    pthread_join(thread_recv, NULL);
    pthread_join(thread_send, NULL);

    // Fermeture de la connexion
    printf("Fermeture de la connexion\n");
    shutdown(socketID, SHUT_RDWR);
    close(socketID);

    return 0;
}