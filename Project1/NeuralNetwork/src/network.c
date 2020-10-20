#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "layer.h"
#include "network.h"
#include "matrice.h"
#include <math.h>


int init_network(Network *network,int nbLayer, int nbNeuronPerLayer[]){

	network->nbLayer =nbLayer;

	//ALLOCATION DYNAMIQUE
	network->tabLayer=NULL;
	network->tabLayer = malloc (nbLayer * sizeof(Layer));//alloc
	if(network->tabLayer==NULL) //Vérification
		return -1;

	network->tabLayer[0] = new_layer(nbNeuronPerLayer[0], NULL);
	//initialisation des layers
	for(int i=1; i<nbLayer;i++){
		network->tabLayer[i] = new_layer(nbNeuronPerLayer[i], &(network->tabLayer[i-1]));
	}
	
	network->inputLayer = &network->tabLayer[0];
	network->outputLayer = &network->tabLayer[nbLayer-1];

	return 0;
}

float sigmoid(float z)
{
	return 1/(1 + exp(-z));
}

void feed_forward(Network *network, float *inputs)
{
	for(int i=0; i<network->tabLayer[0].nbNeuron;i++){
		network->tabLayer[0].valuesNeurons[i] = inputs[i];
	}
	
	for(int i=1; i<network->nbLayer; i++){
		produit_matrice(network->tabLayer[i].nbNeuron, 
			network->tabLayer[i-1].nbNeuron,
			network->tabLayer[i].weights,
			network->tabLayer[i-1].valuesNeurons,
			network->tabLayer[i].valuesNeurons);
		somme_arrays(network->tabLayer[i].nbNeuron,
			network->tabLayer[i].valuesBiais,
			network->tabLayer[i].valuesNeurons,
			network->tabLayer[i].valuesNeurons);

		affiche_array(network->tabLayer[i].nbNeuron,network->tabLayer[i].valuesNeurons);
		
		for(int j=0; j<network->tabLayer[i].nbNeuron; j++){
			network->tabLayer[i].valuesNeurons[j] = sigmoid(network->tabLayer[i].valuesNeurons[j]);
		}
	}


}

void free_network(Network *network){
	free(network->tabLayer);
}

