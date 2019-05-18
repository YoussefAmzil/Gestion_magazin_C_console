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
		new->next=(*List);
		(*List)=new;
		//(trouverFin(*List))->next=new;
	
	}
}

void displaycmd(Lignecmd *c){
	if(c!=NULL) 
	{
		displayproduit(c->p);
		printf("quantite: %d \nsous total: %2.f \n\n",c->qte,c->stotal);
	}
	else printf("la ligne commande est null ou n'existe pas");
}

void diplaycmdList(LignecmdElement **L){
	LignecmdElement *ptr=*L;
	if((*L)==NULL) printf("NULL\n");
	while(ptr!=NULL){
		printf("----------------------\n");
		displaycmd(ptr->lcmd);
		//printf("%p\n",ptr->lcmd);
		printf("----------------------\n");
		ptr=ptr->next;
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

void savelcmdtodb(Lignecmd *l){
	l_out=fopen("lcmds.txt","a");
		fprintf(l_out,"%d %d %d %f\n",(l)->codeV,(l)->p->code,(l)->qte,(l)->stotal);
	fflush(l_out);
	fclose(l_out);
}

void savelcmdltodb(LignecmdElement **l){
	LignecmdElement *ptr=*l;
	while(ptr){
		savelcmdtodb(ptr->lcmd);
		ptr=ptr->next;
	}
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

LignecmdElement *getlcmdinv(LignecmdElement **l,int cv){
	LignecmdElement *n=NULL;
	while((*l)!=NULL){
		if((*l)->lcmd->codeV==cv) addcmdToList(&n,(*l)->lcmd);
		(*l)=(*l)->next;
	}
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

Lignecmd * isexistinv(LignecmdElement **p,int cp){
	while((*p)!=NULL){
		if((*p)->lcmd->p->code==cp){
			return (*p)->lcmd;
		}
		(*p)=(*p)->next;
	}
	return NULL;
}


void deletelcmdfromv(LignecmdElement **l,int c){
	LignecmdElement *ptr,*tmp;
	ptr=*l;
	if(ptr->lcmd->p->code!=c){
		while(ptr->next!=NULL){
			if(ptr->next->lcmd->p->code==c){
				tmp=ptr->next->next;
				free(ptr->next);
				ptr->next=tmp;
				break;
			}
			ptr=ptr->next;
		}
	}
	else if(ptr->lcmd->p->code==c){
		*l=(*l)->next;
	}	
}

//int main(){
//	Lignecmd *l1,*l2,*l3;
//	Product *p,*p2;
//	LignecmdElement *L=NULL;
//	p=createproduit(123,"chips",28.6);
//	p2=createproduit(1,"danone",2.6);
//	l1=createlcmd(2,3,p);
//	l2=createlcmd(1,9,p2);
//	l3=createlcmd(1,3,p);
//	addcmdToList(&L,l1);
//	addcmdToList(&L,l2);
//	addcmdToList(&L,l3);
//	diplaycmdList(&L);
//	return 0;
//}
