/*  Karim SALAMA
 */
/* Base: TAI release 2 (25/03/2014) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "limace.h"
#include "tai.h"


#define DEBOGAGE

#ifdef DEBOGAGE
#define DEBUG fprintf(stderr,"Fichier %s, ligne %d\n",__FILE__,__LINE__);
#else
#define DEBUG
#endif

/*
 * Fonction a appeler en debut de main pour initialiser la gestion des erreurs 
 */
char *InitMesg(char *argv[]){
	static char *Prog=NULL;

	if (Prog==NULL){
	if ((Prog=strrchr(argv[0],'/')))
	  Prog++;
	else
	  Prog=argv[0];
  }
  return Prog;
}

/*
 * Affichage d'un message formate sur stderr
 */
void Mesg(const char *Msg, ...){
  va_list Params;

  va_start(Params, Msg);
  vfprintf(stderr, Msg, Params);
  va_end(Params);
}

/*
 * Affichage de la syntaxe d'appel d'un operateur sur stderr
 */
void Usage(char *Syntaxe){
  char *Ligne, *Copie;

  Copie=malloc(strlen(Syntaxe)+1);
  strcpy(Copie,Syntaxe);
  Mesg("SYNTAXE\n");
  Ligne=strtok(Copie,"\n");
  while (Ligne!=NULL){
	Mesg("\t%s %s\n",InitMesg(NULL),Ligne);
	Ligne=strtok(NULL,"\n");
  }
  free(Copie);
}

/*
 * Affichage d'un message d'erreur sur stderr
 */
void Erreur(char *Msg){
  Mesg("[%s] %s\n",InitMesg(NULL),Msg);
}

/*
 * Inversion d'une image binaire ou de niveaux de gris
 * Entree : image initiale (en niveaux de gris ou binaire)
 * Sortie : Image resultat
 */
Image Inversion(Image Im){
	Image Res;
	unsigned char **I,**R;
	int i,j,l,c;
	
	if (ImType(Im)!=BitMap && ImType(Im)!=GrayLevel) return NULL;
	l=ImNbRow(Im); c=ImNbCol(Im);
	I=ImGetI(Im);
	if (ImType(Im)==BitMap){
		Res=ImCAlloc(BitMap,l,c);
		if (Res==NULL) return NULL;
		R=ImGetI(Res);
		for (i=0;i<l;i++)
			for (j=0;j<c;j++)
				R[i][j]=!I[i][j];
	}
	else{
		Res=ImCAlloc(GrayLevel,l,c);
		if (Res==NULL) return NULL;
			R=ImGetI(Res);
		for (i=0;i<l;i++)
			for (j=0;j<c;j++)
				R[i][j]=255-I[i][j];
	}
	return Res;
}

/*
 * Representation d'un histogramme sous forme d'une image
 * Entrees : histogramme et nombre de lignes de l'image resultat
 * Sortie : image resultat
 */
Image Hist2Im(Matrix Hist, int NbLig){
	unsigned char **I;
	int *h,i,j,Max=0,NbCol=256;
	Image Resultat;
	
	if (MatType(Hist)!=Int) return NULL;
	Resultat=ImCAlloc(GrayLevel,NbLig,NbCol);
	h=*MatGetInt(Hist);
	for (j=0;j<NbCol;j++)
		if (h[j]>Max) Max=h[j];
	I=ImGetI(Resultat);
	for (j=0;j<256;j++)
		for (i=NbLig-1;i>=(NbLig-NbLig*h[j]/Max);i--)
			I[i][j]=255;
	return Resultat;
}

/*
 * Appliquer une transformation ponctuelle a une image selon une matrice
 * Entrees : Image (Niveau de gris)
 * Sortie : Image transformee
 */
Image AppLut(Image Im, Matrix Mat){
	unsigned char **I, **B;
	int *h,i,j;
	Image Resultat;
	
	if (ImType(Im)!=GrayLevel) return NULL;
	Resultat=ImCAlloc(GrayLevel,ImNbRow(Im),ImNbCol(Im));
	I=ImGetI(Im);
	h=*MatGetInt(Mat);
	B=ImGetI(Resultat);
	for (j=0;j<ImNbCol(Im);j++)
		for (i=0;i<ImNbRow(Im);i++)
			B[i][j]= h[I[i][j]];
	return Resultat;
}

