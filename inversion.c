#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"


/* Codes de retour */
#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_IM_IN 3
#define ERR_IM_TYPE 4
#define ERR_IM_OUT 5

void Syntaxe(void)
{
  Usage("ImageEntree ImageSortie\n"
        "-h\n");
}

void Description(void)
{
  Mesg("ROLE\n");
  Mesg("\tInversion d'une image binaire ou de niveaux de gris\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tImageEntree : nom du fichier source (format PBM ou PGM)\n");
  Mesg("\tImageSortie : nom du fichier destination (meme format que la source)\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : type d'image incorrect (binaire ou niveaux de gris)\n");
  Mesg("\t5 : probleme d'ecriture du fichier destination\n");
}

int main(int argc, char *argv[])
{
  Image ImIn=NULL, ImOut=NULL;
  
  /* Initialisation du mecanisme d'affichage des messages */
  InitMesg(argv);
  /* Verification du nombre de parametres */
	if (argc==1 || argc>=4)
	{
	  Syntaxe();
	  return ERR_NB_PARAM;
	}
	if (argc==2)
	{
	  if (!strcmp(argv[1],"-h"))
	  {
	    Syntaxe();
	    Description();
	    return HELP;
	  }
	  else /* un seul parametre different de -h */
	  {
	    Syntaxe();
	    return ERR_NB_PARAM;
	  }
	}
	/* Lecture de l'image source */
	ImIn=ImRead(argv[1]);
	if (ImIn==NULL)
	{
		Erreur("Probleme lors de la lecture de l'image en entree");
		return ERR_IM_IN;
	}
	/* Verification du type de l'image */
	if (ImType(ImIn)!=GrayLevel && ImType(ImIn)!=BitMap)
	{
		Erreur("L'image doit etre binaire ou en niveaux de gris");
		ImFree(&ImIn);
		return ERR_IM_TYPE;
	}
	/* Inversion de l'image */
	ImOut=Inversion(ImIn);
	if (ImOut==NULL)
	{
		Erreur("Probleme lors du traitement de l'image");
		ImFree(&ImIn);
		return ERR_IM_OUT;
	}
	/* Ecriture de l'image resultat */
	ImWrite(ImOut,argv[2]);
	/* Liberation des images */
	ImFree(&ImIn);
	ImFree(&ImOut);
	return OK;
}