#include <stdlib.h>
#include <stdio.h>
#include "sdd.h"


int boolverif_depl_pion(eCOULEUR c, int dx, int dy, int ax, int ay);
int boolverif_depl_dame(int dx, int dy, int ax, int ay);

DAMIER* init_damier(){
	/**
	 * Initialisation d'un DAMIER.
	 * La couleur du tour est initialisée à BLANC
	 * Le nombre de pièces blanches ou noires est initialisé à 20
	 * L'initialisation de la liste de pions est expliquée plus bas
	 * La fonction renvoie un DAMIER* ou NULL en cas d'échec
	 **/
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
	/**
	 * Initialisation de la liste de pions.
	 * Les pièces ont positionnée une case sur deux sur chaque ligne du damier alternativement, changement à chaque ligne, sur les cases d'abscisse paire ou impaire.
	 * Les pièces de la liste de [0 à 19] sont initialisées avec la couleur BLANC, de [20 à 39] avec la couleur NOIR
	 * Chaque pièce est initialisée avec le type T_PION
	 * La dernière case de la liste est initialisé à NULL
	 **/
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
	/**
	 * Suppression d'un damier
	 * La fonction permet de supprimer un DAMIER ainsi que toutes ses composantes
	 * La fonction supprime chaque pièce existante de la liste de pions du DAMIER avant de supprimer la liste puis le damier
	 * La fonction renvoie le nombre de pièces supprimées
	 **/
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
	/**
	 * Passage au tour suivant
	 * La fonction alterne la couleur du tour à chaque appel
	 **/
	damier->c_tour=1-damier->c_tour;
}

PION* rech_pion_c(DAMIER *damier, int x, int y){
	/**
	 * Recherche de pion par ses coordonnées
	 * La fonction parcours la liste de pions d'un DAMIER jusqu'à trouver une pièce de coordonnées(x,y) ou la fin de la liste
	 * La fonction renvoie un PION* ou NULL en cas d'échec
	 **/
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

int prendre(DAMIER *damier, int dx, int dy, int ax, int ay){
	
}

int deplacer(DAMIER *damier, int dx, int dy, int ax, int ay){
	/**
	 * Déplacement d'une pièce
	 * Déplace une pièce d'un DAMIER de (dx,dy) à (ax,ay) si toute les conditions sont respectée
	 * 	la pièce éxiste en (dx,dy)
	 * 	la case (ax,ay) n'est pas emcombrée
	 * 	la couleur de la pièce correspond à la couleur du tour actuel
	 * 	le type de déplacement est correct pour ce type de pièce (T_PION || T_DAME)
	 * renvoie 0 en cas de réussite
	 *         1 si la pièce n'éxsite pas en (dx,dy)
	 *         2 si la case (ax,ay) est encombrée
	 *         3 si la couleur de la pièce ne correspond pas au tour actuel
	 *         4 si le type de déplacement n'est pas autorisé pour se type de pièce
	 **/

	PION *p = rech_pion_c(damier,dx,dy);
	if(p==NULL) return 1;
	if(rech_pion_c(damier,ax,ay)!=NULL) return 2;
	if(p->couleur!=damier->c_tour) return 3;
	if(p->type==T_DAME){
		if(boolverif_depl_dame(dx,dy,ax,ay)) return 4;
	}else{
		if(boolverif_depl_pion(damier->c_tour,dx,dy,ax,ay)) return 4;
	}
	p->position.x=ax;
	p->position.y=ay;

	return 0;
}

int boolverif_depl_pion(eCOULEUR c, int dx, int dy, int ax, int ay){
	/**
	 * Vérifie si une pièce de type T_PION peut ce déplacer de (dx,dy) à (ax,ay)
	 * retourne 1 si vrai, 0 sinon
	 **/
	if( (((dx-ax)*(dx-ax))!=1 || ((dy-ay)*(dy-ay))!=1)) return 0;
	if(ax<0 || ay<0 || ax>=COTE_DAMIER || ay>=COTE_DAMIER) return 0;
	if((c==BLANC && dy-ay>0) || (c==NOIR && dy-ay<0)) return 6;

	return 1;
}


int boolverif_depl_dame(int dx, int dy, int ax, int ay){
	/**
	 * Vérifie si une pièce de type T_DAME peut ce déplacer de (dx,dy) à (ax,ay)
	 * retourne 1 si vrai, 0 sinon
	 **/
	if( (((dx-ax)*(dx-ax)) == ((dy-ay)*(dy-ay)))) return 0;
	if(ax<0 || ay<0 || ax>=COTE_DAMIER || ay>=COTE_DAMIER) return 0;

	return 1;
}

void aff_damier(DAMIER damier){
	/**
	 * Affiche un cadrillage d'un damier dans son état actuel, avec les pièces positionnées et les coordonnées, dans la console
	 **/
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









