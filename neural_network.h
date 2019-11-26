#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include "dataset_controller.h"

#define LAYERS 1
#define NEURONS_MAX 10
#define CONN_MAX IMAGE_SIZE

typedef struct layer_t_ {
    float b[NEURONS_MAX];
    float W[NEURONS_MAX][CONN_MAX];
    int neurons;
    int connections;
} layer_t;

#endif