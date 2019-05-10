#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "vente.c"

FILE *p_out;
FILE *p_in;

typedef struct{//un paiement
int paymentCode;
double amount;
char *type,*date;
Vente *sale;//apartient � une vente
}Payment;


typedef struct PaymentElement{//une liste chain�e de paiements
Payment*payment;
struct PaymentElement*next;
}PaymentElement; 



