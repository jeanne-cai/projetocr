#ifndef MATRICE_H
#define MATRICE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void init_matrice(int length,float **tab);
void random_matrice(int length, int width, float **tab);
void produit_matrice(int line1,int col1,float **mat1,float *mat2,float *matResult);
void somme_arrays(int taille, float *mat1, float *mat2, float *mat3);
void affiche_array(int length, float *arr);
//void affiche_Matrice(int length, int width, int mat[][width]);

#endif

