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
#define ERR_MAT_OUT 6
#define ERR_MAT_TYPE 7
#define ERR_MASQUE 8

void Syntaxe(void){
  Usage("[c|n|b] Image.pgm Masque.mat MatriceResultat.mat\n"
		"-h\n");
}

void Description(void){
	Mesg("ROLE\n");
	Mesg("\tRealise la convolution d'une image par un masque\n");
	Mesg("ARGUMENTS\n");
	Mesg("\tImage : nom du fichier Image source\n");
	Mesg("\tMasque : nom du fichier Matrix (MAT) source (masque de dimension(n,n) avec n impair)\n");
	Mesg("\tMatriceResultat : nom du fichier Matrix (MAT) destination (Image transformee de type Double)\n");
	Mesg("OPTION\n");
	Mesg("\t-h : affichage de l'aide\n");
	Mesg("\t-c : les pixels de bord prennent la valeur des pixels de l'image initiale\n");
	Mesg("\t-n : les pixels de bord sont mis a 0 (noir)\n");
	Mesg("\t-b : les pixels de bord sont mis a 1 (blanc)\n");
	Mesg("DIAGNOSTIC (codes de retour)\n");
	Mesg("\t0 : operation realisee sans probleme\n");
	Mesg("\t1 : aide demandee\n");
	Mesg("\t2 : mauvais nombre de parametres\n");
	Mesg("\t3 : probleme d'ouverture du fichier Image\n");
	Mesg("\t4 : type d'image incorrect (GrayLevel)\n");
	Mesg("\t5 : probleme d'ouverture du fichier Matrice\n");
	Mesg("\t6 : probleme d'ecriture du fichier destination\n");
	Mesg("\t7 : type de matrice incorrect (Double)\n");
	Mesg("\t8 : les dimensions du masque ne sont pas correctes (n*n avec n impair) \n");
}

int main(int argc, char *argv[]){
	Image ImIn=NULL;
	Matrix MsqIn=NULL;
	Matrix MatOut=NULL;
	char *arg1In, *arg2In, *argOut;
	int opt;

	InitMesg(argv);
	if (argc!=2 && argc!=4 && argc!=5){
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
	if (argc==4){
		opt=-1;
		arg1In=argv[1];
	  	arg2In=argv[2];
	  	argOut=argv[3];
	}
	else{
		if(!strcmp(argv[1],"-c")) opt=-1;
		else if(!strcmp(argv[1],"-n")) opt=0;
		else if(!strcmp(argv[1],"-b")) opt=255;
		else {
			Syntaxe();
			return ERR_NB_PARAM;
		}
		arg1In=argv[2];
	  	arg2In=argv[3];
	  	argOut=argv[4];
	}

	ImIn=ImRead(arg1In);
	if (ImIn==NULL){
		Erreur("Probleme lors de la lecture de l'image");
		return ERR_IM_IN;
	}
	if (ImType(ImIn)!=GrayLevel){
		Erreur("L'image doit etre de type Int");
		ImFree(&ImIn);
		return ERR_IM_TYPE;
	}

	MsqIn=MatReadAsc(arg2In);
	if (MsqIn==NULL){
		Erreur("Probleme lors de la lecture du masque");
		return ERR_MAT_IN;
	}
	if (MatType(MsqIn)!=Double){
		Erreur("Le masque doit etre de type Double");
		MatFree(&MsqIn);
		ImFree(&ImIn);
		return ERR_MAT_TYPE;
	}
	if (MatNbRow(MsqIn)!=MatNbCol(MsqIn) || (MatNbCol(MsqIn)%2 ==0 || MatNbRow(MsqIn)%2 ==0)){
		Erreur("Dimensions du masque pas correctes");
		ImFree(&ImIn);
		MatFree(&MsqIn);
		return ERR_MASQUE;
	}

	MatOut=Conv(ImIn, MsqIn, opt);
	if (MatOut==NULL){
		Erreur("Probleme lors du calcul de la matrice lut");
		ImFree(&ImIn);
		MatFree(&MsqIn);
		return ERR_MAT_OUT;
	}

	MatWriteAsc(MatOut,argOut);
	MatFree(&MatOut);
	MatFree(&MsqIn);
	ImFree(&ImIn);
	return OK;
}