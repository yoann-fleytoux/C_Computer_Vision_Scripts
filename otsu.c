#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_MAT_IN 3
#define ERR_MAT_TYPE 4

void Syntaxe(void){
  Usage("Histogramme\n"
		"-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tCalcule le seuil optimal selon Otsu d'une image a partir de son histogramme\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tHistogramme : nom du fichier Matrix (MAT) source\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : type de matrice incorrect (Int)\n");
}

int main(int argc, char *argv[]){
	Matrix MatIn=NULL;
	char *fIn;

	InitMesg(argv);
	if (argc!=2){
		Syntaxe();
		return ERR_NB_PARAM;
	}
	if (!strcmp(argv[1],"-h")){
		Syntaxe();
		Description();
		return HELP;
	}
	else{
		fIn=argv[1];
	}

	MatIn=MatReadAsc(fIn);
	if (MatIn==NULL){
		Erreur("Probleme lors de la lecture de l'histogramme en entree");
		return ERR_MAT_IN;
	}
	if (MatType(MatIn)!=Int){
		Erreur("L'histogramme doit etre de type Int");
		MatFree(&MatIn);
		return ERR_MAT_TYPE;
	}
	printf("%d\n", Otsu(MatIn));
	MatFree(&MatIn);
	return OK;
}