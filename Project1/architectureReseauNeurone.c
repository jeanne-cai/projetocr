#include <stdio.h>
#include <stdlib.h>

//structure neurone
typedef struct Neuron Neuron;
struct Neuron{
	float biais;
};

//structure couche
typedef struct Layer Layer;
struct Layer{
	int nb_neuron;
	struct Neuron n[100];
};
//permet de créer une nouvelle couche
Layer new_layer(int nbNeuron)
{
	/* faire alloc dyn du tableau de neurone */
	Layer newLayer;
	return newLayer;
}
//permet de créer un nouveau neurone
Neuron new_neuron()
{
	Neuron newNeuron;
	return newNeuron;
}
int creation_neural_network(int nbLayer, int nbNeuronByLayer[]){
	/*ALLOCATION DYNAMIQUE */
	Layer* tabLayer= NULL; // tableau où je vais stocker ttes  mes couches
	tabLayer = malloc (nbLayer * sizeof(Layer)); //alloc
	if(tabLayer==NULL)//Vérification
		exit(-1);
	
	for(int i=0;i<nbLayer;i++)
	{
		//création des couches
		tabLayer[i]= new_layer(nbNeuronByLayer[i]);
		tabLayer[i].nb_neuron = nbNeuronByLayer[i];
		printf("Couche n°%d créée qui doit avoir %d neurones\n", i, nbNeuronByLayer[i]);
		//création des neurones par couches
		for(int j=0; j<nbNeuronByLayer[i]; j++){
			tabLayer[i].n[j] = new_neuron();
			printf("neurone n° %d créé de la couche n° %d\n",j,i);
		}

	}
	return 0;
	
}

int main()
{
	int nbLayer;
	printf("Veuillez rentrer le nombre souhaité de couche (layer) :\n");
	scanf("%d",&nbLayer);
	int nbNeuronByLayer[nbLayer];
	for(int i=0;i<nbLayer;i++){
	
		printf("Veuillez rentrer le nombre de de neurone pour la couche n°%d:\n",i);
		scanf("%d",(nbNeuronByLayer+i));
	}
	creation_neural_network(nbLayer,nbNeuronByLayer);
}
