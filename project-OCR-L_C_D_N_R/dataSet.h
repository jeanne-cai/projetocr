#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct dataset Dataset;
struct dataset{
    size_t size,input_size,output_size;
    float **inputs;
    float **outputs;
    int size_height;
    int size_width;
};
int dataset_new(Dataset *dataset, size_t size, size_t input_size, size_t output_size);
int recupSize(char str[]);
float strToFloat(char str[]);
void initialise_output(char label, Dataset *training_data,int i);
void initialise_input(char *str,Dataset *training_data, int i);
void dataset_initialise_input_output(FILE* file,Dataset *training_data,int sizeInputs);
void initialiseDataSet(Dataset *training_data,char path_file[]);
void split_dataSet(Dataset *data_set,Dataset *training_data,Dataset *test_data,int percent_of_test);
void dataset_free(Dataset *dataset);
void swap(float **a, float **b);
void dataset_shuffle(Dataset *dataset);
Dataset *generate_batches(Dataset *training_data,size_t nb_batch,size_t mini_batch_size );
char getCodeChar(int code);
int getCharCode(char label);
#endif