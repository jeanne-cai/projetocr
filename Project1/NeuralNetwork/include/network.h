#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

typedef struct network Network;
struct network{
	int nbLayer;
	Layer *tabLayer;
	Layer *inputLayer;
	Layer *outputLayer;
};

int init_network(Network *network,int nbLayer, int nbNeuronPerLayer[]);
void free_network(Network *network);
float sigmoid(float z);
void feed_forward(Network *network, float *inputs);

#endif