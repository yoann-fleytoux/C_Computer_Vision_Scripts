#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

/* Codes de retour */
#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_IM_IN 3
#define ERR_IM_OUT 4
#define ERR_IM_TYPE 5
#define ERR_PAR_TYPE 6

void Syntaxe(void){
  Usage("ImageEntree Seuil ImageSortie\n"
        "-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tBinarisation d'une image\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tImageEntree : image (niveau de gris)\n");
  Mesg("\tSeuil : entier qui determine si le pixel binarise est blanc ou noir\n");
  Mesg("\tImageSortie : image binarisee (PBM)\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : probleme d'ecriture du fichier destination\n");
  Mesg("\t5 : type d'image incorrect (GrayLevel)\n");
  Mesg("\t6 : type incorrect du seuil (entier)\n");
}

int main(int argc, char *argv[]){
	Image ImIn=NULL;
	Image ImOut=NULL;
	int seuil;
	char *fIn,*fOut;

    /* Initialisation du mecanisme d'affichage des messages */
    InitMesg(argv);
    /* Verification du nombre de parametres */
	if (argc!=2 && argc!=4){
	  Syntaxe();
	  return ERR_NB_PARAM;
	}
	if (argc==2){
	  if (!strcmp(argv[1],"-h")){
	    Syntaxe();
	    Description();
	    return HELP;
	  }
	  else{ /* un seul parametre different de -h */
	    Syntaxe();
	    return ERR_NB_PARAM;
	  }
	}
	else{
	  if (sscanf(argv[2],"%d",&seuil) && seuil>0 && seuil<255){
	    fIn=argv[1];
	    fOut=argv[3];
	  }
	  else{
	    Erreur("Le seuil doit etre un entier entre 1 et 254");
	    Syntaxe();
	    return ERR_PAR_TYPE;
	  }
	}

	ImIn=ImRead(fIn);
	if (ImIn==NULL){
		Erreur("Probleme lors de la lecture de l'image en entree");
		return ERR_IM_IN;
	}
	if (ImType(ImIn)!=GrayLevel){
		Erreur("L'image doit etre de type GrayLevel");
		ImFree(&ImIn);
		return ERR_IM_TYPE;
	}
	ImOut=Bin(ImIn, seuil);
	if (ImOut==NULL){
		Erreur("Probleme lors de la binarisation");
		ImFree(&ImIn);
		return ERR_IM_OUT;
	}	
	ImWriteAsc(ImOut,fOut);
	ImFree(&ImOut);
	ImFree(&ImIn);
	return OK;
} 