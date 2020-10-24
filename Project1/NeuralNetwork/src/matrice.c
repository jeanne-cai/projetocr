#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrice.h"
#include <math.h>

void init_array(int length,float **tab)
{	
	//ALLOCATION DYNAMIQUE
	*tab = calloc (length ,sizeof(float));
}
void random_matrice(int length, int width, float **tab)
{
	int min = -1, max=1;
	for(int i =0; i<length; i++)
		for(int j=0;j<width;j++)
			tab[i][j]= min + (max-min)*(rand()/(float)(RAND_MAX)); 

}

void produit_matrice(int line1,int col1,float **mat1,float *mat2,float *matResult)
{
	for(int i=0;i<line1;i++){
		for(int k=0;k<col1;k++){
			matResult[i]+=mat1[i][k]*mat2[k];
		}
	}
}
void product_matrix_transposed(int sizeL,int sizeC,float **mat,float *col,float *matResult)
{
	for(int i=0;i<sizeC;i++){
		matResult[i]=0;
		for(int k=0;k<sizeL;k++){
			matResult[i]+=mat[k][i]*col[k];
		}
	}
}
void product_col_line(int sizeC, int sizeL, float *col, float *line,float **matResult)
{
	for(int i=0;i<sizeC;i++){
		for(int k=0;k<sizeL;k++){
			matResult[i][k]+=col[i]*line[k];
		}
	}

}
void somme_arrays(int taille, float *arr1, float *arr2, float *arr3)
{
	for(int i=0; i<taille;i++){
		arr3[i]=arr1[i]+arr2[i];
		
	}

}
void diff_arrays(int taille, float *arr1, float *arr2, float *arr3)
{
	for(int i=0; i<taille;i++){
		arr3[i]=arr1[i]-arr2[i];
		
	}

}
void affiche_Matrice(int length, int width, float **mat)
{
	for(int i=0; i<length; i++)
	{
		for(int j=0; j<width; j++){
			printf("%f ",mat[i][j]);
		}printf("\n");
	}
}  

void affiche_array(int length, float *arr){
	printf("Array :\n");
	for(int j=0; j<length; j++){
		printf("%f ",arr[j]);
	}
	printf("\n");
}
// int main()
// {
// 	srand(time(NULL));
// 	int l1;
// 	int m1[2][3];
// 	int m2[3][2];
// 	int m3[2][2];

// 	random_Matrice(2,3,m1);
// 	random_Matrice(3,2,m2);
	
// 	printf("Matrice n°1:\n");
// 	affiche_Matrice(2,3,m1);
// 	printf("\nMatrice n°2:\n");
// 	affiche_Matrice(3,2,m2);


//        	printf("\nProduit matriciel m1*m2 :\n");
// 	produit_Matrice(2,3,3,2,m1,m2,m3);
// 	affiche_Matrice(2,2,m3);
	
// 	return 0;


// }
