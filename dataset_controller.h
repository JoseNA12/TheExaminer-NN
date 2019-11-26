#ifndef DATASET_CONTROLLER_H_
#define DATASET_CONTROLLER_H_

#include <stdint.h>

#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT
#define LABELS 8

// Lista Enlazada de imgs. Cada img tiene su mapa de pixeles 
typedef struct x_set_t_ {
    double pixel_matrix[IMAGE_SIZE];
    int annotation[LABELS];
} x_set_t;


void list_directory_files(int count_img, x_set_t *set_);
x_set_t *read_pixels_image_file(char * filename);
char read_annotation_image_file(char * filename);

#endif