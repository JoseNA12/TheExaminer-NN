#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "dataset_controller.h"
#include "neural_network.h"
#include "Vector.c"


// Returns a random value between 0 and 1
#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))
// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE(x) (((float) (x)) / 255.0f)
// Get the real used size of an array
#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

#define STEPS 1000
#define BATCH_SIZE 100

void vector_initialize(vector * vec){
    vector_free(vec); 
    vector_init(vec);
}


void create_layer(int n_connections, int n_neurons, layer_t layer){
    double bias[n_neurons];
    double weights[n_connections][n_neurons];
    for (int i = 0; i < n_neurons; i++) {
        bias[i] = RAND_FLOAT();
        for (int j = 0; j < n_connections; j++) {
            weights[i][j] = RAND_FLOAT();
        }
    }

    layer = (layer_t){.b = bias, .W = weights};
}

void create_neural_network(vector * neural_network, int * topology){
    layer_t layer;
    for(int i = 0; i < ARRAY_SIZE(topology) - 1; i++){
        create_layer(topology[i], topology[i+1], &layer);
        vector_add(neural_network, &layer);
    }
}

void softmax_activation_function(vector * activations){
    int i;
    int length = vector_total(*activations);
    double sum, max, elem;

    for (i = 1, max = vector_get(*activations, 0); i < length; i++) {
        if (vector_get(*activations, i) > max) {
            max = vector_get(*activations, i);
        }
    }

    for (i = 0, sum = 0; i < length; i++) {
        vector_set(*activations, i, exp(vector_get(*activations, i) - max));
        sum += vector_get(*activations, i);
    }

    for (i = 0; i < length; i++) {
        elem = vector_get(*activations, i);
        vector_set(*activations, i, elem / sum);
    }
}

vector regression_function (float * X, float * W, float * b){
    vector output;
    vector_initialize(&output); 
    float sum;
    for(int i = 0; i < ARRAY_SIZE(W); i++ ){
        sum = b[i];
        for(int j = 0; j < ARRAY_SIZE(W[i]); j++ ){
            sum += PIXEL_SCALE(X[j]) * W[i][j];
        }
        vector_add(&output, &sum);
    }

    return output;
}

void neural_network_training_step(vector *neural_network, float * X, int * Y, float learning_rate){
    vector output, activation;
    vector_initialize(&activation);
    vector_initialize(&output);
    layer_t *layer;
    for(int i = 0; i < vector_total(neural_network); i++){
        layer = vector_get(neural_network, i);
        activation = regression_function(X, layer->W, layer->b); 
        softmax_activation_function(&activation);
    }
}

void init(float X[IMAGE_SIZE], int Y[LABELS]){
    float learning_rate = 0.5f;
    int topology[] = {IMAGE_SIZE, 3, 4, LABELS};
    vector neural_network;
    vector_initialize(&neural_network); 
    create_neural_network(&neural_network, topology);

    for (int i = 0; i < STEPS; i++) {
        // Initialise a new batch
        // --
        // Run one step of gradient descent and calculate the loss
        neural_network_training_step(&neural_network, X, Y, learning_rate);

        // Calculate the accuracy using the whole test dataset
        // --

        //printf("Step %04d\tAverage Loss: %.2f\tAccuracy: %.3f\n", i, loss / batch.size, accuracy);
    }
}
