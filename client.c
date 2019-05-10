#include "stdio.h"
#include "stdlib.h"
#include "string.h"

FILE *cl_out;
FILE *cl_in;


typedef struct Client{
	int code;
	char *nom,*prenom;
	char *tele;
}Client;

typedef struct ClientElement{
	Client *client;
	struct ClientElement *next;
}ClientElement;


Client * createClient (int c,char *n,char *p,char *t){
	Client *c1;
	c1=(Client*)malloc(sizeof(Client));
	c1->code=c;
	c1->tele=t;
	c1->prenom=(char*)malloc(strlen(p)*sizeof(char));
	strcpy(c1->prenom,p);
	c1->nom=(char*)malloc(strlen(n)*sizeof(char));
	strcpy(c1->nom,n);
	c1->tele=(char*)malloc(strlen(t)*sizeof(char));
	strcpy(c1->tele,t);
	return c1;
}

void displayClient(Client *c){
	if(c!=NULL) 
	{
		printf("code client: %d\nnom : %s\nprenom: %s\ntelephone : %s\n\n",(c)->code,(c)->nom,(c)->prenom,(c)->tele);
	}
	else printf("le client est null ou n'existe pas");
}

ClientElement *trouverFinclient(ClientElement *L)
{
    ClientElement *ptr;

    for(ptr=L; ptr->next!=NULL; ptr=ptr->next);

    return ptr;
}

void addClientToList(ClientElement **List , Client *cl){
	
	if(*List==NULL){

		*List=(ClientElement*)malloc(sizeof(ClientElement));
		(*List)->client=cl;
		(*List)->next=NULL;
		
	}
	else{
		ClientElement *new;
		new=(ClientElement*)malloc(sizeof(ClientElement));
		new->client=cl;
		new->next=NULL;
		(trouverFinclient(*List))->next=new;
	
	}
	
}

void diplayClientsList(ClientElement * L){

	while(L!=NULL){
		displayClient(L->client);
		L=L->next;
	}
}

Client *getClient(ClientElement * L, int c){
	
	while(L!=NULL){
		if(L->client->code==c) break;	
				
		L=L->next;
	}
	return L->client;
}


void deleteClient (ClientElement **l, int c){
	ClientElement *ptr,*tmp;
	ptr=*l;
	if(ptr->client->code!=c){
		while(ptr->next!=NULL){
			if(ptr->next->client->code==c){
				tmp=ptr->next->next;
				free(ptr->next);
				ptr->next=tmp;
				break;
			}
			ptr=ptr->next;
		}
	}
	else if(ptr->client->code==c){
		*l=(*l)->next;
	}
}

void updateClient (ClientElement **l,int c,char *n,char *p,char * t){
	Client *c1;
	c1=getClient(*l,c);
	//c1->tele=t;
	c1->prenom=(char*)malloc(strlen(p)*sizeof(char));
	strcpy(c1->prenom,p);
	c1->nom=(char*)malloc(strlen(n)*sizeof(char));
	strcpy(c1->nom,n);
}

void saveclienttodb(Client *c){
	cl_out=fopen("clients.txt","a");
	fprintf(cl_out,"%d %s %s %s\n",(c)->code,(c)->nom,(c)->prenom,(c)->tele);
	fflush(cl_out);
	fclose(cl_out);
}

void getclientfromdb(ClientElement **pe){
	cl_in=fopen("clients.txt","r");
	Client *p;
	p=(Client*)malloc(sizeof(Client));
	p->nom=(char*)malloc(30*sizeof(char));
	p->prenom=(char*)malloc(30*sizeof(char));
	p->tele=(char*)malloc(30*sizeof(char));
	while (fscanf(cl_in,"%d %s %s %s\n",&p->code,p->nom,p->prenom,p->tele)!=EOF){
		addClientToList(pe,p);
		p=(Client*)malloc(sizeof(Client));
		p->nom=(char*)malloc(30*sizeof(char));
		p->prenom=(char*)malloc(30*sizeof(char));
		p->tele=(char*)malloc(30*sizeof(char));
	}
	fflush(cl_in);
	fclose(cl_in);
}

Client *getclient(int c){
	ClientElement *o=NULL;
	getclientfromdb(&o);
	
	while(o!=NULL){
		if(o->client->code==c) return o->client;	
		o=o->next;
	}
	return NULL;
}

//int main(){
//	Client *c,*c2,*c3;
//	c=createClient(123,"amzil","youssef","0655343");
//	c2=createClient(198,"amzil","ayoub","07636553453");
////	saveclienttodb(c);
////	displayClient(c);
//	ClientElement *List;
//	List=NULL;
//	getclientfromdb(&List);
//	diplayClientsList(List);	
//	return 0;
//}








