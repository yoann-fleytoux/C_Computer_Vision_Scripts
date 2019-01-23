#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_MAT_IN 3
#define ERR_IM_OUT 4
#define ERR_MAT_TYPE 5

void Syntaxe(void){
  Usage("[-p|-r] Matrice.mat Image.pgm\n"
		"-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tTransformation d'une matrice en image\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tMatrice : fichier au format Matrix\n");
  Mesg("\tImage : nom du fichier image (PGM) destination\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("\t-p : affichage de l'aide\n");
  Mesg("\t-r : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : probleme d'ecriture du fichier destination\n");
  Mesg("\t5 : type de matrice incorrect (Matrix de Double)\n");
}

int main(int argc, char *argv[]){
	Matrix MatIn=NULL;
	Image ImOut=NULL;
	int opt;
	char *fIn,*fOut;

	InitMesg(argv);
	if (argc!=2 && argc!=3 && argc!=4){
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
	if (argc==3){
		opt=0;
		fIn=argv[1];
		fOut=argv[2];
	}else{
		if(!strcmp(argv[1],"-p")) opt=0;
		else if(!strcmp(argv[1],"-r")) opt=1;
		else {
			Syntaxe();
			return ERR_NB_PARAM;
		}
		fIn=argv[2];
	  	fOut=argv[3];
	}
	MatIn=MatReadAsc(fIn);
	if (MatIn==NULL){
		Erreur("Probleme lors de la lecture de la matrice en entree");
		return ERR_MAT_IN;
	}
	if (MatType(MatIn)!=Double){
		Erreur("La matrice doit etre de type Double");
		MatFree(&MatIn);
		return ERR_MAT_TYPE;
	}
	ImOut=Mat2Im(MatIn, opt);
	if (ImOut==NULL){
		Erreur("Probleme lors de la formation de l'image de la matrice");
		MatFree(&MatIn);
		return ERR_IM_OUT;
	}	
	ImWrite(ImOut,fOut);
	MatFree(&MatIn);
	ImFree(&ImOut);
	return OK;
}