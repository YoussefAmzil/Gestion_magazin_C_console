#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *pr_out;
FILE *pr_in;
 
typedef struct Product		
{
    int code ; 
    char* designation;
    float price;
} Product;
typedef struct ProductElement
{
    Product* product;
    struct ProductElement *next;
} ProductElement;


Product * createproduit (int c,char *n,double t){
	Product *p;
	p=(Product*)malloc(sizeof(Product));
	p->code=c;
	p->price=t;

	p->designation=(char*)malloc(strlen(n)*sizeof(char));
	strcpy(p->designation,n);
	return p;
}

void displayproduit(Product *c){
	if(c!=NULL) 
	{
		printf("code produit: %d\ndesignation: %s\nprix: %.2f \n\n",(c)->code,c->designation,c->price);
	}
	else printf("le client est null ou n'existe pas");
}

ProductElement *trouverFinproduit(ProductElement *P)
{
    ProductElement *ptr;
    for(ptr=P; ptr->next!=NULL; ptr=ptr->next);

    return ptr;
}

void addProductToList(ProductElement **List , Product *cl){
	
	if(*List==NULL){

		*List=(ProductElement*)malloc(sizeof(ProductElement));
		(*List)->product=cl;
		(*List)->next=NULL;
		
	}
	else{
		ProductElement *new;
		new=(ProductElement*)malloc(sizeof(ProductElement));
		new->product=cl;
		new->next=NULL;
		(trouverFinproduit(*List))->next=new;
	
	}
	
}

void diplayproductList(ProductElement * L){
	while(L!=NULL){
		displayproduit(L->product);
		L=L->next;
	}
}

void getproductfromdb(ProductElement **pe){
	pr_in=fopen("produits.txt","r");
	Product *p;
	int y;
	p=(Product*)malloc(sizeof(Product));
	p->designation=(char*)malloc(30*sizeof(char));
	while (fscanf(pr_in,"%d %s %f\n",&p->code,p->designation,&p->price)!=EOF){
		addProductToList(pe,p);
		p=(Product*)malloc(sizeof(Product));
		p->designation=(char*)malloc(30*sizeof(char));
	}
	fflush(pr_in);
	fclose(pr_in);
}


Product *getproduct(int c){
	ProductElement *o;
	o=NULL;
	getproductfromdb(&o);
	if(o==NULL) return NULL;
//	printf("NOT NULL\n");
	
	while(o!=NULL){
//		printf("Not NULL File\n");
		if(o->product->code==c) {
		return o->product;	
		}				
		o=o->next;
	}
	return NULL;
}


void saveproducttodb(Product *p){
	pr_out=fopen("produits.txt","a");
	fprintf(pr_out,"%d %s %f\n",p->code,p->designation,p->price);
	fflush(pr_out);
	fclose(pr_out);
}


//int main()
//{
//    ProductElement *pe=NULL;
//	Product *p1,*p2;
//	p1=createproduit(123,"chipps",28.2);
//	p2=createproduit(1,"danoune",2.4);
////	saveproducttodb(p1);
////	saveproducttodb(p2);
//	getproductfromdb(&pe);
//	diplayproductList(pe);
//	//displayproduit(p1);
////	addProductToList(&pe,p1);
////	addProductToList(&pe,p2);
////	diplayproductList(pe);
//    return 0;
//}
