#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_MAT_IN 3
#define ERR_MAT_OUT 4
#define ERR_MAT_TYPE 5

void Syntaxe(void){
  Usage("Histogramme HistogrammeCumule\n"
		"-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tCalcul d'histogramme cumule a partir d'un autre histogramme\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tHistogramme : nom du fichier Matrix (MAT) source\n");
  Mesg("\tHistogrammeCumule : nom du fichier Matrix (MAT) destination\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : probleme d'ecriture du fichier destination\n");
  Mesg("\t5 : type de matrice incorrect (Int)\n");
}

int main(int argc, char *argv[]){
	Matrix MatIn=NULL;
	Matrix MatOut=NULL;
	char *fIn,*fOut;

	InitMesg(argv);
	if (argc!=2 && argc!=3){
	  	Syntaxe();
	  	return ERR_NB_PARAM;
	}
	if (argc==2){
	  	if (!strcmp(argv[1],"-h")){
			Syntaxe();
			Description();
			return HELP;
	  	}
		else{
			Syntaxe();
			return ERR_NB_PARAM;
	  	}
	}
	fIn=argv[1];
	fOut=argv[2];

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
	MatOut=Hist2CumHist(MatIn);
	if (MatOut==NULL){
		Erreur("Probleme lors du calcul de l'histogramme cumule");
		MatFree(&MatIn);
		return ERR_MAT_OUT;
	}	
	MatWriteAsc(MatOut,fOut);
	MatFree(&MatOut);
	MatFree(&MatIn);
	return OK;
}