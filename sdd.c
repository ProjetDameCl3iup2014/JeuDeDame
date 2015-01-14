#include <stdlib.h>
#include <stdio.h>
#include "sdd.h"


int verif_pren_piece(DAMIER *damier,PION *p1, PION *p2, int ax, int ay);
int verif_depl_piece(PION *p, int ax, int ay);
int verif_pion_jouable(DAMIER *damier, PION *p);

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
			damier->liste_pion[i]->pos.x=i%(COTE_DAMIER/2)*2;
		}else{
			damier->liste_pion[i]->pos.x=i%(COTE_DAMIER/2)*2+1;
		}
		damier->liste_pion[i]->pos.y=i/(COTE_DAMIER/2);
		damier->liste_pion[i]->couleur=BLANC;
		damier->liste_pion[i]->type=T_PION;
		damier->liste_pion[i+NB_PION_N]=(PION*)malloc(sizeof(PION));
		if(damier->liste_pion[i+NB_PION_N]==NULL){
			free_damier(damier);
			return NULL;
		}
		if(i/(COTE_DAMIER/2)%2==0){
			damier->liste_pion[i+NB_PION_N]->pos.x=COTE_DAMIER-i%(COTE_DAMIER/2)*2-1;
		}else{
			damier->liste_pion[i+NB_PION_N]->pos.x=COTE_DAMIER-i%(COTE_DAMIER/2)*2-2;
		}
		damier->liste_pion[i+NB_PION_N]->pos.y=COTE_DAMIER-i/(COTE_DAMIER/2)-1;
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

void suppr_pion(DAMIER *damier, PION *pion){

	int i=0;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			if(damier->liste_pion[i]==pion){
				damier->liste_pion[i]=NULL;
			}
		}
		i++;
	}
	//free(pion);
}

void promotion(DAMIER *damier){
	int i=0;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			if(damier->liste_pion[i]->couleur==BLANC && damier->liste_pion[i]->pos.y==COTE_DAMIER){
				damier->liste_pion[i]->type=T_DAME;
			}else if(damier->liste_pion[i]->couleur==NOIR && damier->liste_pion[i]->pos.y==0){
				damier->liste_pion[i]->type=T_DAME;
			}else{
			}
		}
		i++;
	}
}

int fin_partie(DAMIER *damier){
	int i=0,j=0,x,y,cpt=0;
	PION *p;
	
	if(verif_possibilite_prendre(damier)!=0){
		return 0;
	}
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			p=damier->liste_pion[i];
			x=p->pos.x;
			y=p->pos.y;
			for(j=0;j<4;j++){
				if((verif_depl_piece(p,x+(j/2*2-1),y+(j%2*2-1))==0) && (p->couleur==damier->c_tour)){
					return 0;
				}
			}
		}
		i++;
	}	

	return 1;
}

int verif_possibilite_prendre(DAMIER *damier){
	int i=0,j=0,x,y,cpt=0;
	PION *p;
	PION *p2;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			p=damier->liste_pion[i];
			x=p->pos.x;
			y=p->pos.y;
			for(j=0;j<4;j++){
				p2=rech_pion_c(damier,x+(j/2*2-1),y+(j%2*2-1));
				if(p2!=NULL){
					if((verif_pren_piece(damier,p,p2,x+(j/2*4-2),y+(j%2*4-2))==0) && (verif_pion_jouable(damier,p2)==3) && (p->couleur==damier->c_tour)){
						cpt++;
					}
				}
			}
		}
		i++;
	}

	return cpt;
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
			if(damier->liste_pion[i]->pos.x==x && damier->liste_pion[i]->pos.y==y){
				return damier->liste_pion[i];
			}
		}
		i++;
	}

	return NULL;
}

PION* rech_pion(DAMIER *damier, PION *pion){
	/**
	 * Recherche de pion
	 * La fonction parcours la liste de pions d'un DAMIER jusqu'à trouver une pièce de coordonnées(x,y) ou la fin de la liste
	 * La fonction renvoie un PION* ou NULL en cas d'échec
	 **/
	int i=0;
	
	while(i<NB_PION){
		if(damier->liste_pion[i]!=NULL){
			if(damier->liste_pion[i]==pion){
				return damier->liste_pion[i];
			}
		}
		i++;
	}

	return NULL;
}

int prendre(DAMIER *damier, PION *p1, PION *p2, int x, int y){

	int err;
	
	if(rech_pion(damier,p1)==NULL) return 1;
	if(rech_pion(damier,p2)==NULL) return 1;
	err=verif_pren_piece(damier,p1,p2,x,y);
	if(err) return err+4;

	last_modif.type=SUP;
	last_modif.value.dep[0][0]=p1->pos.x;
	last_modif.value.dep[0][1]=p1->pos.y;
	last_modif.value.dep[1][0]=x;
	last_modif.value.dep[1][1]=y;
	last_modif.value.sup[0]=p2->pos.x;
	last_modif.value.sup[1]=p2->pos.y;

	suppr_pion(damier,p2);
	p1->pos.x=x;
	p1->pos.y=y;

	return 0;

}