/*
 * Creation d'un histogramme a partir d'une image
 * Entrees : image (niveau de gris)
 * Sortie : matrice histogramme
 */
Matrix Hist(Image Im){
	unsigned char **I;
	int *h,i,j;
	Matrix Resultat;
	
	if (ImType(Im)!=GrayLevel) return NULL;
	Resultat=MatCAlloc(Int,1,256);
	I=ImGetI(Im);
	h=*MatGetInt(Resultat);
	for (j=0;j<ImNbCol(Im);j++)
		for (i=0;i<ImNbRow(Im);i++)
			h[I[i][j]]++;
	return Resultat;
}

/*
 * Creation de l'histogramme egalise a partir d'une image
 * Entrees : ImageEntree : image (niveau de gris)
 * Sortie : Histogramme : nom du fichier Matrix (MAT) destination
 */
Matrix HistEg(Image Im){
	int *h,*hst,*hc,i;
	double p;
	Matrix hist = Hist(Im);
	Matrix hCum = Hist2CumHist(Hist(Im));
	Matrix Resultat;

	if (ImType(Im)!=GrayLevel) return NULL;
	Resultat=MatCAlloc(Int,1,256);
	h=*MatGetInt(Resultat);
	hst=*MatGetInt(hist);
	hc=*MatGetInt(hCum);
	for (i=0;i<256;i++){
		p=255*hc[i]/((ImNbRow(Im))*(ImNbCol(Im)));
		h[(int)(p)] = h[(int)(p)] + hst[i];
	}
	return Resultat;
}

/*
 * Calculer l'histogramme cumule a partir d'un autre histogramme
 * Entrees : histogramme
 * Sortie : histogramme cumule
 */
Matrix Hist2CumHist(Matrix Hist){
	int *h,*hc,i;
	Matrix Resultat;
	Resultat=MatCAlloc(Int,1,256);
	h=*MatGetInt(Hist);
	hc=*MatGetInt(Resultat);
		hc[0]=h[0];
		for (i=1;i<256;i++) hc[i]=hc[i-1]+h[i];
	return Resultat;
}

/*
 * Calculer la transformation ponctuelle donnee par l'algorithme de specification a partir de 2 histogrammes
 * Entrees : histogramme, histogramme desire
 * Sortie : matrice lut
 */
Matrix SpecifHist(Matrix HistC, Matrix HistCD){
	int *h, *hc, *hcd, i, j, min, imin=0;
	Matrix Resultat;
	Resultat=MatCAlloc(Int,256,1);
	hc=*MatGetInt(HistC);
	hcd=*MatGetInt(HistCD);
	h=*MatGetInt(Resultat);
	for (i=0;i<256;i++){
		min = abs(hc[0]-hcd[0]);
		for(j=0;j<i;j++){
			if(min > abs(hc[i]-hcd[j])){
				min = abs(hc[i]-hcd[j]);
				imin = j;
			}
		}
		h[i]= hcd[imin];
	}
	return Resultat;
}

/*
 * Transformer une matrice en une image
 * Entrees : Matrice de type double
 * Sortie : Image de type GrayLevel
 */
Image Mat2Im(Matrix mat, int opt){
	unsigned char **I;
	int i,j, min, max;
	int lig = MatNbRow(mat);
	int col = MatNbCol(mat);
	double **h;
	Image Resultat;
	if (MatType(mat)!=Double) return NULL;
	Resultat=ImCAlloc(GrayLevel,lig,col);
	h=MatGetDouble(mat);
	I=ImGetI(Resultat);
	min=(int)h[0][0]; max=(int)h[0][0];
	for (j=0;j<col;j++)
		for (i=0;i<lig;i++){
			if (min>(int)h[i][j]) min=(int)h[i][j];
			if (max<(int)h[i][j]) max=(int)h[i][j];
			I[i][j]=(int)h[i][j];
		}
	if (opt == 0) return Resultat;
	for (j=0;j<col;j++)
		for (i=0;i<lig;i++){
			I[i][j]=(255/(max-min))*(I[i][j]-min);
		}
	return Resultat;
}

/*
 * Realiser la convolution d'une image par un masque
 * Entrees : Image (Niveau de gris), Matrice (masque de dimension(n,n) avec n impair), une option (c, b ou n)
 * Sortie : Matrice representant transformee
 */
