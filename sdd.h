#ifndef SDD_H
#define SDD_H

//PREPROCESSEUR
#define COTE_DAMIER 10
#define TAILLE_DAMIER COTE_DAMIER*COTE_DAMIER
#define NB_PION_B 20
#define NB_PION_N 20
#define NB_PION NB_PION_B+NB_PION_N

//STRUCTURES
typedef struct{
	int x;
	int y;
}POS;

typedef enum{
	T_PION,
	T_DAME
}eTYPE_PION;

typedef enum{
	BLANC,
	NOIR
}eCOULEUR;

typedef struct{
	POS pos;
	eCOULEUR couleur;
	eTYPE_PION type;
}PION;

typedef struct{
	eCOULEUR c_tour;
	int nb_pion_b;
	int nb_pion_n;
	PION **liste_pion;
}DAMIER;

typedef struct data{
	enum{DEP,SUP,FIN}type;
	struct{
		int dep[2][2];
		int sup[2];
	}value;
}DATA;
//GLOBALE
DATA last_modif;
//FONCTIONS
DAMIER* init_damier();
int free_damier(DAMIER *damier);
void suppr_pion(DAMIER *damier, PION *pion);
void promotion(DAMIER *damier);
void tour_suivant(DAMIER *damier);
int verif_possibilite_prendre(DAMIER *damier);
PION* rech_pion_c(DAMIER *damier, int x, int y);
PION* rech_pion(DAMIER *damier, PION *pion);
void aff_damier(DAMIER damier);
int deplacer(DAMIER *damier, PION *p, int ax, int ay);
int prendre(DAMIER *damier, PION* p1, PION *p2, int x, int y);
int readData(DAMIER *damier, DATA data);
int fin_partie(DAMIER *damier);




#endif
