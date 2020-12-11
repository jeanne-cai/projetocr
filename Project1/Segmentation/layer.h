#ifndef LAYER_H
#define LAYER_H
typedef struct layer Layer;
struct layer{
	int nbNeuron;
	Layer *previousLayer;

	float *valuesNeurons;
	float **weights;
	float *valuesBiases;
	
	float *z;
	float **deltaWeights;
	float *deltaBiases;
	float *cost;
};

Layer new_layer(int nbNeuron, Layer *previousLayer);
void init_weights(int l, int c, float **weights, int nbNeuronsPreviousLayer);
void init_biais(int l, float *valuesBiases);
float randn();

#endif 