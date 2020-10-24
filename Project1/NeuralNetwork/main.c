#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "layer.h"
#include "network.h"
#include "tools.h"

void print_valuesNeurons(Network network)
{
	printf("================== Input : %f %f ==================\n"
		,network.arrLayer[0].valuesNeurons[0],
		network.arrLayer[0].valuesNeurons[1]);
	for(int i=0; i<network.nbLayer;i++){
		printf("\nLayer n°%d : ",i+1);
		for(int j=0; j<network.arrLayer[i].nbNeuron;j++){
			printf("neuron %d: %f ",j+1,network.arrLayer[i].valuesNeurons[j]);
		}
	}
	printf("\n\n");

}
void new_network(float **datasIn,float **datasOut,int nbLayer,
	int nbNeuron,int *arrLayer,Network *network)
{

	//number of intermediate layer
	printf("Enter desired number of intermediates layers : ");
	secuScanf("%d",&nbLayer);
	nbLayer+=2; // +inputLayer +outputLayer

	//Dynamic allocation
	arrLayer=malloc(nbLayer*sizeof(int));

	//init nb neuron in 
	arrLayer[0]=2;
	arrLayer[nbLayer-1]=1;

	printf("Layer n°1 : 2 neurons\n");
	for(int i=1;i<nbLayer-1;i++){
		printf("Enter desired number of neurons for layer n°%d : ",i+1);
		secuScanf("%d",&nbNeuron);
		arrLayer[i]=nbNeuron;
	}
	printf("Layer n°%d : 1 neuron\n\n",nbLayer);

	
	//initialisation of network
	init_network(network, nbLayer, arrLayer);

	//train network
	train(network, datasIn, datasOut, 10.f, 100000, 4);
}



int main()
{
	//XOR inputs
	float input00[] = {0.f,0.f};
	float input01[] = {0.f,1.f};
	float input10[] = {1.f,0.f};
	float input11[] = {1.f,1.f};

	//XOR outputs
	float out0[] = {0.f};
	float out1[] = {1.f};

	float *datasIn[] = {input00,input01,input10,input11};
	float *datasOut[] = {out0,out1,out1,out0};

	srand(time(NULL));
	int nbLayer=0,nbNeuron=0;
	int *arrLayer=NULL;
	Network network;
	
	//clear the console
	system("clear"); 

	//load previous network?
	char answer;
	printf("Do you want to load previous neural network? (Y/N) : ");
	secuScanf("%c",&answer);
	
	//yes
	if(answer=='y'||answer=='Y')
	{
		load_network(&network,arrLayer);
	}
	//no
	else
	{
		new_network(datasIn,datasOut,nbLayer,nbNeuron,arrLayer,&network);
	}


	//Result of input 00
	feed_forward(&network, input00);
	print_valuesNeurons(network);

	//Results of input 01
	feed_forward(&network, input01);
	print_valuesNeurons(network);
	
	//result of input 10
	feed_forward(&network, input10);
	print_valuesNeurons(network);
	
	//result of input 11
	feed_forward(&network, input11);
	print_valuesNeurons(network);

	
	//save weights and biases
	save_network(network);

	return 0;
}
