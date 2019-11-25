#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "dataset_controller.h"

#define LAYERS 2
#define NEURONS_MAX 10
#define CONN_MAX IMAGE_SIZE

typedef double (*ActivationFunc)(double sum);

// todo dynamic list or linked list

typedef struct layer_t_ {
    float b[NEURONS_MAX];
    float W[NEURONS_MAX][CONN_MAX];
    int neurons;
    int connections;
} layer_t;

typedef struct output {
    double weighted_sum;
    double activated_result;
} output;


#endif