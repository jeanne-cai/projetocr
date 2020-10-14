#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "layer.h"
#include "network.h"


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

void free_network(Network *network){
	free(network->tabLayer);
}

