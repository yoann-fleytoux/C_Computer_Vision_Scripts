#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_IM_IN 3
#define ERR_IM_TYPE 4
#define ERR_IM_OUT 5
#define ERR_MAT 6
#define ERR_MAT_TYPE 7
#define ERR_MAT_DIM 8

void Syntaxe(void){
  Usage("ImageEntree lut ImageSortie\n"
		"-h\n");
}

void Description(void){
	Mesg("ROLE\n");
	Mesg("\tApplique une transformation ponctuelle a une image\n");
	Mesg("ARGUMENTS\n");
	Mesg("\tImageEntree : nom du fichier source (format PBM ou PGM)\n");
	Mesg("\tlut : nom d'une matrice (format MAT)\n");
	Mesg("\tImageSortie : nom du fichier image destination (meme format que la source)\n");
	Mesg("OPTION\n");
	Mesg("\t-h : affichage de l'aide\n");
	Mesg("DIAGNOSTIC (codes de retour)\n");
	Mesg("\t0 : operation realisee sans probleme\n");
	Mesg("\t1 : aide demandee\n");
	Mesg("\t2 : mauvais nombre de parametres\n");
	Mesg("\t3 : probleme d'ouverture du fichier source\n");
	Mesg("\t4 : type d'image incorrect (niveaux de gris)\n");
	Mesg("\t5 : probleme d'ecriture du fichier destination\n");
	Mesg("\t6 : probleme de chargement de la matrice");
	Mesg("\t7 : type de la matrice incorrect (int)");
	Mesg("\t8 : les dimensions du masque ne sont pas correctes (n*n avec n impair) \n");

}

int main(int argc, char *argv[]){
  Image ImIn=NULL, ImOut=NULL;
  Matrix Mat=NULL;
  
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
	
	ImIn=ImRead(argv[1]);
	if (ImIn==NULL){
		Erreur("Probleme lors de la lecture de l'image en entree");
		return ERR_IM_IN;
	}
	if (ImType(ImIn)!=GrayLevel && ImType(ImIn)!=BitMap){
		Erreur("L'image doit etre en niveaux de gris");
		ImFree(&ImIn);
		return ERR_IM_TYPE;
	}

	Mat=MatReadAsc(argv[2]);
	if (Mat==NULL){
		Erreur("Probleme lors de la lecture de la matrice lut");
		ImFree(&ImIn);
		return ERR_MAT;
	}
	if (MatType(Mat)!=Int){
		Erreur("La matrice doit etre en Int");
		MatFree(&Mat);
		ImFree(&ImIn);
		return ERR_MAT_TYPE;
	}
	if (MatNbRow(Mat)!=1 && MatNbCol(Mat)!=1){
		Erreur("La matrice doit etre de dimension 1*n");
		MatFree(&Mat);
		ImFree(&ImIn);
		return ERR_MAT_TYPE;
	}
	
	ImOut=AppLut(ImIn, Mat);
	if (ImOut==NULL){
		Erreur("Probleme lors du traitement de l'image");
		ImFree(&ImIn);
		MatFree(&Mat);
		return ERR_IM_OUT;
	}
	ImWrite(ImOut,argv[3]);
	ImFree(&ImIn);
	MatFree(&Mat);
	ImFree(&ImOut);
	return OK;
}