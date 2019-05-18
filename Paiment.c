#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "vente.c"

FILE *p_out;
FILE *p_in;

typedef struct{//un paiement
int paymentCode;
Vente *v;
float amount;
char *type,*date;
}Payment;


typedef struct PaymentElement{//une liste chainée de paiements
Payment *payment;
struct PaymentElement*next;
}PaymentElement; 


PaymentElement *trouverFinp(PaymentElement *P)
{
    PaymentElement *ptr;

    for(ptr=P; ptr->next!=NULL; ptr=ptr->next);

    return ptr;
}


void addpaimentToList(PaymentElement **List , Payment *cl){
	
	if(*List==NULL){

		*List=(PaymentElement*)malloc(sizeof(PaymentElement));
		(*List)->payment=cl;
		(*List)->next=NULL;
		
	}
	else{

		PaymentElement *new;
		new=(PaymentElement*)malloc(sizeof(PaymentElement));
		new->payment=cl;
		new->next=NULL;
		(trouverFinp(*List))->next=new;
	
	}
	
}


void savepaimenttodb(Payment *l){
	p_out=fopen("paiments.txt","a");
	fprintf(p_out,"%d %d %f %s\n",l->paymentCode,l->v->code,l->amount,l->type);
	fflush(p_out);
	fclose(p_out);
}

void savepaimenlttodb(PaymentElement **l){
	PaymentElement *ptr=*l;
	while(ptr!=NULL){
		savepaimenttodb(ptr->payment);
		ptr=ptr->next;
	}
}


void getAllpaimentfromdb(PaymentElement **v){
	p_in=fopen("paiments.txt","r");
	Payment *p;
	PaymentElement *lp;
	int y;
	p=(Payment*)malloc(sizeof(Payment));
	p->v=(Vente*)malloc(sizeof(Vente));
	p->type=(char*)malloc(3*sizeof(char));
	lp=(PaymentElement*)malloc(sizeof(PaymentElement));
	while (fscanf(p_in,"%d %d %f %s\n",&p->paymentCode,&y,&p->amount,p->type)!=EOF){
		p->v=getvente(y);
		addpaimentToList(v,p);
	p=(Payment*)malloc(sizeof(Payment));
	p->v=(Vente*)malloc(sizeof(Vente));
	p->type=(char*)malloc(12*sizeof(char));
//		printf("%d \n",p->code);
	}
	fflush(p_in);
	fclose(p_in);
}


Payment* getpaiment(int c){
	PaymentElement *o;
	o=NULL;
	getAllpaimentfromdb(&o);	
	while(o!=NULL){
		if(o->payment->paymentCode==c){
			return o->payment;
		}
		o=o->next;
	}
	return NULL;
}

Payment* getpaimentbyvente(int c){
	PaymentElement *o;
	o=NULL;
	getAllpaimentfromdb(&o);	
	while(o!=NULL){
		if(o->payment->v->code==c){
			return o->payment;
		}
		o=o->next;
	}
	return NULL;
}

Payment* getpaimentbyv(PaymentElement **uy,int vc){
	while((*uy)!=NULL){
		if((*uy)->payment->v->code==vc) return (*uy)->payment;
		(*uy)=(*uy)->next;
	}
}



void displayonepaiment(Payment *p){
	printf("code paiment :%d\n",p->paymentCode);
	printf("Type de paiment :");
	if(strcmp(p->type,"es")==0) printf("Espece \n");
	else printf("Cheque \n");
	printf("----------------------\n");
	printf("TOTAL::%.2f\n",p->amount);
	printf("----------------------\n");
}

