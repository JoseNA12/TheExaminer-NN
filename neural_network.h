#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "dataset_controller.h"

#define LAYERS 2

typedef struct layer_t_ {
    float b[LABELS];
    float W[LABELS][IMAGE_SIZE];
    int neurons;
    int connections;
} layer_t;

void create_layer(int n_connections, int n_neurons, layer_t * layer);
void create_neural_network(layer_t * neural_network, int * topology, size_t topology_size);
void softmax_activation_function(float * activations, int length);
void regression_function (float * X, layer_t * layer, float * output);
void print_array(float * array, int length);
float cross_entropy_loss(float * Ypredicted, float * Yreal, size_t vectorSize);
void softmax_and_cross_entropy_loss_derivative(float * Ypredicted, int * Yreal, float * output, int n_neurons);
void calculateNewBias(float * bias, float * delta, float lr, size_t biasSize);
void calculateNewWeights(layer_t layer, float * activations, float * deltas, size_t deltSize, float learning_rate);
void neural_network_backpropagation(layer_t * neural_network, float * activations[LAYERS+1], int * Yreal, float learning_rate);
void neural_network_training_step(layer_t * neural_network, float * X, int * Y, float learning_rate, int train);
void start(int count_img);

#endif