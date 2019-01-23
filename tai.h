/*  Karim SALAMA
 */
/* Base: TAI release 2 (25/03/2014) */
#ifndef __tai_h_
#define __tai_h_

#include <stddef.h>
#include "limace.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Fonction a appeler en debut de main pour initialiser la gestion des erreurs */
extern char *InitMesg(char *argv[]);

/* Affichage d'un message formate sur stderr */
extern void Mesg(const char *Msg, ...);

/* Affichage de la syntaxe d'appel d'un operateur sur stderr */
extern void Usage(char *Syntaxe);

/* Affichage d'un message d'erreur sur stderr */
extern void Erreur(char *Msg);

/*
 * Inversion d'une image binaire ou de niveaux de gris
 * Entree : image initiale (en niveaux de gris ou binaire)
 * Sortie : Image resultat
 */
extern Image Inversion(Image Im);

/*
 * Representation d'un histogramme sous forme d'une image
 * Entrees : histogramme et nombre de lignes de l'image resultat
 * Sortie : image resultat
 */
extern Image Hist2Im(Matrix Hist, int NbLig);

/*
 * Appliquer une transformation ponctuelle a une image selon une matrice
 * Entrees : Image (Niveau de gris)
 * Sortie : Image transformee
 */
extern Image AppLut(Image Im, Matrix Mat);

/*
 * Creation d'un histogramme a partir d'une image
 * Entrees : Image source
 * Sortie : Histogramme resultat
 */
extern Matrix Hist(Image Im);

/*
 * Creation de l'histogramme egalise a partir d'une image
 * Entrees : ImageEntree : image (niveau de gris)
 * Sortie : Histogramme : nom du fichier Matrix (MAT) destination
 */
extern Matrix HistEg(Image Im);

/*
 * Calculer l'histogramme cumule a partir d'un autre histogramme
 * Entrees : histogramme
 * Sortie : histogramme cumule
 */
extern Matrix Hist2CumHist(Matrix hist);

/*
 * Calculer la transformation ponctuelle donnee par l'algorithme de specification a partir de 2 histogrammes
 * Entrees : histogramme, histogramme desire
 * Sortie : matrice lut
 */
extern Matrix SpecifHist(Matrix HistC, Matrix HistCD);

/*
 * Transformer une matrice en une image
 * Entrees : Matrice de type double
 * Sortie : Image de type GrayLevel
 */
extern Image Mat2Im(Matrix mat, int opt);

/*
 * Realiser la convolution d'une image par un masque
 * Entrees : Image (Niveau de gris), Matrice (masque de dimension(n,n) avec n impair), une option (c, b ou n)
 * Sortie : Matrice representant transformee
 */
extern Matrix Conv(Image Im, Matrix masque, int opt);

/*
 * Binariser une image en utilisant seuil
 * Entrees : image (niveau de gris), seuil (entier)
 * Sortie : image binarisee
 */
extern Image Bin(Image Im, int seuil);

/*
 * Calculer le seuil optimal selon Otsu d'une image a partir de son histogramme
 * Entrees : matrice representant l'histogramme d'une image
 * Sortie : le seuil optimal d'Otsu (entier)
*/
extern int Otsu(Matrix hist);

/*
 * Realiser l'erosion d'une image binaire
 * Entrees : Image (Niveau de gris), Matrice (element structurant de dimension(n,n) avec n impair) de type Int
 * Sortie : Image apres erosion
 */
extern Image Erode(Image Im, Matrix es);

/*
 * Realiser la dilatation d'une image binaire
 * Entrees : Image (Niveau de gris), Matrice (element structurant de dimension(n,n) avec n impair) de type Int
 * Sortie : Image apres dilatation
 */
extern Image Dilate(Image Im, Matrix es);

/*CETTE FONCTION N'EST PAS DEMANDEE DANS LE SUJET
  C'EST JUSTE UNE IDEE QUE J'AI EU.. DONC POURQUOI PAS :)
 * Afficher l'image sous forme de text sur le terminal
 * Entrees : Image
*/
extern void Im2Ascii(Image im);

#ifdef __cplusplus
}
#endif


#endif /* !__tai_h_ */
