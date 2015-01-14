#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdd.h"
#include "com.h"

void affTour(DAMIER *damier){
	switch(damier->c_tour){
		case 0: printf("Tour blanc\n");
			break;
		case 1: printf("Tour noir\n");
			break;
		default : printf("Tour inconnu");
			  break;
	}
}

int main(int argc, char *argv[]){
	int nbByteCom;
	int i=0, x, y, a, b, k, l, erreur, mauvMouv;
	char str2[100];
	char stop;
	int nbPara;
	int nbPionJ;
	int sockfd;
	DATA *dataBuf=(DATA*)malloc(sizeof(DATA));
	DAMIER *damier;
	PION *p;
	DATA dataFin;
	dataFin.type=FIN;
	damier = init_damier();
	if(damier==NULL){
		printf("Initialisation du damier échouée\n");
		return 0;
	}

	if(argc==2){
		printf("Server, en attente d'un client sur le port %s\n", argv[1]);
		sockfd=server(atoi(argv[1]));
		if(sockfd<0) return 1;
		aff_damier(*damier);

	}else if(argc==3){
		printf("Client, connection au server %s, sur le port %s\n", argv[1], argv[2]);
		sockfd=client(argv[1],atoi(argv[2]));
		if(sockfd<0) return 1;
		
		aff_damier(*damier);
		affTour(damier);
		printf("Nombre de prise possible : %d\n", verif_possibilite_prendre(damier));
		puts("En attente du joueur adverse...");
		nbByteCom=read(sockfd,dataBuf,sizeof(DATA));
		if(nbByteCom>0){
			while(dataBuf->type!=FIN){
				if(readData(damier,*dataBuf)){
					printf("Desynchronisation\n");
					close(sockfd);
					free(dataBuf);
					free_damier(damier);
					return 1;
				}
				aff_damier(*damier);
				nbByteCom=read(sockfd,dataBuf,sizeof(DATA));
			}
		}else{
			printf("Connection perdue\n");
			close(sockfd);
			if(dataBuf!=NULL) free(dataBuf);
			if(damier!=NULL) free_damier(damier);
			exit(0);
		}
		tour_suivant(damier);
	}else{
		printf("Nombre d'arguments incorrect\n");
		return 1;
	}


	while(!fin_partie(damier)){
		mauvMouv=1;
		promotion(damier);
		affTour(damier);
		printf("Nombre de prise possible : %d\n", verif_possibilite_prendre(damier));
		while(mauvMouv){
			mauvMouv=0;
			printf("Action sur un pion : ");
			nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %d,%d %d,%d", &x, &y, &a, &b, &k, &l);
			if(nbPara<1){
				printf("Bye!\n");
				close(sockfd);
				if(dataBuf!=NULL) free(dataBuf);
				if(damier!=NULL) free_damier(damier);
				exit(0);	
			}
			while(nbPara<4){
				printf("paramètre manquant(%d)\n",nbPara);
				printf("Action sur un pion : ");
				nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %d,%d %d,%d", &x, &y, &a, &b, &k, &l);
				if(nbPara<1){
					printf("Bye!\n");
					close(sockfd);
					if(dataBuf!=NULL) free(dataBuf);
					if(damier!=NULL) free_damier(damier);
					exit(0);	
				}
			}
			PION *p1 = rech_pion_c(damier,x,y);
			if(nbPara<6){
				k=a+(a-x);
				l=b+(b-y);
			}
			if(verif_possibilite_prendre(damier)==0){
				if(deplacer(damier,p1,a,b)==0){
					printf("Pion déplacé en (%d,%d)\n",a,b);
					nbByteCom=write(sockfd,&last_modif,sizeof(DATA));
				}else{
					printf("MAUVAIS MOUVEMENT\n");
					mauvMouv=1;
				}
			}else{
				if(prendre(damier,p1,rech_pion_c(damier,a,b),k,l)==0){
					printf("Prise de (%d,%d) par (%d,%d) en (%d,%d)\n",a,b,x,y,k,l);
					nbByteCom=write(sockfd,&last_modif,sizeof(DATA));
					while(verif_possibilite_prendre(damier)>0){
						aff_damier(*damier);
						printf("Rafle : ");
						nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %d,%d %d,%d", &x, &y, &a, &b, &k, &l);
						if(nbPara<1){
							printf("Bye!\n");
							close(sockfd);
							if(dataBuf!=NULL) free(dataBuf);
							if(damier!=NULL) free_damier(damier);
							exit(0);	
						}
						while(nbPara<4){
							printf("paramètre manquant(%d)\n",nbPara);
							printf("Rafle : ");
							nbPara=sscanf(fgets(str2,14,stdin),"%d,%d %d,%d %d,%d", &x, &y, &a, &b, &k, &l);
							if(nbPara<1){
								printf("Bye!\n");
								close(sockfd);
								if(dataBuf!=NULL) free(dataBuf);
								if(damier!=NULL) free_damier(damier);
								exit(0);	
							}
						}
						if(nbPara<6){
							k=a+(a-x);
							l=b+(b-y);
						}
						if(prendre(damier,p1,rech_pion_c(damier,a,b),k,l)==0){
							printf("Prise de (%d,%d) par (%d,%d) en (%d,%d)\n",a,b,x,y,k,l);
							nbByteCom=write(sockfd,&last_modif,sizeof(DATA));
						}else{
							printf("Prise de (%d,%d) par (%d,%d) en (%d,%d)\n",a,b,x,y,k,l);
							printf("MAUVAIS MOUVEMENT\n");
						}
					}
				}else{
					printf("MAUVAIS MOUVEMENT\n");
					mauvMouv=1;
				}
			}
		}
		aff_damier(*damier);
		nbByteCom=write(sockfd,&dataFin,sizeof(DATA));
		tour_suivant(damier);
		affTour(damier);
		printf("Nombre de prise possible : %d\n", verif_possibilite_prendre(damier));
		printf("En attente du joueur adverse...\n");
		nbByteCom=read(sockfd,dataBuf,sizeof(DATA));
		if(nbByteCom>0){
			while(dataBuf->type!=FIN){
				if(readData(damier,*dataBuf)){
					printf("Desynchronisation\n");
					close(sockfd);
					free(dataBuf);
					free_damier(damier);
					return 1;
				}
				aff_damier(*damier);
				nbByteCom=read(sockfd,dataBuf,sizeof(DATA));
			}
		}else{
			printf("Connection perdue\n");
			close(sockfd);
			if(dataBuf!=NULL) free(dataBuf);
			if(damier!=NULL) free_damier(damier);
			exit(0);
		}
		tour_suivant(damier);
	}
	free_damier(damier);
	free(dataBuf);
	close(sockfd);
	return 0;
}
