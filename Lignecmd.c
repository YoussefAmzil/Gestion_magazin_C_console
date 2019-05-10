#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "client.c"
#include "produit.c"

FILE *l_out;
FILE *l_in;


typedef struct Lignecmd{
	int codeV;//code vente
	Product *p;
	int qte;
	float stotal;
}Lignecmd;

typedef struct LignecmdElement{
	Lignecmd *lcmd;
	struct LignecmdElement *next;
}LignecmdElement;




Lignecmd * createlcmd (int qte,int codev,Product *p){
	Lignecmd *c1;
	c1=(Lignecmd*)malloc(sizeof(Lignecmd));
	c1->qte=qte;
	c1->p=p;
	c1->stotal=qte*p->price;
	c1->codeV=codev;
	return c1;
}

LignecmdElement *trouverFin(LignecmdElement *L)
{
    LignecmdElement *ptr;

    for(ptr=L; ptr->next!=NULL; ptr=ptr->next);

    return ptr;
}

void addcmdToList(LignecmdElement **List , Lignecmd *cmd){
	
	if(*List==NULL){

		*List=(LignecmdElement*)malloc(sizeof(LignecmdElement));
		(*List)->lcmd=cmd;
		(*List)->next=NULL;
		
	}
	else{
		LignecmdElement *new;
		new=(LignecmdElement*)malloc(sizeof(LignecmdElement));
		new->lcmd=cmd;
		new->next=NULL;
		(trouverFin(*List))->next=new;
	
	}
}

void displaycmd(Lignecmd *c){
	if(c!=NULL) 
	{
		displayproduit(c->p);
		printf("quantite: %d \n sous total: %2.f \n\n",c->qte,c->stotal);
	}
	else printf("la ligne commande est null ou n'existe pas");
}

void diplaycmdList(LignecmdElement * L){

	while(L!=NULL){
		printf("----------------------\n");
		displaycmd(L->lcmd);
		printf("----------------------\n");
		L=L->next;
	}
}

float gettotal(LignecmdElement *L){
	float t=0;
	while(L!=NULL){
		t+=L->lcmd->stotal;
		L=L->next;
	}
	return t;
}

void savelcmdtodb(LignecmdElement *l){
	l_out=fopen("lcmds.txt","a");
	while(l!=NULL){
		fprintf(l_out,"%d %d %d %f\n",(l)->lcmd->codeV,(l)->lcmd->p->code,(l)->lcmd->qte,(l)->lcmd->stotal);
		(l)=(l)->next;
	}
	fflush(l_out);
	fclose(l_out);
}

void getlcmdfromdb(LignecmdElement **pe){
	l_in=fopen("lcmds.txt","r");
	Lignecmd *p;
	p=(Lignecmd*)malloc(sizeof(Lignecmd));
	p->p=(Product*)malloc(sizeof(Product));
	while (fscanf(l_in,"%d %d %d %f\n",&p->codeV,&p->p->code,&p->qte,&p->stotal)!=EOF){
		p->p=getproduct(p->p->code);
		addcmdToList(pe,p);
		p=(Lignecmd*)malloc(sizeof(Lignecmd));
		p->p=(Product*)malloc(sizeof(Product));
	}
	fflush(l_in);
	fclose(l_in);
}

Lignecmd *isexistinlcmd(LignecmdElement **l,int code){
	while(*l!=NULL){
		if((*l)->lcmd->p->code==code) return (*l)->lcmd;
		*l=(*l)->next;
	}
	return NULL;
}

LignecmdElement *getlcmdwithvente(int c){
	LignecmdElement *pe=NULL;
	int tmpc;
	l_in=fopen("lcmds.txt","r");
	Lignecmd *p;
	p=(Lignecmd*)malloc(sizeof(Lignecmd));
	while (fscanf(l_in,"%d %d %d %f\n",&p->codeV,&tmpc,&p->qte,&p->stotal)!=EOF ){
			
		if(p->codeV==c){
			p->p=getproduct(tmpc);
			addcmdToList(&pe,p);
		}
		p=(Lignecmd*)malloc(sizeof(Lignecmd));
	}
	return pe;
}


//int main(){
//	Lignecmd *l1,*l2;
//	Product *p,*p2;
//	LignecmdElement *L=NULL;
//	p=createproduit(123,"chips",28.6);
//	p2=createproduit(1,"danone",2.6);
//	l1=createlcmd(2,3,p);
//	l2=createlcmd(1,9,p2);
//	savelcmdtodb(l1);
//	getlcmdfromdb(&L);
//	diplaycmdList(L);
//	return 0;
//}
