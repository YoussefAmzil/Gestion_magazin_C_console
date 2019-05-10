#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "vente.c"

void gestionproduit(){
	int choix;
	char *des;
	Product *p1;
	ProductElement *l=NULL;
	menu:	printf("1-Ajouter un produit\n2-liste des produits\n0- quitter\n");
	scanf("%d",&choix);
	switch(choix){
		case 1:{
			p1=(Product*)malloc(sizeof(Product));
	repeat:	printf("donnez le code de produit  ");
			scanf("%d",&p1->code);
			printf("donnez la designation de produit  ");
			scanf("%s",des);
			p1->designation=(char*)malloc(strlen(des)*sizeof(char));
			strcpy(p1->designation,des);
			printf("donnez le prix de produit  ");
			scanf("%f",&p1->price);
			printf("\n%d\n",p1->code);
			if(getproduct(p1->code)!=NULL){
				printf("le produit existe déja\n");
				goto repeat;
			}else{
				saveproducttodb(p1);
				printf("le produit est bien enregistrer\n");
				goto menu;
			}
		}break;
		case 2:{
			getproductfromdb(&l);
			diplayproductList(l);
			printf("\n");
			goto menu;
		};break;
		
	}
}

void gestionclient(){
	int choix;
	char *des;
	Client *c1;
	ClientElement *l;
	l=NULL;
	menu:	printf("1-Ajouter un client\n2-liste des clients\n0- quitter\n");
	scanf("%d",&choix);
	switch(choix){
		case 1:{
			c1=(Client*)malloc(sizeof(Client));
	repeat:	printf("donnez le code de client  ");
			scanf("%d",&c1->code);
			printf("donnez le nom de client  ");
			scanf("%s",des);
			c1->nom=(char*)malloc(strlen(des)*sizeof(char));
			strcpy(c1->nom,des);
			printf("donnez le prenom de client  ");
			scanf("%s",des);
			c1->prenom=(char*)malloc(strlen(des)*sizeof(char));
			strcpy(c1->prenom,des);
			printf("donnez le téléphone de client  ");
			scanf("%s",des);
			c1->tele =(char*)malloc(strlen(des)*sizeof(char));
			strcpy(c1->tele,des);
			if(getclient(c1->code)){
				printf("le client existe déja\n");
				goto repeat;
			}else{
				saveclienttodb(c1);
				printf("le client est bien enregistrer\n");
				goto menu;
			}
		}break;
		case 2:{
			getclientfromdb(&l);
			diplayClientsList(l);
			printf("\n");
			goto menu;
		};break;
		
	}
}

void gestionvente(){
	int choix;
	char *des;
	Client *c1;
	ClientElement *l;
	l=NULL;
	menu:	printf("1-Passer une commande\n2-chercher une commandes\n0- quitter\n");
	scanf("%d",&choix);
	switch(choix){
		case 1:{
			int nbr,count,i,n;
			char *dat;
			Vente *v=NULL;
			ClientElement *l=NULL;
			ProductElement *p=NULL;
			LignecmdElement *ll=NULL;
			Lignecmd *lcmd=NULL;
			v=(Vente*)malloc(sizeof(Vente));
	p:		printf("donnez le code de vente \n");
			scanf("%d",&nbr);
			if(getvente(nbr)!=NULL){
				printf("Le code que vus avez saisie existe déja !! \n");
				goto p;
			}
			v->code=nbr;
			printf("donnez la date sous forme dd/mm/yyyy \n");
			scanf("%s",dat);
			v->date_c=(char*)malloc(sizeof(char)*12);
			strcpy(v->date_c,dat);
		again:	printf("Choisir le client \n----------------------\n");
			getclientfromdb(&l);
			diplayClientsList(l);
			printf("----------------------\n");
			scanf("%d",&nbr);
			if(getclient(nbr)==NULL){
				printf("le client que vous avez saisie n'existe pas \n");
				goto again;
			}
			v->c=getclient(nbr);
			printf("\nDonnez le nombre des produits\n");
			scanf("%d",&count);
			for(i=0;i<count;i++){
			lcmd=(Lignecmd*)malloc(sizeof(Lignecmd));
again2:			printf("donnez le code de produit %d ::\n",i+1);
			printf("-------------------------\n");
			getproductfromdb(&p);
			diplayproductList(p);
			printf("-------------------------\n");
			scanf("%d",&nbr);
			
			if(getproduct(nbr)==NULL){
				printf("le produit que vous avez saisie n'existe pas \n");
				goto again2;
			}
			
			lcmd->codeV=v->code;
			if(isexistinlcmd(&ll,nbr)!=NULL){
			printf("donnez la quantite de produit %d ::\n",i+1);
			scanf("%d",&n);
			printf("first quantity :: %d\n",isexistinlcmd(&ll,nbr)->qte);
				(isexistinlcmd(&ll,nbr))->qte+=n;
			printf("final quantity :: %d\n",isexistinlcmd(&ll,nbr)->qte);
				(isexistinlcmd(&ll,nbr))->stotal=((isexistinlcmd(&ll,nbr))->qte)*((isexistinlcmd(&ll,nbr))->p->price);
			printf("current total : %f\n",(isexistinlcmd(&ll,nbr))->stotal);
			}
			else{
			lcmd->p=getproduct(nbr);
			printf("donnez la quantite de produit %d ::\n",i+1);
			scanf("%d",&nbr);
			lcmd->qte=nbr;	
			lcmd->stotal=lcmd->qte*lcmd->p->price;
			addcmdToList(&ll,lcmd);
			}
			printf("ok\n");
			lcmd=NULL;
			}
			v->total=gettotal(ll);
			v->lcmd=ll;
			saveventetodb(v);
			printf("La commande est bien enregistre si vous voulez la voir taper sur 1 sinon 0 \n");
			scanf("%d",&nbr);
			if(nbr==1){
				diplayOnevente(&v);
			}
		}break;
		case 2:{

			int nbr;
	f:		printf("entrer le code de vente \n");
			scanf("%d",&nbr);
			if(getvente(nbr)==NULL){
				printf("la vente que vous avez saisie n'existe pas\n");
				goto f;
			}
			Vente *re=getvente(nbr);
			printf("exist\n");
			diplayOnevente(&re);
		
		};break;
		
	}
}

int main(){
	int choix;
	Vente *v=NULL;
	LignecmdElement *lcmd=NULL;
	ProductElement *pe=NULL;

	do{
	printf("1-Gestion des produits\n2-Gestion des clients\n3-Gestion des commandes\n0- quitter\n");
	printf("Donnez votre choix :\n");
	scanf("%d",&choix);
		switch(choix){
		case 1: gestionproduit(); break;
		case 2: gestionclient() ;break;
		case 3:	gestionvente() ;break;
	}
	}while(choix!=0);
	
	
	return 0;
}
