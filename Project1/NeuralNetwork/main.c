#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "layer.h"
#include "network.h"
#include "dataSet.h"
#include "tools.h"

int NB_DATA=7100;

void print_valuesNeurons(Network network)
{
	printf("================== Input : %f %f %f ==================\n"
		,network.arrLayer[0].valuesNeurons[0],
		network.arrLayer[0].valuesNeurons[1],
		network.arrLayer[0].valuesNeurons[2]);
	for(int i=0; i<network.nbLayer;i++){
		printf("\nLayer n°%d : ",i+1);
		for(int j=0; j<network.arrLayer[i].nbNeuron;j++){
			printf("neuron %d: %f ",j+1,network.arrLayer[i].valuesNeurons[j]);
		}
	}
	printf("\n\n");

}

int main()
{
	srand(time(NULL));
	char path_file[]="dataSetfile.csv";
	Network network;

	Dataset data_set;
	Dataset training_data;
	Dataset test_data;
	int percent_of_test=20;

	initialiseDataSet(&data_set,path_file);
	split_dataSet(&data_set,&training_data,&test_data,percent_of_test);
	printf("training_data  MAIN=%ld",training_data.size);
	/*for(int i =0; i<NB_DATA;i++)
	{
		printf("Input :\n");
		for(size_t j=0; j<28;j++){
			for(size_t k=0; k<28;k++){
				float f = training_data.inputs[i][j*28+k];
				if(f>0.1){
					printf("\033[41m");
				}
				printf("%.1f \033[00m",f);
			}
			printf("\n");
		}
		printf("\n\nOutput : \n");
		for(size_t j=0; j<training_data.output_size;j++)
			printf("%.2f ",training_data.outputs[i][j]);
		printf("\n\n\n\n");
	}
	exit(1);*/

	//load previous network?
	//char answer;
	//printf("Do you want to load previous neural network? (Y/N) : ");
	//secuScanf("%c",&answer);
	
	//yes
	// if(answer=='y'||answer=='Y')
	// {
	// 	load_network(&network);
	// }
	// //no
	// else
	// {
		printf("okguuud\n");
		size_t nbLayer = 3;
		int arrLayer[] = {784,100,72};
		printf("okigoudgoud\n");
		init_network(&network, nbLayer, arrLayer);
		printf("okigoudgoudgouuuuud\n");

		//train network
		sgd(&training_data, 200, 5, 0.3f, &test_data,&network);
		//1000 5 0.3
		printf("oyeahbaby\n");

	//}

	//save weights and biases
	save_network(network);

	return 0;
}
