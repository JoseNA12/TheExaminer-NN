#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "dataset_controller.c"
#include "neural_network.h"
#include "Vector.c"


// Returns a random value between 0 and 1
#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))
// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE(x) (abs(((float) (x)) / 255.0f) - 1)
// Get the real used size of an array
#define ARRAY_SIZE(x) ((int)(sizeof(x) / sizeof((x)[0])))

#define STEPS 1000
#define BATCH_SIZE 100

void vector_initialize(vector * vec){
    vector_free(vec); 
    vector_init(vec);
}

/* 
 1 bias por cada neurona
 pesos 1 por cada conexiones y diferente por cada
*/

void create_layer(int n_connections, int n_neurons, layer_t * layer){
    for (int neuron = 0; neuron < n_neurons; neuron++) {
        layer->b[neuron] = RAND_FLOAT();
        for (int connection = 0; connection < n_connections; connection++) {
            layer->W[neuron][connection] = RAND_FLOAT();
        }
    }
    layer->neurons = n_neurons;
    layer->connections = n_connections ;
}

void create_neural_network(layer_t * neural_network, int * topology, size_t topology_size){
    layer_t layer;
    for(int i = 0; i < topology_size; i++){
        create_layer(topology[i], topology[i+1], &layer);
        neural_network[i] = layer;
    }
}

/* Autor: AndrewCarter 
   Función sacada de  https://github.com/AndrewCarterUK/mnist-neural-network-plain-c/blob/master/neural_network.c
*/
void softmax_activation_function(float * activations, int length){
    int i;
    float sum, max;

    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }

    for (i = 0, sum = 0; i < length; i++) {
        activations[i] = exp(activations[i] - max);
        sum += activations[i];
    }

    for (i = 0; i < length; i++) {
        activations[i] /= sum;
    }
}

void regression_function (float * X, layer_t * layer, float * output){
    float sum;
    for(int i = 0; i < layer->neurons; i++ ){
        sum = layer->b[i];
        for(int j = 0; j < layer->connections; j++ ){
            X[j] = PIXEL_SCALE(X[j]);
            sum += X[j] * layer->W[i][j];
        }
        output[i] = sum;
    }
}

void print_array(float * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %f \n",i, array[i]);
    }
}

float cross_entropy_loss(float * Ypredicted, float * Yreal, size_t vectorSize){
    float sum = 0.0f;
    for(int i = 0; i < vectorSize; i++){
        sum += Yreal[i] * log(Ypredicted[i]);
    }
    return 0.0f - sum;
}

void cross_entropy_loss_derivative(float * Ypredicted, int * Yreal, float * output){
    for(int i = 0; i < LABELS; i++){
        output[i] = Ypredicted[i] - Yreal[i];
    }
}

void calculateNewBias(float * bias, float * delta, float lr, size_t biasSize){
    for(int i = 0; i < biasSize; i++){
        delta[i] *= lr;
        bias[i] -= delta[i];
    }
}

void calculateNewWeights(layer_t layer, float * activations, float * deltas, size_t deltSize, float learning_rate){
    float result[IMAGE_SIZE];
    for(int i = 0; i < layer.neurons; i++){
        for(int j = 0; j < deltSize; j++){
            result[i] += activations[i] * deltas[j] * learning_rate;
        }
    }

    for(int i = 0; i < LABELS; i++){
        for(int j = 0; j < IMAGE_SIZE; j++){
            layer.W[i][j] -= result[j];
        }
    }
}

void neural_network_backpropagation(layer_t * neural_network, float * activations[LAYERS+1], int * Yreal, float learning_rate){
    float deltas[LAYERS][IMAGE_SIZE]; 
    for (unsigned layer = LAYERS ; layer-- > 0 ; ){ // Reversed loop
        if(layer == LAYERS - 1){ // Ultima capa
            int previousLayer = layer + 1; 
            cross_entropy_loss_derivative(activations[previousLayer], Yreal, deltas[layer]);
        }else{
            printf("Capas ocultas que no hay lol");
        }

        // Gradient Descent
        calculateNewBias(neural_network[layer].b, deltas[layer], learning_rate, LABELS);   
        calculateNewWeights(neural_network[layer], activations[layer], deltas[layer], ARRAY_SIZE(deltas[layer]),learning_rate);
    }
    printf("Backprop \n");
    print_array(activations[1], 8);
}

void neural_network_training_step(layer_t * neural_network, float * X, int * Y, float learning_rate, int train){
    float * activations_array[LAYERS+1];
    activations_array[0] = X;
    for(int i = 0; i < LAYERS; i++){
        float activation[neural_network[i].neurons];
        regression_function(activations_array[i], &neural_network[i], activation); 
        softmax_activation_function(activation, neural_network[i].neurons);
        activations_array[i+1] = activation;
    }

    printf("Feed F \n");
    print_array(activations_array[1], 8);

    if (train){
        neural_network_backpropagation(neural_network, activations_array, Y, learning_rate);
    }

}

void init(float * X , int * Y){
    layer_t neural_network[LAYERS];
    
    float learning_rate = 0.5f;

    /* Topologia
        1ra capa: 784 connexiones y 3 neuronas
    */
    int topology[] = {IMAGE_SIZE, 8}; 
    size_t topology_size = ARRAY_SIZE(topology) - 1;
    create_neural_network(neural_network, topology, topology_size);

    for (int i = 0; i < STEPS; i++) {
        neural_network_training_step(neural_network, X, Y, learning_rate, 1);
    }
}


void start(int count_img) { // recibo desde Python la cantidad de images procesadas
    x_set_t set_[count_img];

    list_directory_files(count_img, set_);

    // AQUI

    for (int i = 0; i < count_img; i++) {
        //printf("Pixel retrieved: %lf\n", set_[i].pixel_matrix[4]);
        printf("{%d %d %d %d %d %d %d %d}\n", 
            set_[i].annotation[0], set_[i].annotation[1], set_[i].annotation[2], set_[i].annotation[3],
            set_[i].annotation[4], set_[i].annotation[5], set_[i].annotation[6], set_[i].annotation[7]
            );
            printf("Pixel de matriz: %lf\n", set_[i].pixel_matrix[0]);
    }

}


// int main(int argc, char *argv[]){
//     float X[] = {
//         252.0, 255.0, 255.0
//     };
//     int Y[] = {1,0,0,0,0,0,0,0};
//     init(X, Y);
        
//     return 0;
// }