int deplacer(DAMIER *damier, PION *p, int ax, int ay){
	/**
	 * Déplacement d'une pièce
	 * Déplace une pièce d'un DAMIER de (dx,dy) à (ax,ay) si toute les conditions sont respectée
	 * 	la pièce éxiste en (dx,dy)
	 * 	la case (ax,ay) n'est pas emcombrée
	 * 	la couleur de la pièce correspond à la couleur du tour actuel
	 * 	le chemin est correct pour ce type de pièce (T_PION || T_DAME)
	 * renvoie 0 en cas de réussite
	 *         1 si la pièce n'éxiste pas en (dx,dy)
	 *         2 si l'adresse de la pièce donnée est NULL
	 *         3 si la pièce est introuvable dans la liste des pièces du damier
	 *         4 si la couleur de la pièce ne correspond pas à celle du tour actuel
	 *         5 si le chemin n'est pas en diagonale et/ou d'une longueur incorrect
	 *         6 si le chemin sort du plateau
	 *         7 si la pièce recule
	 *         8 si la case d'arrivée est encombrée
	 **/
	int err;

	if(rech_pion(damier,p)==NULL) return 1;
	err=verif_pion_jouable(damier,p);
	if(err) return err+1;
	err=verif_depl_piece(p,ax,ay);
	if(err) return err+4;
	if(rech_pion_c(damier,ax,ay)!=NULL) return 8;
	
	last_modif.type=DEP;
	last_modif.value.dep[0][0]=p->pos.x;
	last_modif.value.dep[0][1]=p->pos.y;
	last_modif.value.dep[1][0]=ax;
	last_modif.value.dep[1][1]=ay;
	
	p->pos.x=ax;
	p->pos.y=ay;

	return 0;
}

int verif_pion_jouable(DAMIER *damier, PION *p){
	/**
	 * Vérifie si une pièce est jouable pour ce tour
	 * retourne :	1 si l'adresse de la pièce donnée est NULL
	 * 		2 si la pièce est introuvable dans la liste des pièces du damier
	 * 		3 si la couleur de la pièce ne correspond pas à celle du tour actuel
	 * 		0 sinon
	 **/
	if(p==NULL) return 1;
	if(rech_pion(damier,p)==NULL) return 2;
	if(p->couleur!=damier->c_tour) return 3;

	return 0;
}

int verif_depl_piece(PION *p, int ax, int ay){
	/**
	 * Vérifie si une pièce de type T_PION peut ce déplacer de (dx,dy) à (ax,ay)
	 * retourne :	1 si le chemin est mauvais
	 * 		2 si la pièce sort du plateau
	 * 		3 si la direction si la pièce recule
	 * 		0 sinon
	 **/
	int dx = p->pos.x;
	int dy = p->pos.y;

	if(((dx-ax)*(dx-ax)!=(dy-ay)*(dy-ay)) || (p->type==T_PION && (((dx-ax)*(dx-ax))!=1 || ((dy-ay)*(dy-ay))!=1))) return 1;
	if(ax<0 || ay<0 || ax>=COTE_DAMIER || ay>=COTE_DAMIER) return 2;
	if(p->type==T_PION && ((p->couleur==BLANC && dy-ay>0) || (p->couleur==NOIR && dy-ay<0))) return 3;

	return 0;
}

int verif_pren_piece(DAMIER *damier, PION *p1, PION *p2, int ax, int ay){
	/**
	 * Vérifie si une pièce de type T_PION peut ce déplacer de (dx,dy) à (ax,ay)
	 * retourne :	1 si le chemin est mauvais
	 * 		2 si la pièce sort du plateau
	 * 		3 si la piece à prendre est mal positionnée
	 * 		4 si la case d'arrivée n'est pas vide
	 * 		5 si le pion à prendre n'est pas valide
	 * 		0 sinon
	 **/
	int dx = p1->pos.x;
	int dy = p1->pos.y;
	int k = p2->pos.x;
	int l = p2->pos.y;

	if(((dx-ax)*(dx-ax)!=(dy-ay)*(dy-ay)) || (p1->type==T_PION && (((dx-ax)*(dx-ax))!=4 || ((dy-ay)*(dy-ay))!=4))) return 1;
	if(ax<0 || ay<0 || ax>=COTE_DAMIER || ay>=COTE_DAMIER) return 2;
	if(((dx-k)*(dx-k)!=(dy-l)*(dy-l)) || (p1->type==T_PION && (((dx-k)*(dx-k))!=1 || ((dy-l)*(dy-l))!=1))) return 3;
	if(rech_pion_c(damier,ax,ay)!=NULL) return 4;
	if(verif_pion_jouable(damier,p2)!=3) return 5;
	
	return 0;
}

int readData(DAMIER *damier, DATA data){
	if(data.type==DEP){
		if(deplacer(damier,rech_pion_c(damier,data.value.dep[0][0],data.value.dep[0][1]),data.value.dep[1][0],data.value.dep[1][1])!=0) return 1;
	}else if(data.type==SUP){
		if(prendre(damier,rech_pion_c(damier,data.value.dep[0][0],data.value.dep[0][1]),rech_pion_c(damier,data.value.sup[0],data.value.sup[1]),data.value.dep[1][0],data.value.dep[1][1])!=0) return 1;
	}else{
		return 1;
	}

	return 0;
}

void aff_damier(DAMIER damier){
	/**
	 * Affiche un cadrillage d'un damier dans son état actuel, avec les pièces posnées et les coordonnées, dans la console
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
				if(p->type==T_PION){
					sprintf(buf,format,white,a*x+b,"0 ");
				}else{
					sprintf(buf,format,white,a*x+b,"D ");
				}
				system(buf);
			}else{
				if(p->type==T_PION){
					sprintf(buf,format,green,a*x+b,"O ");
				}else{
					sprintf(buf,format,green,a*x+b,"D ");
				}
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









