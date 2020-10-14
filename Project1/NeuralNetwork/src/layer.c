#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include "layer.h"


Layer new_layer(int nbNeuron, Layer *previousLayer){
	Layer layer;
	layer.nbNeuron=nbNeuron;
	layer.previousLayer = previousLayer;

	//ALLOCATION DYNAMIQUE
	layer.valuesBiais = malloc (nbNeuron * sizeof(float));

	if(previousLayer!=NULL){ //si c'est pas le layer input
		//ALLOCATION DYNAMIQUE
		layer.weights = malloc (nbNeuron * sizeof(float*)); //1ere dimension
		for(int i =0; i<nbNeuron;i++)
			layer.weights[i] = malloc (previousLayer->nbNeuron * sizeof(float)); //2eme dimension

		init_weights(nbNeuron,previousLayer->nbNeuron,layer.weights);
	}

	init_biais(nbNeuron,layer.valuesBiais);

	//ALLOCATION DYNAMIQUE
	layer.valuesNeurons = malloc (nbNeuron * sizeof(float));

	//produit_Matrice(nbNeuron,previousLayer->nbNeuron,previousLayer->nbNeuron,1,weights,previousLayer->valuesNeuron,int matResult[][colM2]);
	return layer;
}

void init_weights(int l, int c,float **weights)
{
	random_Matrice(l,c,weights);
}

void init_biais(int l, float *valuesBiais )
{

	int min = -1, max=1;
	for(int i =0; i<l; i++)
		valuesBiais[i]= min + (max-min)*(rand()/(float)(RAND_MAX)); 
}
