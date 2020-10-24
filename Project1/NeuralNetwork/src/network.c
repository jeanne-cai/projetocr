#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "layer.h"
#include "network.h"
#include "matrice.h"
#include "tools.h"
#include <math.h>


int init_network(Network *network,int nbLayer, int nbNeuronPerLayer[]){

	network->nbLayer =nbLayer;

	//ALLOCATION DYNAMIQUE
	network->arrLayer=NULL;
	network->arrLayer = malloc (nbLayer * sizeof(Layer));//alloc
	if(network->arrLayer==NULL) //Vérification
		return -1;

	network->arrLayer[0] = new_layer(nbNeuronPerLayer[0], NULL);
	//initialisation des layers
	for(int i=1; i<nbLayer;i++){
		network->arrLayer[i] = 
		new_layer(nbNeuronPerLayer[i], &(network->arrLayer[i-1]));
	}
	
	network->inputLayer = &network->arrLayer[0];
	network->outputLayer = &network->arrLayer[nbLayer-1];

	return 0;
}
//float sigmoidPrime()
float sigmoid(float z)
{
	return 1.f/(1.f + expf(-z));
}

float sigmoidPrime(float z)
{
	return sigmoid(z)*(1-sigmoid(z));
}
void feed_forward(Network *network, float *inputs)
{
	int nbLayer = network->nbLayer,nbNeuron,nbOldNeuron; 
	float z;
	
	for(int i=0; i<network->arrLayer[0].nbNeuron;i++){
		network->arrLayer[0].valuesNeurons[i] = inputs[i];
	}
	
	for(int i=1; i<nbLayer; i++){
		
		nbNeuron=network->arrLayer[i].nbNeuron;
		nbOldNeuron=network->arrLayer[i-1].nbNeuron;

		produit_matrice(nbNeuron,nbOldNeuron,
			network->arrLayer[i].weights,
			network->arrLayer[i-1].valuesNeurons,
			network->arrLayer[i].valuesNeurons);
		
		somme_arrays(nbNeuron,
			network->arrLayer[i].valuesBiases,
			network->arrLayer[i].valuesNeurons,
			network->arrLayer[i].valuesNeurons);
	
		
		for(int j=0; j<network->arrLayer[i].nbNeuron; j++){
			z=network->arrLayer[i].valuesNeurons[j];
			network->arrLayer[i].z[j]=z;
			network->arrLayer[i].valuesNeurons[j] = sigmoid(z);
		}
	}


}

void backpropagation(Network *network, float *dataInput, float *expectedResult)
{
	//feedforward
	feed_forward(network,dataInput);

	Layer *currentLayer;
	Layer *previousLayer;
	int nbNeuron;
	int nbNeuronPreviousLayer;
	
	// //init cost
	Layer *outputLayer = &network->arrLayer[network->nbLayer-1];
	int nbNeuronOutputLayer = outputLayer->nbNeuron;

	for (int i = 0; i < nbNeuronOutputLayer; ++i)
	{
		outputLayer->cost[i] = 
		(outputLayer->valuesNeurons[i]-expectedResult[i])
						*sigmoidPrime(outputLayer->z[i]);								
	}

	//Backpropagation
	for(int i=network->nbLayer-1; i>0; i--){		
		currentLayer = &network->arrLayer[i];
		previousLayer=currentLayer->previousLayer;
		nbNeuron = currentLayer->nbNeuron;
		nbNeuronPreviousLayer = currentLayer->previousLayer->nbNeuron;

		for(int j=0;j<nbNeuron;j++){
			currentLayer->deltaBiases[j] += currentLayer->cost[j];
		}
		
		product_col_line(nbNeuron, nbNeuronPreviousLayer,
						currentLayer->cost, previousLayer->valuesNeurons,
						currentLayer->deltaWeights);

		if(i>1){
			product_matrix_transposed(nbNeuron,nbNeuronPreviousLayer,
								currentLayer->weights,currentLayer->cost,
								previousLayer->cost);
			for(int i=0; i<nbNeuronPreviousLayer;i++)
			{
				previousLayer->cost[i]*=sigmoidPrime(previousLayer->z[i]);
			}
		}
	}
}

void train(Network *network, float **dataInput, float **expectedResult, 
	float learningRate,long epoch, int nbInputs)
{
	for(long i=0; i<epoch;i++)
	{
		for(int j=0;j<nbInputs;j++){
			backpropagation(network,dataInput[j],expectedResult[j]);
			// _print_network(*network);
		}
		applyModif(network,learningRate,nbInputs);
	}
}

void applyModif(Network *network, float learningRate, int nbInputs)
{
	Layer *layer;
	for (int i = 1; i < network->nbLayer; ++i)
	{
		layer=&network->arrLayer[i];
		for (int j = 0; j < layer->nbNeuron; ++j)
		{
			layer->valuesBiases[j] -= learningRate * 
			layer->deltaBiases[j] / nbInputs;
			layer->deltaBiases[j] = 0.f;

			for (int k = 0; k < layer->previousLayer->nbNeuron; ++k)
			{
				layer->weights[j][k] -= learningRate * 
				layer->deltaWeights[j][k] / nbInputs;
				layer->deltaWeights[j][k] = 0.f;
			}
		}
	}

}

int is_Save()
{
	char answer;
	printf("Do you want to save your weights/biases in a text file? (Y/N) \n");
	secuScanf("%c",&answer);
	if(answer=='N' || answer == 'n')
		return 0;
	else
		return 1;
}

