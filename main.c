#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdd.h"

//test
int main(){
	DAMIER *damier;
	PION *p;
	damier = init_damier();
	if(damier==NULL){
		printf("Initialisation du damier échouée\n");
		return 0;
	}
	int i=0, x, y, a, b, erreur;
	
	for(i=0;i<NB_PION;i++){
		printf("Pion n°%d : ",i);
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
	
		printf("(%d,%d)",damier->liste_pion[i]->position.x, damier->liste_pion[i]->position.y);
		p = rech_pion_c(damier,damier->liste_pion[i]->position.x,damier->liste_pion[i]->position.y);
		if(p==NULL){
			printf("Pion non trouvé\n");
		}else{
			printf("Pion trouvé\n");
		}
	}/*
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
		printf("Déplacement d'un pion : ");
		char str2[11];
		sscanf(fgets(str2,10,stdin),"%d,%d %d,%d", &x, &y, &a, &b); 
		erreur = deplacer(damier,x,y,a,b);
		printf("de (%d,%d) en (%d,%d)\n",x,y,a,b);
		while(erreur){
			switch(erreur){
				case 1: printf("Pièce inexistante\n");
					break;
				case 2: printf("Case d'arrivée encombrée\n");
					break;
				case 3: printf("Pièce de la mauvaise couleur\n");
					break;
				case 4: printf("Déplacement interdit\n");
					break;
				default : printf("Erreur imprévue\n");
					  break;
			}
			printf("Déplacement d'un pion : ");
			sscanf(fgets(str2,10,stdin),"%d,%d %d,%d", &x, &y, &a, &b); 
			erreur = deplacer(damier,x,y,a,b);
			printf("de (%d,%d) en (%d,%d)\n",x,y,a,b);
		}
		printf("Pion délacer en (%d,%d)\n",a,b);
		
		aff_damier(*damier);
		tour_suivant(damier);
	}
	free(damier);
	free(p);
	return 0;
}
