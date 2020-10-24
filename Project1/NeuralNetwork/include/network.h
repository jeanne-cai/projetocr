#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

typedef struct network Network;
struct network{
	int nbLayer;
	Layer *arrLayer;
	Layer *inputLayer;
	Layer *outputLayer;
};

int init_network(Network *network,int nbLayer, int nbNeuronPerLayer[]);
void free_network(Network *network);
float sigmoid(float z);
float prime_sigmoid(float z);
void feed_forward(Network *network, float *inputs);
void train(Network *network, float **dataInput, float **expectedResult, float learningRate,long epoch, int nbInputs);
void backpropagation(Network *network, float *dataInput, float *expectedResult);
void _print_network(Network network);
void applyModif(Network *network, float learningRate, int nbInputs);
int is_Save();
void cleanBuffer();
void write_network(Network network,FILE **file);
void save_network(Network network);
void load_network(Network *network,int *arrLayer);
void read_network(Network *network,FILE **file, int *arrLayer);

#endif