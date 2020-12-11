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
		init_weights(nbNeuron,previousLayer->nbNeuron,layer.weights, previousLayer->nbNeuron);
		

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

void init_weights(int l, int c,float **weights, int nbNeuronsPreviousLayer)
{
	random_matrice(l,c,weights,nbNeuronsPreviousLayer);
}

void init_biais(int l, float *valuesBiases )
{
	for(int i =0; i<l; i++)
		valuesBiases[i]=randn();
}

float randn() {
    double u1, u2, w, mult;
    static double x1, x2;
    static int use_previous = 0;

    if (use_previous == 1) {
        use_previous = !use_previous;
        return x2;
    }

    do {
        u1 = -1 + ((double) rand() / RAND_MAX) * 2;
        u2 = -1 + ((double) rand() / RAND_MAX) * 2;
        w = pow(u1, 2) + pow(u2, 2);
    } while (w >= 1 || w == 0);

    mult = sqrt((-2 * log(w)) / w);
    x1 = u1 * mult;
    x2 = u2 * mult;

    use_previous = !use_previous;

    return x1;
}