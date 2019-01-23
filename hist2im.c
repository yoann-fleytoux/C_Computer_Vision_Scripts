#include <stdio.h>
#include <string.h>
#include "limace.h"
#include "tai.h"

/* Codes de retour */
#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_MAT_IN 3
#define ERR_IM_OUT 4
#define ERR_MAT_TYPE 5
#define ERR_PAR_TYPE 6

void Syntaxe(void)
{
  Usage("[NbLig] MatriceEntree ImageSortie\n"
        "-h\n");
}

void Description(void)
{
  Mesg("ROLE\n");
  Mesg("\tRepresentation d'un histogramme sous forme d'une image\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tNbLig : nombre de lignes de l'image resultat (200 par defaut)\n");
  Mesg("\tMatriceEntree : fichier au format Matrix contenant l'histogramme\n");
  Mesg("\tImageSortie : nom du fichier image (PGM) destination\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t0 : operation realisee sans probleme\n");
  Mesg("\t1 : aide demandee\n");
  Mesg("\t2 : mauvais nombre de parametres\n");
  Mesg("\t3 : probleme d'ouverture du fichier source\n");
  Mesg("\t4 : probleme d'ecriture du fichier destination\n");
  Mesg("\t5 : type de matrice incorrect (Matrix de Int)\n");
  Mesg("\t6 : type incorrect du nombre de ligne\n");
}

int main(int argc, char *argv[])
{
	Matrix MatIn=NULL;
	Image ImOut=NULL;
	int NbLig;
	char *fIn,*fOut;

    /* Initialisation du mecanisme d'affichage des messages */
    InitMesg(argv);
    /* Verification du nombre de parametres */
	if (argc==1 || argc>4)
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
	if (argc==4)
	{
	  if (sscanf(argv[1],"%d",&NbLig) && NbLig>0)
	  {
	    fIn=argv[2];
	    fOut=argv[3];
	  }
	  else
	  {
	    Erreur("Le nombre de ligne doit etre un entier strictement positif");
	    Syntaxe();
	    return ERR_PAR_TYPE;
	  }
	}
	else
	{
	  NbLig=200;
	  fIn=argv[1];
	  fOut=argv[2];
	}
	    	  
	MatIn=MatReadAsc(fIn);
	if (MatIn==NULL)
	{
		Erreur("Probleme lors de la lecture de l'histogramme en entree");
		return ERR_MAT_IN;
	}
	if (MatType(MatIn)!=Int)
	{
		Erreur("L'histogramme doit etre stocke dans une matrice de Int");
		MatFree(&MatIn);
		return ERR_MAT_TYPE;
	}
	ImOut=Hist2Im(MatIn,NbLig);
	if (ImOut==NULL)
	{
		Erreur("Probleme lors de la formation de l'image de l'histogramme");
		MatFree(&MatIn);
		return ERR_IM_OUT;
	}	
	ImWrite(ImOut,fOut);
	MatFree(&MatIn);
	ImFree(&ImOut);
	return OK;
}