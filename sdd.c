#include <stdlib.h>
#include <stdio.h>
#include "sdd.h"

DAMIER* init_damier(){
	int i = 0;
	DAMIER *damier = (DAMIER*)malloc(sizeof(DAMIER));
	if (damier==NULL) return NULL;

	damier->c_tour=BLANC;
	damier->nb_pion_b=NB_PION_B;
	damier->nb_pion_n=NB_PION_N;
	damier->liste_pion=(PION**)malloc(sizeof(PION*)*(NB_PION+1));
	if(damier->liste_pion==NULL){
		free(damier);
		return NULL;
	}
	for(i=0;i<NB_PION_B;i++){
	//Positionnement des pions blancs de [0,19]
		damier->liste_pion[i]=(PION*)malloc(sizeof(PION));
		if(damier->liste_pion[i]==NULL){
			free_damier(damier);
			return NULL;
		}
		if(i/(COTE_DAMIER/2)%2==0){
			damier->liste_pion[i]->position.x=i%(COTE_DAMIER/2)*2;
		}else{
			damier->liste_pion[i]->position.x=i%(COTE_DAMIER/2)*2+1;
		}
		damier->liste_pion[i]->position.y=i/(COTE_DAMIER/2);
		damier->liste_pion[i]->couleur=BLANC;
		damier->liste_pion[i]->type=T_PION;
	//Positionnement des pions noirs de [20,39]
		damier->liste_pion[i+NB_PION_N]=(PION*)malloc(sizeof(PION));
		if(damier->liste_pion[i+NB_PION_N]==NULL){
			free_damier(damier);
			return NULL;
		}
		if(i/(COTE_DAMIER/2)%2==0){
			damier->liste_pion[i+NB_PION_N]->position.x=COTE_DAMIER-i%(COTE_DAMIER/2)*2-1;
		}else{
			damier->liste_pion[i+NB_PION_N]->position.x=COTE_DAMIER-i%(COTE_DAMIER/2)*2-2;
		}
		damier->liste_pion[i+NB_PION_N]->position.y=COTE_DAMIER-i/(COTE_DAMIER/2)-1;
		damier->liste_pion[i+NB_PION_N]->couleur=NOIR;
		damier->liste_pion[i]->type=T_PION;
	}
	damier->liste_pion[NB_PION]=NULL;

	return damier;
}


int free_damier(DAMIER *damier){
	int i=0, nb_free=0;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			free(damier->liste_pion[i]);
			nb_free++;
		}
		i++;
	}
	free(damier->liste_pion);
	free(damier);

	return nb_free;
}

void tour_suivant(DAMIER *damier){
	damier->c_tour=1-damier->c_tour;
}

PION* rech_pion_c(DAMIER *damier, int x, int y){
	int i=0;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			if(damier->liste_pion[i]->position.x==x && damier->liste_pion[i]->position.y==y){
				return damier->liste_pion[i];
			}
		}
		i++;
	}

	return NULL;
}

int deplacement(DAMIER *damier, int dx, int dy, int ax, int ay){
	if( (((dx-ax)*(dx-ax))!=1 || ((dy-ay)*(dy-ay))!=1)) return 1;
	if(ax<0 || ay<0 || ax>=COTE_DAMIER || ay>=COTE_DAMIER) return 2;
	PION *p = rech_pion_c(damier,dx,dy);
	if(p==NULL) return 3;
	if(rech_pion_c(damier,ax,ay)!=NULL) return 4;
	if(p->couleur!=damier->c_tour) return 5;
	if((damier->c_tour==BLANC && dy-ay>0) || (damier->c_tour==NOIR && dy-ay<0)) return 6;
	p->position.x=ax;
	p->position.y=ay;

	return 0;
}

void aff_damier(DAMIER damier){
	int i=0,x=0,a=7,b=0;
	PION *p;
	char buf[100];
	char buf2[10];
	char format[] = "echo -n \"\033[3%d;4%dm%s\033[0m\"";
	int gray = 0, red = 1, green = 2, yellow = 3, blue = 4, purple = 5, light_blue = 6, white = 7, black = 8 ;

	for(i=0;i<100;i++){
		if(i%10==0){
			printf("\n");
			sprintf(buf2,"%d ",9-i/10);
			sprintf(buf,format,green,black,buf2);
			system(buf);
			x=1-x;
		}
		p=rech_pion_c(&damier,i%10,COTE_DAMIER-i/10-1);

		if(p!=NULL){
			if(p->couleur==BLANC){
				sprintf(buf,format,white,a*x+b,"0 ");
				system(buf);
			}else{
				sprintf(buf,format,green,a*x+b,"O ");
				system(buf);
			}
		}else{
				sprintf(buf,format,gray,a*x+b,"  ");
				system(buf);
		}
		x=1-x;
	}
	printf("\n");
	sprintf(buf,format,white,black,"  0 1 2 3 4 5 6 7 8 9");
	system(buf);
	printf("\n\n");

}









