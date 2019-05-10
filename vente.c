#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Lignecmd.c"


FILE *v_out;
FILE *v_in;

typedef struct Vente		
{
    int code ;
	Client *c; 
    char* date_c;
    LignecmdElement *lcmd;
    float total;
} Vente;
typedef struct VenteElement
{
    Vente* vente;
    struct VenteElement *next;
} VenteElement;


Vente * createvente (int code,LignecmdElement *l,char *d){
	Vente *v;
	v=(Vente*)malloc(sizeof(Vente));
	v->code=code;
	v->lcmd=l;
	v->date_c=(char*)malloc(strlen(d)*sizeof(char));
	v->total=0;
	return v;
}

void displayvente(Vente *c){
	if(c!=NULL) 
	{
		printf("code: %d\n",c->code);
	}
}

VenteElement *trouverFinvente(VenteElement *P)
{
    VenteElement *ptr;

    for(ptr=P; ptr->next!=NULL; ptr=ptr->next);

    return ptr;
}


void addventeToList(VenteElement **List , Vente *cl){
	
	if(*List==NULL){

		*List=(VenteElement*)malloc(sizeof(VenteElement));
		(*List)->vente=cl;
		(*List)->next=NULL;
		
	}
	else{

		VenteElement *new;
		new=(VenteElement*)malloc(sizeof(VenteElement));
		new->vente=cl;
		new->next=NULL;
		(trouverFinvente(*List))->next=new;
	
	}
	
}

void saveventetodb(Vente *l){
	v_out=fopen("ventes.txt","a");
	fprintf(v_out,"%d %d %s %f\n",l->code,l->c->code,"bonjour",l->total);
	fflush(v_out);
	fclose(v_out);
	savelcmdtodb(l->lcmd);
}

void diplayOnevente(Vente **v){
	printf("\n----------------------\nCode : %d\nClient : %s %s\n",(*v)->code,(*v)->c->nom,(*v)->c->prenom);
	diplaycmdList((*v)->lcmd);
	printf("----------------------\n");
	printf("TOTAL::%.2f\n",(*v)->total);
	printf("----------------------\n");
}

//void diplayproductList(VenteElement * L){
//	while(L!=NULL){
//		diplayOnevente(&L->vente); 
//		L=L->next;
//	}
//}

void getAllventefromdb(VenteElement **v){
	v_in=fopen("ventes.txt","r");
	Vente *p;
	LignecmdElement *lcmd;
	int y;
	p=(Vente*)malloc(sizeof(Vente));
	p->c=(Client*)malloc(sizeof(Client));
	p->date_c=(char*)malloc(12*sizeof(char));
	lcmd=(LignecmdElement*)malloc(sizeof(LignecmdElement));
	while (fscanf(v_in,"%d %d %s %f\n",&p->code,&p->c->code,p->date_c,&p->total)!=EOF){
		p->c=getclient(p->c->code);
		p->lcmd=getlcmdwithvente(p->code);
		addventeToList(v,p);
		p=(Vente*)malloc(sizeof(Vente));
	p->c=(Client*)malloc(sizeof(Client));
	p->date_c=(char*)malloc(12*sizeof(char));
//		printf("%d \n",p->code);
	}
	fflush(v_in);
	fclose(v_in);
}

Vente* getvente(int c){
	VenteElement *o;
	o=NULL;
	getAllventefromdb(&o);
	
	while(o!=NULL){
		if(o->vente->code==c){
			return o->vente;
		}
		o=o->next;
	}
	return NULL;
}