Matrix Conv(Image Im, Matrix masque, int opt){
	unsigned char **I;
	int i,j,x,y, bord = MatNbRow(masque)/2;
	double **R,**m;
	Matrix Resultat;
	int lig = ImNbRow(Im);
	int col = ImNbCol(Im);
	
	if (ImType(Im)!=GrayLevel) return NULL;
	Resultat=MatCAlloc(Double,lig,col);
	I=ImGetI(Im);
	m=MatGetDouble(masque);
	R=MatGetDouble(Resultat);
	for (j=0;j<col;j++){
		for (i=0;i<lig;i=((j>bord && j<col-bord) && i==bord-1) ? i+lig-2*bord+1: i+1){
			R[i][j]=(opt==-1) ? I[i][j] : opt;
		}
	}
	for (j=bord;j<col-bord;j++)
		for (i=bord;i<lig-bord;i++){
			R[i][j]=0;
			for (y=0;y<MatNbCol(masque);y++)
				for (x=0;x<MatNbRow(masque);x++)
					R[i][j]+=m[x][y]*I[i-bord+x][j-bord+y];
		}
	return Resultat;
}

/*
 * Binariser une image en utilisant seuil
 * Entrees : image (niveau de gris), seuil (entier)
 * Sortie : image binarisee
 */
Image Bin(Image Im, int seuil){
	unsigned char **I, **B;
	int i,j;
	Image Resultat;
	
	if (ImType(Im)!=GrayLevel) return NULL;
	Resultat=ImCAlloc(BitMap,ImNbRow(Im),ImNbCol(Im));
	I=ImGetI(Im);
	B=ImGetI(Resultat);
	for (j=0;j<ImNbCol(Im);j++)
		for (i=0;i<ImNbRow(Im);i++)
			B[i][j]= (I[i][j] < seuil) ? 0 : 1;
	return Resultat;
}

/*
 * Calculer le seuil optimal selon Otsu d'une image a partir de son histogramme
 * Entrees : matrice representant l'histogramme d'une image
 * Sortie : le seuil optimal d'Otsu (entier)
*/
int Otsu(Matrix hist){
	int *hc, *h, j, i, max, otsu=0;
	double p1, p2, mu1, mu2, var;
	Matrix hCum = Hist2CumHist(hist);

	h=*MatGetInt(hist);
	hc=*MatGetInt(hCum);

	p1= hc[0]/hc[255];
	p2= 1-(hc[0]/hc[255]);
	mu1= 0;
	mu2= 0;
		for (i=1;i<256;++i){
			mu2= mu2 + h[i]*i;
		}
	mu2= mu2/(hc[255]-hc[0]);
	max =p1*p2*(mu1-mu2)*(mu1-mu2);
	for (i=1;i<256;i++){
		p1= (double)hc[i]/(double)hc[255];
		p2= 1-p1;

		mu1= 0;
		for (j=0;j<=i;++j){
			mu1= mu1 + h[j]*j;
		}
		if (hc[i-1]!=0) mu1=mu1/hc[i-1];

		mu2= h[i]*i;
		for (j=i+1;j<256;++j){
			mu2= mu2 + h[j]*j;
		}
		if ((hc[255]-hc[i-1])!=0) mu2= mu2/(hc[255]-hc[i-1]);
		var= p1*p2*(mu1-mu2)*(mu1-mu2);
		if(max < var){
			max = var;
			otsu = i;
		}
	}
	return otsu;
}

/*
 * Realiser l'erosion d'une image binaire
 * Entrees : Image (Niveau de gris), Matrice (element structurant de dimension(n,n) avec n impair) de type Int
 * Sortie : Image apres erosion
 */
