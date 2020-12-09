#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "layer.h"
#include "network.h"
#include "matrice.h"
#include "tools.h"
#include "dataSet.h"
#include <math.h>
#include <string.h>

extern int NB_DATA;

int dataset_new(Dataset *dataset, size_t size, size_t input_size, size_t output_size) {
  dataset->size = size;
  dataset->input_size = input_size;
  dataset->output_size = output_size;

  dataset->inputs = malloc(size*sizeof(float*));
  for(size_t i=0; i<size; i++){
    dataset->inputs[i] = calloc(input_size, sizeof(float));
  }

  dataset->outputs= malloc(size* sizeof(float*));
  for(size_t i=0; i<size; i++){
    dataset->outputs[i] = calloc(output_size, sizeof(float));
  }
  return 1;
}
int recupSize(char str[])
{
  char c=str[0];
  int result=0,result2=0;
  int i=1;
  for(;  c!=',';i++)
  {
    result*=10;
    result+=(int)c - (int)'0';
    c=str[i];
  }
  c=str[i];
  for(int j=i+1;c!='\n'&& c!=';';j++)
  {

    result2*=10;
    result2+=(int)c - (int)'0';
    c=str[j];
  }
  return result*result2;
}
float strToFloat(char str[])
{
  int result=0,i=0;
  char c=str[i];
  while(c!='\0')
  {
    result*=10;
    result+=c-'0';
    i++;
    c=str[i];
  }
  return (float) result;
}
char getCodeChar(int code)
{
  char character='-';

  if(code>=0 && code<=25)
    return (char) (code+'A');
    
  if(code>=26 && code<=51)
    return (char)(code+'a'-26);
  if(code>='0' && code<='9')
    return (char)(code+'0'-52);
    
  switch(code)
  {
    case 63: return '?';
    case 64: return '!';
    case 65: return ';';
    case 66: return ',';
    case 67: return '.';
    case 68: return ':';
    case 69: return '\'';
    case 70: return '(';
    case 71: return ')';  
  }
  return character;
}

int getCharCode(char label)
{
  int num=62;

  if(label>='A' && label<='Z')
    return label-'A';
    
  if(label>='a' && label<='z')
    return label-'a'+26;
  if(label>='0' && label<='9')
    return label-'0'+52;

    
  switch(label)
  {
    case '?': return 63;
    case '!': return 64;
    case ';': return 65;
    case ',': return 66;
    case '.': return 67;
    case ':': return 68;
    case '\'': return 69;
    case '(': return 70;
    case ')': return 71;  
  }
  return num;
}
void initialise_output(char label, Dataset *training_data,int i)
{
  training_data->outputs[i][getCharCode(label)]=1.f;  
}
void initialise_input(char *str,Dataset *training_data, int i)
{
  char *inputc;
  int input;

  inputc = strtok(str, ",");
  for(unsigned int j=0;j<training_data->input_size;j++)
  {
    // printf("%d input : %s\n",j, inputc);
    input=strToFloat(inputc);
    training_data->inputs[i][j]=input/255.f*0.99+0.01;  
    inputc = strtok(NULL, ",\n");
  }

/*
  for(unsigned int j=1;j<training_data->input_size;j++)
  {
    inputc=strtok(str,",");
    input=strToFloat(inputc);
    training_data->inputs[i][j]=input;
  }*/
}
void dataset_initialise_input_output(FILE* file,Dataset *training_data,int sizeInputs)
{
  int sizeMaxLine=sizeInputs*4+2;
  char label;
  char str[sizeMaxLine];
  //printf("ayayayeuh\n");
  for(int i=0;i<NB_DATA;i++)
  {
    //printf("oulah ok : i=%d\n",i);
    fgets(str,sizeMaxLine,file);
    //printf("prout\n");
    label=str[0];
    initialise_output(label, training_data,i);
    //printf("output okkay bb\n");
    //on skip le label et la virgule
    //printf("jai peur lol\n");
    initialise_input(&str[2],training_data,i); 
    //printf("INput okkay bb\n");

  }

}
void initialiseDataSet(Dataset *training_data,char path_file[])
{
  int sizeMax=8,sizeInputs,sizeOutputs=72;
  char str[8]="";
  
  //ouverture fichier
  FILE* file=NULL;
  file = fopen(path_file,"r");
  if(file==NULL)
    exit(-1);
  

  //recupération première ligne (taille)
  if(fgets(str,sizeMax,file)==NULL)
    exit(-1);
  sizeInputs= recupSize(str);

  //création du dataset
  dataset_new(training_data, NB_DATA, sizeInputs, sizeOutputs);

  //chargement des inputs
  dataset_initialise_input_output(file,training_data,sizeInputs);

  fclose(file);

}
Dataset *generate_batches(Dataset *training_data,size_t nb_batch,size_t mini_batch_size )
{
  Dataset *batches = calloc(nb_batch, sizeof(Dataset));
    if(batches == NULL) exit(-1);

  
  for(size_t i =0;i<nb_batch;i++)
  {
    dataset_new(&batches[i], mini_batch_size, 3, 2) ;
    for(size_t j =0;j<mini_batch_size;j++)
    {
      batches[i].inputs[j]=training_data->inputs[i*mini_batch_size+j];
      batches[i].outputs[j]=training_data->outputs[i*mini_batch_size+j];
  
    }
  }
  return batches;
}
void swap(float **a, float **b){
  float *temp = *a;
  *a = *b;
  *b = temp;
  //printf("ayaya\n");
}
void dataset_shuffle(Dataset *dataset){
  for (int i = dataset->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&dataset->inputs[i],&dataset->inputs[j]);
        swap(&dataset->outputs[i],&dataset->outputs[j]);
      //printf("8fois\n");
    }
}

void dataset_free(Dataset *dataset) {
    if(dataset->size == 0) return;

    for (size_t i = 0; i < dataset->size; i++) {
      for (size_t j = 0; j < dataset->input_size; j++) {
        free(dataset->inputs[i]);
      }
      free(dataset->inputs);

      for (size_t j = 0; j < dataset->output_size; j++) {
        free(dataset->outputs[i]);
      }
      free(dataset->outputs);
    }
    free(dataset->outputs);

    dataset->size = 0;
}
