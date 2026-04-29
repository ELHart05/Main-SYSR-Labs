#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "calcul.h"

CLIENT *clnt;

void test_addition(uint param1, uint param2)
{
    reponse *resultat;
    data parametre;
    
    /* 1. Prepare the arguments */
    parametre.arg1 = param1;
    parametre.arg2 = param2;
    printf("Appel de la fonction CALCUL_ADDITION avec les paramètres: %u et %u \n",
           parametre.arg1, parametre.arg2);
    
    /* 2. Call the remote function */
    resultat = calcul_addition_1(&parametre, clnt);
    if (resultat == (reponse *) NULL) {
        clnt_perror(clnt, "call failed");
        clnt_destroy(clnt);
        exit(EXIT_FAILURE);
    }
    else if (resultat->errno == 0) {
        printf("Le résultat de l'addition est: %u \n\n", resultat->somme);
    } else {
        printf("La fonction distante ne peut faire l'addition à cause d'un overflow \n\n");
    }
}

void test_soustraction(uint param1, uint param2)
{
    reponse *resultat;
    data parametre;
    
    /* 1. Prepare the arguments */
    parametre.arg1 = param1;
    parametre.arg2 = param2;
    printf("Appel de la fonction CALCUL_SOUSTRACTION avec les paramètres: %u et %u \n",
           parametre.arg1, parametre.arg2);
    
    /* 2. Call the remote function */
    resultat = calcul_soustraction_1(&parametre, clnt);
    if (resultat == (reponse *) NULL) {
        clnt_perror(clnt, "call failed");
        clnt_destroy(clnt);
        exit(EXIT_FAILURE);
    }
    else if (resultat->errno == 0) {
        printf("Le résultat de la soustraction est: %u \n\n", resultat->somme);
    } else {
        printf("La fonction distante ne peut faire la soustraction à cause d'un overflow \n\n");
    }
}

void test_multiplication(uint param1, uint param2)
{
    reponse *resultat;
    data parametre;
    
    /* 1. Prepare the arguments */
    parametre.arg1 = param1;
    parametre.arg2 = param2;
    printf("Appel de la fonction CALCUL_MULTIPLICATION avec les paramètres: %u et %u \n",
           parametre.arg1, parametre.arg2);
    
    /* 2. Call the remote function */
    resultat = calcul_multiplication_1(&parametre, clnt);
    if (resultat == (reponse *) NULL) {
        clnt_perror(clnt, "call failed");
        clnt_destroy(clnt);
        exit(EXIT_FAILURE);
    }
    else if (resultat->errno == 0) {
        printf("Le résultat de la multiplication est: %u \n\n", resultat->somme);
    } else {
        printf("La fonction distante ne peut faire la multiplication à cause d'un overflow \n\n");
    }
}

void test_division(uint param1, uint param2)
{
    reponse *resultat;
    data parametre;
    
    /* 1. Prepare the arguments */
    parametre.arg1 = param1;
    parametre.arg2 = param2;
    printf("Appel de la fonction CALCUL_DIVISION avec les paramètres: %u et %u \n",
           parametre.arg1, parametre.arg2);
    
    /* 2. Call the remote function */
    resultat = calcul_division_1(&parametre, clnt);
    if (resultat == (reponse *) NULL) {
        clnt_perror(clnt, "call failed");
        clnt_destroy(clnt);
        exit(EXIT_FAILURE);
    }
    else if (resultat->errno == 0) {
        printf("Le résultat de la division est: %u \n\n", resultat->somme);
    } else {
        printf("La fonction distante ne peut faire la division à cause d'une erreur \n\n");
    }
}

int main(int argc, char *argv[])
{
    char *host;
    
    if (argc < 2) {
        printf("usage: %s server_host\n", argv[0]);
        exit(1);
    }
    
    host = argv[1];
    clnt = clnt_create(host, CALCUL, VERSION_UN, "udp");
    
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
    
    /* Test all operations */
    test_addition(10, 20);
    test_addition(UINT_MAX, 1);
    
    test_soustraction(20, 10);
    test_soustraction(10, 20);
    
    test_multiplication(10, 20);
    test_multiplication(UINT_MAX, 2);
    
    test_division(20, 10);
    test_division(20, 0);
    
    clnt_destroy(clnt);
    exit(EXIT_SUCCESS);
}