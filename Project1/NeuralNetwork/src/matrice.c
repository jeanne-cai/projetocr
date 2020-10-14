#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrice.h"
/*
void init_Matrice(int length, int width, int tab[][width])
{	
	for(int i=0;i <length; i++)
		for(int j=0; j<width;j++)
			tab[i][j]=0;

}*/
void random_Matrice(int length, int width, float **tab)
{
	int min = -1, max=1;
	for(int i =0; i<length; i++)
		for(int j=0;j<width;j++)
			tab[i][j]= min + (max-min)*(rand()/(float)(RAND_MAX)); 

}
/*
void produit_Matrice(int lineM1,int colM1,int lineM2,int colM2,int mat1[][colM1],int mat2[][colM2],int matResult[][colM2])
{
	init_Matrice(lineM1,colM2, matResult);	
        for(int i=0;i<lineM1;i++)
            for(int j=0;j<colM2;j++)
                for(int k=0;k<lineM2;k++)
                	matResult[i][j]+=mat1[i][k]*mat2[k][j];
}
void affiche_Matrice(int length, int width, int mat[][width])
{
	for(int i=0; i<length; i++)
	{
		for(int j=0; j<width; j++){
			printf("%d ",mat[i][j]);
		}printf("\n");
	}
} */ /*
int main()
{
	srand(time(NULL));
	int l1;
	int m1[2][3];
	int m2[3][2];
	int m3[2][2];

	random_Matrice(2,3,m1);
	random_Matrice(3,2,m2);
	
	printf("Matrice n°1:\n");
	affiche_Matrice(2,3,m1);
	printf("\nMatrice n°2:\n");
	affiche_Matrice(3,2,m2);


       	printf("\nProduit matriciel m1*m2 :\n");
	produit_Matrice(2,3,3,2,m1,m2,m3);
	affiche_Matrice(2,2,m3);
	
	return 0;


}*/
