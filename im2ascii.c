#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

/*CETTE FONCTION N'EST PAS DEMANDEE DANS LE SUJET
  C'EST JUSTE UNE IDEE QUE J'AI EU.. DONC POURQUOI PAS :)
  VOUS POUVEZ LA TESTER AVEC L'IMAGE "ascii.pgm"*/

#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_IM_IN 3
#define ERR_IM_TYPE 4

void Syntaxe(void){
  Usage("Image\n"
		"-h\n");
}

void Description(void){
  Mesg("ROLE\n");
  Mesg("\tAffiche l'image sous forme de text sur le terminal\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tImage : nom de l'image a afficher\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : type d'image incorrect (GrayLevel)\n");
}

int main(int argc, char *argv[]){
	Image ImIn=NULL;
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
	fIn=argv[1];

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
	Im2Ascii(ImIn);
	ImFree(&ImIn);
	return OK;
}