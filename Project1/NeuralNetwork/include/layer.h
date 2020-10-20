#ifndef LAYER_H
#define LAYER_H
typedef struct layer Layer;
struct layer{
	int nbNeuron;
	Layer *previousLayer;
	float **weights;
	float *valuesNeurons;
	float *valuesBiais;

};

Layer new_layer(int nbNeuron, Layer *previousLayer);
void init_weights(int l, int c, float **weights);
void init_biais(int l, float *valuesBiais);

#endif 