void write_network(Network network,FILE **file)
{
	//write the number of layer
	fprintf(*file,"%d\n",network.nbLayer);
	//write the number of neurons per layer
	for(int i=0; i<network.nbLayer;i++)
		fprintf(*file,"%d\n",network.arrLayer[i].nbNeuron);
	
	//write the biais for each neurons of each layers
	for(int i=1; i<network.nbLayer;i++)
	{
		for(int j=0;j<network.arrLayer[i].nbNeuron;j++)
		{
			fprintf(*file,"%f\n",network.arrLayer[i].valuesBiases[j]);
		}
	}

	//write the weights
	for(int i=1; i<network.nbLayer;i++)
	{
		for(int j=0;j<network.arrLayer[i].nbNeuron;j++){
			for(int k=0;k<network.arrLayer[i-1].nbNeuron;k++)
			{
				fprintf(*file,"%f\n",network.arrLayer[i].weights[j][k]);
			}
		}
	}
}
void save_network(Network network)
{
	if(is_Save()==0)
		return;

	char nameoffile[100];
	printf("Please enter a name for your file : ");
	secuScanf("%s",nameoffile);
	
	
	//open a new folder
	FILE* file=NULL;
	file =fopen(nameoffile,"a");
	if(file == NULL)
	{
		printf("Error : can't open the file %s",nameoffile);
		return;
	}
	//write in the file
	write_network(network,&file);
	
	//free memory
	fclose(file);


}
void read_network(Network *network,FILE **file, int *arrLayer)
{
	int nbLayer;
	
	//write the number of layer
	fscanf(*file,"%d\n",&nbLayer);
	
	//Dynamic allocation
	arrLayer=malloc(nbLayer*sizeof(int));
	
	//write the number of neurons per layer
	for(int i=0; i<nbLayer;i++){
		fscanf(*file,"%d\n",&(arrLayer[i]));
	}

	//initialisation of network
	init_network(network, nbLayer, arrLayer);
	
	//write the biais for each neurons of each layers
	for(int i=1; i<network->nbLayer;i++)
	{
		for(int j=0;j<network->arrLayer[i].nbNeuron;j++)
		{
			fscanf(*file,"%f\n",&(network->arrLayer[i].valuesBiases[j]));
		}
	}

	//write the weights
	for(int i=1; i<network->nbLayer;i++)
	{
		for(int j=0;j<network->arrLayer[i].nbNeuron;j++)
		{
			for(int k=0;k<network->arrLayer[i-1].nbNeuron;k++)
			{
				fscanf(*file,"%f\n",&(network->arrLayer[i].weights[j][k]));
			}
		}
	}
}
void load_network(Network *network,int *arrLayer)
{
	char nameoffile[100];
	printf("Please enter a name for your file : ");

	secuScanf("%s",nameoffile);
	
	//open the folder
	FILE* file=NULL;
	file =fopen(nameoffile,"r");
	if(file == NULL)
	{
		printf("Error : can't open the file %s",nameoffile);
		return;
	}

	//write in the file
	read_network(network,&file,arrLayer);
	
	//free memory
	fclose(file);
}
void free_network(Network *network){
	free(network->arrLayer);
}
/*void print_network(Network network){
	printf("===========================================================\n\n");
	//info layer 0
	printf("COUCHE 0\n\nNeurones: %d\nBiais:\n",network.arrLayer[0].nbNeuron);
	for(int i=0; i<network.arrLayer[0].nbNeuron; i++)
		printf("N%d: %f\n",i, network.arrLayer[0].valuesBiases[i]);
	printf("\n\nPoids: NULL\n\n");
	printf("\nValeurs Neurones :\n");
	for(int k=0; k<network.arrLayer[0].nbNeuron; k++)
	{
				printf("N%d: %f  \n",k,network.arrLayer[0].valuesNeurons[k]);
			
	}
	
	//info autres layers
	for(int j=1; j<network.nbLayer; j++)
	{
		printf("--------------------------------------------------------\n\n");
		printf("COUCHE %d\n\nNeurones: %d\nBiais:\n"
			,j,network.arrLayer[j].nbNeuron);
		
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
			printf("N%d: %f\n",k, network.arrLayer[j].valuesBiases[k]);
		}

		printf("\n\nPoids :\n");
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
			for(int l=0; l<network.arrLayer[j].previousLayer->nbNeuron;l++)
			{
				printf("oN%dN%d: %f  ",l,k,network.arrLayer[j].weights[k][l]);
			}printf("\n");
		}
		printf("\n\nValeurs Neurones :\n");
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
				printf("N%d: %f  \n",k,network.arrLayer[j].valuesNeurons[k]);
			
		}
		printf("\n\nValeurs Neurones :\n");
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
				printf("N%d: %f  \n",k,network.arrLayer[j].valuesNeurons[k]);
			
		}

		printf("\n\nValeurs delta biais :\n");
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
				printf("N%d: %f  \n",k,network.arrLayer[j].deltaBiases[k]);
		}
		printf("\n\nValeurs delta poids : \n");
		for(int k=0; k<network.arrLayer[j].nbNeuron; k++)
		{
			for(int l=0; l<network.arrLayer[j].previousLayer->nbNeuron;l++)
			{
				printf("oN%dN%d: %f  ",l,k,
					network.arrLayer[j].deltaWeights[k][l]);
			}printf("\n");
		}

	}
}*/