Image Erode(Image Im, Matrix es){
	unsigned char **I, **R;
	int i,j,x,y, bord = MatNbRow(es)/2;
	int **m;
	Image Resultat;
	int lig = ImNbRow(Im);
	int col = ImNbCol(Im);
	int erod;
	
	if (ImType(Im)!=BitMap) return NULL;
	Resultat=ImCAlloc(BitMap,lig,col);
	I=ImGetI(Im);
	m=MatGetInt(es);
	R=ImGetI(Resultat);
	for (j=0;j<col;j++){
		for (i=0;i<lig;i=((j>bord && j<col-bord) && i==bord-1) ? i+lig-2*bord+1: i+1){
			if (I[i][j]==1){
				erod=0;
				for (y=0;y<MatNbCol(es);y++)
					for (x=0;x<MatNbRow(es);x++){
						if (i-bord+x>=0 && i-bord+x<=lig-1 && j-bord+y>=0 && j-bord+y<=col-1)
						if (m[x][y]==1 && I[i-bord+x][j-bord+y]==0 && erod==0) erod=1;
					}
				R[i][j]=(erod) ? 0 : 1;
			} else R[i][j]=0;
		}
	}
	for (j=bord;j<col-bord;j++)
		for (i=bord;i<lig-bord;i++){
			if (I[i][j]==1){
			erod=0;
				for (y=0;y<MatNbCol(es);y++)
					for (x=0;x<MatNbRow(es);x++){
						if (m[x][y]==1 && I[i-bord+x][j-bord+y]==0 && erod==0) erod=1;
					}
				R[i][j]=(erod) ? 0 : 1;
			} else R[i][j]=0;
		}
	return Resultat;
}

/*
 * Realiser la dilatation d'une image binaire
 * Entrees : Image (Niveau de gris), Matrice (element structurant de dimension(n,n) avec n impair) de type Int
 * Sortie : Image apres dilatation
 */
Image Dilate(Image Im, Matrix es){
	unsigned char **I, **R;
	int i,j,x,y, bord = MatNbRow(es)/2;
	int **m;
	Image Resultat;
	int lig = ImNbRow(Im);
	int col = ImNbCol(Im);
	int dilat;
	
	if (ImType(Im)!=BitMap) return NULL;
	Resultat=ImCAlloc(BitMap,lig,col);
	I=ImGetI(Im);
	m=MatGetInt(es);
	R=ImGetI(Resultat);
	for (j=0;j<col;j++){
		for (i=0;i<lig;i=((j>bord && j<col-bord) && i==bord-1) ? i+lig-2*bord+1: i+1){
			if (I[i][j]==0){
				dilat=0;
				for (y=0;y<MatNbCol(es);y++)
					for (x=0;x<MatNbRow(es);x++){
						if (i-bord+x>=0 && i-bord+x<=lig-1 && j-bord+y>=0 && j-bord+y<=col-1)
						if (m[x][y]==1 && I[i-bord+x][j-bord+y]==1 && dilat==0) dilat=1;
					}
				R[i][j]=(dilat) ? 1 : 0;
			} else R[i][j]=1;
		}
	}
	for (j=bord;j<col-bord;j++)
		for (i=bord;i<lig-bord;i++){
			if (I[i][j]==0){
			dilat=0;
				for (y=0;y<MatNbCol(es);y++)
					for (x=0;x<MatNbRow(es);x++){
						if (m[x][y]==1 && I[i-bord+x][j-bord+y]==1 && dilat==0) dilat=1;
					}
				R[i][j]=(dilat) ? 1 : 0;
			} else R[i][j]=1;
		}
	return Resultat;
}


/*CETTE FONCTION N'EST PAS DEMANDEE DANS LE SUJET
  C'EST JUSTE UNE IDEE QUE J'AI EU.. DONC POURQUOI PAS :)
 * Affiche l'image sous forme de text sur le terminal
 * Entrees : Image
*/
void Im2Ascii(Image im){
	int i,j,x,y,moy=0;
	unsigned char **I;
	I=ImGetI(im);
	for (j=14;j<ImNbRow(im);j= j+15){
		for (i=7;i<ImNbCol(im);i= i+8){
			for (x=i;x>i-8;x--){
				for (y=j;y>j-15;y--)
					moy=moy+I[y][x];
			}
			moy = moy/120;
			switch (moy/29){
				case 0:
					printf(" ");
				break;
				case 1:
					printf(".");
				break;
				case 2:
					printf("*");
				break;
				case 3:
					printf(":");
				break;
				case 4:
					printf("o");
				break;
				case 5:
					printf("&");
				break;
				case 6:
					printf("8");
				break;
				case 7:
					printf("#");
				break;
				case 8:
					printf("@");
				break;
				default: printf("%d ",moy);
			}
		}
		printf("\n");
	}
}