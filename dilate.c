#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_IM_IN 3
#define ERR_IM_TYPE 4
#define ERR_MAT_IN 5
#define ERR_IM_OUT 6
#define ERR_MAT_TYPE 7
#define ERR_MASQUE 8

void Syntaxe(void){
  Usage("ImageEntree.pbm ElementStructurant.mat ImageSortie.pbm\n"
		"-h\n");
}

void Description(void){
	Mesg("ROLE\n");
	Mesg("\tRealise la dilatation d'une image binaire (de type BitMap) par un masque\n");
	Mesg("ARGUMENTS\n");
	Mesg("\tImageEntree : nom du fichier Image source\n");
	Mesg("\tElementStructurant : nom du fichier Matrix (MAT) source (masque de dimension(n,n) avec n impair)\n");
	Mesg("\tImageSortie : nom du fichier Image destination (Image transformee de type BitMap)\n");
	Mesg("OPTION\n");
	Mesg("\t-h : affichage de l'aide\n");
	Mesg("DIAGNOSTIC (codes de retour)\n");
	Mesg("\t0 : operation realisee sans probleme\n");
	Mesg("\t1 : aide demandee\n");
	Mesg("\t2 : mauvais nombre de parametres\n");
	Mesg("\t3 : probleme d'ouverture du fichier Image\n");
	Mesg("\t4 : type d'image incorrect (BitMap)\n");
	Mesg("\t5 : probleme d'ouverture du fichier Matrice\n");
	Mesg("\t6 : probleme d'ecriture du fichier destination\n");
	Mesg("\t7 : type de matrice incorrect (Int)\n");
	Mesg("\t8 : les dimensions du masque ne sont pas correctes (n*n avec n impair) \n");
}

int main(int argc, char *argv[]){
	Image ImIn=NULL;
	Matrix MsqIn=NULL;
	Image ImOut=NULL;
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
	else{
		arg1In=argv[1];
		arg2In=argv[2];
		argOut=argv[3];
	}
	ImIn=ImRead(arg1In);
	if (ImIn==NULL){
		Erreur("Probleme lors de la lecture de l'image");
		return ERR_IM_IN;
	}
	if (ImType(ImIn)!=BitMap){
		Erreur("L'image doit etre de type BitMap");
		ImFree(&ImIn);
		return ERR_IM_TYPE;
	}

	MsqIn=MatReadAsc(arg2In);
	if (MsqIn==NULL){
		Erreur("Probleme lors de la lecture du masque");
		return ERR_MAT_IN;
	}
	if (MatType(MsqIn)!=Int){
		Erreur("Le masque doit etre de type Int");
		MatFree(&MsqIn);
		ImFree(&ImIn);
		return ERR_MAT_TYPE;
	}
	if (MatNbRow(MsqIn)!=MatNbCol(MsqIn) || MatNbCol(MsqIn)%2 ==0 || MatNbRow(MsqIn)%2 ==0){
		Erreur("Dimensions du masque pas correctes");
		ImFree(&ImIn);
		MatFree(&MsqIn);
		return ERR_MASQUE;
	}
	ImOut=Dilate(ImIn, MsqIn);
	if (ImOut==NULL){
		Erreur("Probleme lors de la dilatation de l'image");
		ImFree(&ImIn);
		MatFree(&MsqIn);
		return ERR_IM_OUT;
	}	
	ImWrite(ImOut,argOut);
	ImFree(&ImOut);
	MatFree(&MsqIn);
	ImFree(&ImIn);
	return OK;
}