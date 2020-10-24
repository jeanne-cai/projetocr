#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include "layer.h"

Layer new_layer(int nbNeuron, Layer *previousLayer){
	Layer layer;
	layer.nbNeuron=nbNeuron;
	layer.previousLayer = previousLayer;

	//dynamic allocation
	layer.valuesNeurons = malloc (nbNeuron * sizeof(float));

	if(previousLayer!=NULL){ //if it is not the input layer
		
		layer.valuesBiases = malloc (nbNeuron * sizeof(float));
		init_biais(nbNeuron,layer.valuesBiases);
		
		//dynamic allocation
		layer.weights = malloc (nbNeuron * sizeof(float*)); //1st dimension
		for(int i =0; i<nbNeuron;i++)
			//2nd dimension
			layer.weights[i] = malloc (previousLayer->nbNeuron*sizeof(float));
		init_weights(nbNeuron,previousLayer->nbNeuron,layer.weights);
		

		//For backpropagation
		layer.z = calloc (nbNeuron, sizeof(float));
		layer.deltaBiases = calloc (nbNeuron, sizeof(float));
		//1st dimension
		layer.deltaWeights = malloc (nbNeuron * sizeof(float*));
		for(int i =0; i<nbNeuron;i++){
			layer.deltaWeights[i] = 
			calloc (previousLayer->nbNeuron, sizeof(float));//2nd dimension
		}
		layer.cost = calloc (nbNeuron, sizeof(float));
	}

	return layer;
}

void init_weights(int l, int c,float **weights)
{
	random_matrice(l,c,weights);
}

void init_biais(int l, float *valuesBiases )
{

	int min = -1, max=1;
	for(int i =0; i<l; i++)
		valuesBiases[i]= min + (max-min)*(rand()/(float)(RAND_MAX));
}
