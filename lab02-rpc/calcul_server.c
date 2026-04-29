#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "calcul.h"

/* Implementation of the calculator service functions */

reponse *
calcul_addition_1_svc(data *argp, struct svc_req *rqstp)
{
    static reponse result;
    unsigned int sum;
    
    printf("Received addition request: %u + %u\n", argp->arg1, argp->arg2);
    
    /* Check for potential overflow */
    if (argp->arg1 > UINT_MAX - argp->arg2) {
        result.errno = 1; /* Overflow error */
        result.somme = 0;
    } else {
        result.errno = 0; /* No error */
        result.somme = argp->arg1 + argp->arg2;
    }
    
    return &result;
}

reponse *
calcul_soustraction_1_svc(data *argp, struct svc_req *rqstp)
{
    static reponse result;
    
    printf("Received subtraction request: %u - %u\n", argp->arg1, argp->arg2);
    
    /* Check if result would be negative (underflow for unsigned) */
    if (argp->arg1 < argp->arg2) {
        result.errno = 1; /* Underflow error */
        result.somme = 0;
    } else {
        result.errno = 0; /* No error */
        result.somme = argp->arg1 - argp->arg2;
    }
    
    return &result;
}

reponse *
calcul_multiplication_1_svc(data *argp, struct svc_req *rqstp)
{
    static reponse result;
    
    printf("Received multiplication request: %u * %u\n", argp->arg1, argp->arg2);
    
    /* Check for multiplication overflow */
    if (argp->arg1 > 0 && argp->arg2 > 0 && argp->arg1 > UINT_MAX / argp->arg2) {
        result.errno = 1; /* Overflow error */
        result.somme = 0;
    } else {
        result.errno = 0; /* No error */
        result.somme = argp->arg1 * argp->arg2;
    }
    
    return &result;
}

reponse *
calcul_division_1_svc(data *argp, struct svc_req *rqstp)
{
    static reponse result;
    
    printf("Received division request: %u / %u\n", argp->arg1, argp->arg2);
    
    /* Check for division by zero */
    if (argp->arg2 == 0) {
        result.errno = 1; /* Division by zero error */
        result.somme = 0;
    } else {
        result.errno = 0; /* No error */
        result.somme = argp->arg1 / argp->arg2;
    }
    
    return &result;
}

/* No implementation needed for CALCUL_NULL */
void *
calcul_null_1_svc(void *argp, struct svc_req *rqstp)
{
    static char result;
    return (void *) &result;
}