#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdd.h"

void listePion(DAMIER *damier){
	int i;
	for(i=0;i<NB_PION;i++){
		printf("Pion n°%d : ",i);
		if(damier->liste_pion[i]==NULL){
			printf("NULL\n");
		}else{
			if(damier->liste_pion[i]->type==T_PION){
				printf("PION ");
			}else{
				printf("DAME ");
			}
			if(damier->liste_pion[i]->couleur==BLANC){
				printf("BLANC ");
			}else{
				printf("NOIR ");
			}
	
			printf("(%d,%d)",damier->liste_pion[i]->pos.x, damier->liste_pion[i]->pos.y);
			PION *p = rech_pion_c(damier,damier->liste_pion[i]->pos.x,damier->liste_pion[i]->pos.y);
			if(p==NULL){
				printf("Pion non trouvé\n");
			}else{
				printf("Pion trouvé\n");
			}
		}
	}
}

int main(){
	DAMIER *damier;
	PION *p;
	damier = init_damier();
	if(damier==NULL){
		printf("Initialisation du damier échouée\n");
		return 0;
	}
	int i=0, x, y, a, b, k, l, erreur;
	listePion(damier);	
	/*
	printf("Recherche d'un pion : ");
	char str[11];
	sscanf(fgets(str,10,stdin),"%d,%d",&x ,&y); 
	p = rech_pion_c(damier,x,y);
	if(p==NULL){
		printf("Pion non trouvé\n");
	}else{
		printf("Pion trouvé\n");
	}*/
	aff_damier(*damier);

	while(1){
		printf("Couleur : %d\n", damier->c_tour);
		printf("Action sur un pion : ");
		char typeDep='v';
		char str2[100];
		int nbPara;
		k=-1;l=-1;
		nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %c %d,%d %d,%d", &x, &y, &typeDep, &a, &b, &k, &l);
		while(typeDep!='d' && typeDep!='p' && typeDep!='l'|| nbPara<5){
			printf("mauvaise commande(%c%d)\n",typeDep,nbPara);
			printf("Action sur un pion : ");
			nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %c %d,%d %d,%d", &x, &y, &typeDep, &a, &b, &k, &l);
		}
		PION *p1 = rech_pion_c(damier,x,y);
		if(typeDep=='l'){
			listePion(damier);
			tour_suivant(damier);
		}else if(typeDep=='p'){
			PION *p2 = rech_pion_c(damier,a,b);
			if(k==-1 && l==-1){
				k=a+(a-x);
				l=b+(b-y);
			}
			prendre(damier,p1,p2,k,l);
			printf("Prise de (%d,%d) par (%d,%d) en (%d,%d)\n",a,b,x,y,k,l);
		}else{
			erreur = deplacer(damier,p1,a,b);
			printf("de (%d,%d) en (%d,%d)\n",x,y,a,b);
			while(erreur){
				switch(erreur){
					case 1: printf("Case vide\n");
						break;
					case 2: printf("Pièce inexistante (NULL)\n");
						break;
					case 3: printf("Pièce introuvable\n");
						break;
					case 4: printf("Mauvaise couleur\n");
						break;
					case 5: printf("Déplacement interdit\n");
						break;
					case 6: printf("Déplacement hors limite\n");
						break;
					case 7: printf("Déplacement à reculons\n");
						break;
					case 8: printf("Arrivée encombrée\n");
						break;
					default : printf("Erreur imprévue\n");
						  break;
				}
				printf("Couleur : %d\n", damier->c_tour);
				printf("Déplacement d'un pion : ");
				sscanf(fgets(str2,10,stdin),"%d,%d %c %d,%d", &x, &y, &typeDep, &a, &b);
				p1 = rech_pion_c(damier,x,y);
				erreur = deplacer(damier,p1,a,b);
				printf("de (%d,%d) en (%d,%d)\n",x,y,a,b);
			}
			printf("Pion délacer en (%d,%d)\n",a,b);
		}
		aff_damier(*damier);
		tour_suivant(damier);
	}
	free(damier);
	free(p);
	return 0;
}
