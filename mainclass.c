#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Paiment.c"

void gestionproduit(){
	int choix,nb;
	char *des;
	Product *p1;
	ProductElement *l=NULL;
	menu:	printf("1-Ajouter un produit\n2-liste des produits\n0- quitter\n");
	scanf("%d",&choix);
	switch(choix){
		case 1:{
			p1=(Product*)malloc(sizeof(Product));
	repeat:	printf("donnez le code de produit  \n");
			scanf("%d",&p1->code);
			printf("donnez la designation de produit \n ");
			scanf("%s",des);
			p1->designation=(char*)malloc(strlen(des)*sizeof(char));
			strcpy(p1->designation,des);
			printf("donnez le prix de produit \n ");
			scanf("%f",&p1->price);
			printf("\n%d\n",p1->code);
			if(getproduct(p1->code)!=NULL){
				printf("le produit existe déja\n");
				goto repeat;
			}else{
				printf("tapez 1 pour enregistrer dans le fichier \n");
				scanf("%d",&nb);
				if(nb==1){
					saveproducttodb(p1);
				printf("le produit est bien enregistrer\n");
				}
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
	int choix,nb;
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
				printf("tapez 1 pour enregistrer dans le fichier \n");
				scanf("%d",&nb);
				if(nb==1){
				saveclienttodb(c1);
				printf("le client est bien enregistrer\n");
			}
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
	int nbr,count,i,n;
			char *dat;
			Vente *v=NULL;
			VenteElement *vl=NULL;
			ClientElement *l=NULL;
			ProductElement *p=NULL;
			LignecmdElement *ll=NULL;
			LignecmdElement *lll=NULL;
			Lignecmd *lcmd=NULL;
			Payment *pa;
			PaymentElement *pl=NULL;
	menu:	printf("1-Passer une commande\n2-modifier la commande \n3-supprimer une commande\n4-chercher une commandes\n5-enregistrer dans la base de donnée\n0- quitter\n");
	scanf("%d",&choix);
	switch(choix){
		case 1:{
			
			v=(Vente*)malloc(sizeof(Vente));
	pe:		printf("donnez le code de vente \n");
			scanf("%d",&nbr);
			if(getvente(nbr)!=NULL){
				printf("Le code que vus avez saisie existe déja !! \n");
				goto pe;
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
				l=NULL;
				printf("le client que vous avez saisie n'existe pas \n");
				goto again;
			}
			v->c=getclient(nbr);
			printf("\nDonnez le nombre des produits\n");
			scanf("%d",&count);
			for(i=0;i<count;i++){
			lcmd=(Lignecmd*)malloc(sizeof(Lignecmd));
			printf("-------------------------\n");
			getproductfromdb(&p);
			diplayproductList(p);
			printf("-------------------------\n");
again2:			printf("donnez le code de produit %d ::\n",i+1);
			
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
			}
			addcmdToList(&lll,lcmd);
//			diplaycmdList(&lll);
			printf("ok\n");
			lcmd=NULL;
			p=NULL;
			}
			//diplaycmdList(&lll);
			//diplaycmdList(&ll);
			v->total=gettotal(lll);
			v->lcmd=lll;
			
			printf("Donnez la méthode de paiment\n1-espece\n2-cheque\n");
			scanf("%d",&nbr);
			
			pa=(Payment*)malloc(sizeof(Payment));
			if(nbr==1){
					pa->type=(char*)malloc(sizeof(char)*3);
					strcpy(pa->type,"es");
			}
			else{
				pa->type=(char*)malloc(sizeof(char)*3);
					strcpy(pa->type,"ch");
			}
			
			printf("donnez le code de paiment \n");
			scanf("%d",&nbr);
//			if(getpaiment(nbr)==NULL){
//				printf("le code de paiment existe déja \n");
//					}
			pa->paymentCode=nbr;
			pa->amount=v->total;
			pa->v=v;
			addventeToList(&vl,v);
			addpaimentToList(&pl,pa);
			printf("La commande est bien enregistre si vous voulez voir les details taper sur 1 sinon 0 \n");
			scanf("%d",&nbr);
			if(nbr==1){
				diplayOnevente(&v);
				displayonepaiment(pa);
			}
		}goto menu;
		case 2:{
			Vente *mv=NULL;
n:			printf("donnez le code de vente \n");
			scanf("%d",&nbr);
			mv=getvfromlist(&vl,nbr);
			if(mv==NULL) {
				printf("le vente n'existe pas\n");
				goto n;
			}else{
				diplayOnevente(&mv);
	tr:			printf("1-modifier quantite de produit\n2-ajouter un produit\n3-supprimer un produit\n4-affichier vente\n");
				scanf("%d",&nbr);
				switch(nbr){
					case 1:{
						int cp;
						printf("donner le code de produit");
						scanf("%d",&cp);
						LignecmdElement *opp=NULL;
						opp=mv->lcmd;
						if(isexistinv(&opp,cp)!=NULL){
							printf("donnez la nouvelle quantite\n");
							int iu;
							scanf("%d",&iu);
							
							isexistinv(&opp,cp)->qte=iu;
							printf("---------1\n");
							isexistinv(&opp,cp)->stotal=iu*isexistinv(&opp,cp)->p->price;
							printf("---------2\n");
							mv->total=gettotal(opp);
							printf("---------3\n");
							getpaimentbyv(&pl,mv->code)->amount=mv->total;
							printf("---------4\n");
							diplayOnevente(&mv);
						}else{
							printf("none\n");
						}
						goto tr;
					}
					case 2:{
						int y;
		oo:				printf("donner le code de produit");
						scanf("%d",&y);
						LignecmdElement *opp=NULL;
						opp=mv->lcmd;
						if(isexistinv(&opp,y)==NULL){
						Lignecmd *cm=(Lignecmd*)malloc(sizeof(Lignecmd));
						cm->codeV=mv->code;
						cm->p=getproduct(y);
						printf("%p\n",cm->p);
						printf("donner la quantite\n");
						scanf("%d",&(cm->qte));
						cm->stotal=cm->p->price*cm->qte;
						addcmdToList(&opp,cm);
						mv->lcmd=opp;
						mv->total=gettotal(opp);
						Payment *yu= getpaimentbyv(&pl,mv->code);
						displayonepaiment(yu);
						yu->amount=mv->total;
						displayonepaiment(yu);
						}else{
							printf("le produit existe deja dans la commande\n");
							goto oo;
						}	
						goto tr;
					}
					case 3:{
						//delete product
						int cp;
						printf("donner le code de produit");
						scanf("%d",&cp);
						LignecmdElement *opp;
						
						opp=mv->lcmd;
						diplaycmdList(&opp);
						printf("after\n");
						if(isexistinv(&opp,cp)!=NULL){
							deletelcmdfromv(&opp,cp);
						}
						mv->lcmd=opp;
						goto tr;
					}
					case 4:{
						Payment *yu= getpaimentbyv(&pl,mv->code);
						diplayOnevente(&mv);
						displayonepaiment(pa);
						goto tr;
					}
				}
			}
				
		}goto menu;
		case 4:{

			int nbr;
	f:		printf("entrer le code de vente \n");
			scanf("%d",&nbr);
			if(getvente(nbr)==NULL){
				printf("la vente que vous avez saisie n'existe pas\n");
				goto f;
			}
			Payment *py=getpaimentbyvente(nbr);
			diplayOnevente(&py->v);
			displayonepaiment(py);
		}goto menu;
		
		case 3:{
			int m;
			Vente *mv=NULL;
nm:			printf("donnez le code de vente \n");
			scanf("%d",&m);
			mv=getvfromlist(&vl,m);
			if(mv==NULL) {
				printf("le vente n'existe pas\n");
				goto nm;
			}else{
			deletevfromvl(&vl,m);
		}
		printf("la commande est supprimer\n");
		goto menu;
	}
		
		case 5:{
			savepaimenlttodb(&pl);
			saveventeltodb(&vl);
			goto menu;
		}
		
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
