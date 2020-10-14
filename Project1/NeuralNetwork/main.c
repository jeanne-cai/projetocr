#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrice.h"
#include "layer.h"
#include "network.h"

void print_network(Network network){
	printf("La couche 0 a un nombre de neurones = à %d \nValeurs biais par neurones: ",network.tabLayer[0].nbNeuron);
	for(int i=0; i<network.tabLayer[0].nbNeuron; i++)
		printf("%f ", network.tabLayer[0].valuesBiais[i]);
	printf("\n\n");
	for(int j=1; j<network.nbLayer; j++){
		printf("La couche %d a un nombre de neurones = à %d \nValeurs biais par neurones: ",j,network.tabLayer[j].nbNeuron);
		for(int k=0; k<network.tabLayer[j].nbNeuron; k++){
			printf("%f ", network.tabLayer[j].valuesBiais[k]);
		}
		printf("\nValeurs des poids :\n");
		for(int l=0 ; l<network.tabLayer[j].nbNeuron; l++)
		{
			for(int m=0; m<network.tabLayer[j].previousLayer->nbNeuron;m++)
			{
				printf("%f ",network.tabLayer[j].weights[l][m]);
			}printf("\n");
		}
		printf("\n");
	}
}
	

int main()
{
	srand(time(NULL));
	int nbCouches=3;
	int tab[3] = {2,2,1};
	Network network;
	init_network(&network, nbCouches, tab );

	print_network(network);

	return 0;
}
