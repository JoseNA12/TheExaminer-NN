#ifndef DATASET_CONTROLLER_H_
#define DATASET_CONTROLLER_H_

#include <stdint.h>

#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT
#define LABELS 8
#define TOTAL_IMAGES 8

// Lista Enlazada de imgs. Cada img tiene su mapa de pixeles 
typedef struct x_set_t_ {
    double pixel_matrix[IMAGE_SIZE];
} x_set_t;

#endif