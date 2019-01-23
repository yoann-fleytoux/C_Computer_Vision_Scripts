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
  Usage("HistogrammeCumule HistogrammeCumuleDesire lut\n"
		"-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tCalcul la transformation ponctuelle donnee par l'algorithme de specification a partir de 2 histogrammes\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tHistogrammeCumule : nom du fichier Matrix (MAT) source\n");
  Mesg("\tHistogrammeCumuleDesire : nom du fichier Matrix (MAT) source\n");
  Mesg("\tlut : nom du fichier Matrix (MAT) destination\n");
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
	Matrix HistIn=NULL;
	Matrix HistDIn=NULL;
	Matrix LUTOut=NULL;
	char *arg1In, *arg2In, *argOut;

	InitMesg(argv);
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
		else{
			Syntaxe();
			return ERR_NB_PARAM;
	  	}
	}
  	arg1In=argv[1];
  	arg2In=argv[2];
  	argOut=argv[3];

	HistIn=MatReadAsc(arg1In);
	if (HistIn==NULL){
		Erreur("Probleme lors de la lecture de l'histogramme (cumule)");
		return ERR_MAT_IN;
	}
	if (MatType(HistIn)!=Int){
		Erreur("L'histogramme (cumule) doit etre de type Int");
		MatFree(&HistIn);
		return ERR_MAT_TYPE;
	}

	HistDIn=MatReadAsc(arg2In);
	if (HistDIn==NULL){
		Erreur("Probleme lors de la lecture de l'histogramme (cumule desire)");
		return ERR_MAT_IN;
	}
	if (MatType(HistDIn)!=Int){
		Erreur("L'histogramme (cumule desire) doit etre de type Int");
		MatFree(&HistDIn);
		return ERR_MAT_TYPE;
	}

	LUTOut=SpecifHist(HistIn, HistDIn);
	if (LUTOut==NULL){
		Erreur("Probleme lors du calcul de la matrice lut");
		MatFree(&HistIn);
		return ERR_MAT_OUT;
	}	
	MatWriteAsc(LUTOut,argOut);
	MatFree(&LUTOut);
	MatFree(&HistDIn);
	MatFree(&HistIn);
	return OK;
}