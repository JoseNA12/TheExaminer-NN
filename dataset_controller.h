#ifndef DATASET_CONTROLLER_H_
#define DATASET_CONTROLLER_H_

#include <stdint.h>

#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT

// Lista Enlazada de imgs. Cada img tiene su mapa de pixeles 
typedef struct image_t_ {
    float pixels[IMAGE_SIZE];
    int label[];
} image_t;


void list_directory_files(int count_img, image_t *images_set, char * datasetType);
image_t * read_pixels_image_file(char * filename, char * datasetType);
char read_label_image_file(char * filename, char * datasetType);

#endif