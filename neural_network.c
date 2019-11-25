#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dataset_controller.h"
#include "neural_network.h"
#include "Vector.c"


// Returns a random value between 0 and 1
#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))
// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE(x) (((float) (x)) / 255.0f)
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
    for(int i = 0; i < LAYERS; i++){
        create_layer(topology[i], topology[i+1], &layer);
        neural_network[i] = layer;
    }
}

// Referencia: https://github.com/AndrewCarterUK/mnist-neural-network-plain-c/blob/master/neural_network.c
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
            sum += PIXEL_SCALE(X[j]) * layer->W[i][j];
        }
        output[i] = sum;
    }
}

void print_array(float * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %f \n",i, array[i]);
    }
}

void neural_network_training_step(layer_t * neural_network, float * X, int * Y, float learning_rate){
    for(int i = 0; i < LAYERS; i++){
        float activation[neural_network[i].neurons];
        regression_function(X, &neural_network[i], activation); 
        softmax_activation_function(activation, neural_network[i].neurons);
        print_array(activation, neural_network[i].neurons);
    }
}

void init(float * X , int * Y){
    print_array(X, 4);
    layer_t neural_network[LAYERS];
    
    float learning_rate = 0.5f;
    /* Topologia
        1ra capa: 784 connexiones y 3 neuronas
        2ra capa: 3 conneciones y 4 neuronas
        3ra capa: 4 conneciones y 8 salidas
    */
    int topology[] = {IMAGE_SIZE, 3, 4, LABELS}; 
    size_t topology_size = ARRAY_SIZE(topology) - 1;

    create_neural_network(neural_network, topology, topology_size);

    for (int i = 0; i < STEPS; i++) {
        // Run one step of gradient descent and calculate the loss
        neural_network_training_step(neural_network, X, Y, learning_rate);

        // Calculate the accuracy using the whole test dataset
        // --

        //printf("Step %04d\tAverage Loss: %.2f\tAccuracy: %.3f\n", i, loss / batch.size, accuracy);
    }
}

int main(int argc, char *argv[]){
    float X[] = {55.5, 123.2, 232.1, 124.0};
    int Y[] = {1};
    init(X, Y);
        
    return 0;
}