#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "dataset_controller.h"
#include <unistd.h>

#define _GNU_SOURCE
#define ARRAY_SIZE(x) ((int)(sizeof(x) / sizeof((x)[0])))
/*
    Se encarga de leer el conjunto de archivos de texto que contienen los pixeles
    de las respectivas imagenes del dataset.
    Se crea un struct donde almacenará los valores de los pixeles.
*/

void print_arrayxd(float * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %f \n",i, array[i]);
    }
}

void cleanArray(float * array, int length){
    for(int i = 0; i < length; i++){
        array[i] = 0.0;
    }
}

void cleanArrayInt(int * array, int length){
    for(int i = 0; i < length; i++){
        array[i] = 0;
    }
}

image_t *read_pixels_image_file(char * filename, char * datasetType) {
    image_t *set = malloc(sizeof(image_t));
    cleanArray(set->pixels, IMAGE_SIZE);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buffer[256];

    char path[100];
    if(strcmp(datasetType, "numbers") == 0){
        strcpy(path, "./dataset/pixels/numbers/");
    }
    else{
        strcpy(path, "./dataset/pixels/letters/");
    }

    snprintf(buffer, sizeof(buffer), "%s%s", path, filename);
    fp = fopen(buffer, "r");
    
    if (fp == NULL) {
        printf("exit(EXIT_FAILURE);");
        return NULL;
    }
   
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        set->pixels[i] = abs(atof(line));
        i++;
    }
    
    fclose(fp);

    if (line)
        free(line);
    
    return set;
}

/*
    Lee de un archivo de texto las respectivas anotaciones/labels de las imagenes
*/
char read_label_image_file(char * filename, char * datasetType) {
    char letter;
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buffer[256];
    char path[100];

    if(strcmp(datasetType, "numbers")==0){
        strcpy(path, "./dataset/annotations/numbers/");
    }
    else{
        strcpy(path, "./dataset/annotations/letters/");
    }

    snprintf(buffer, sizeof buffer, "%s%s", path, filename);
    fp = fopen(buffer, "r");
    
    if (fp == NULL) {
        printf("exit(EXIT_FAILURE);");
        return '\0';
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        letter = line[0];
    }

    fclose(fp);

    if (line)
        free(line);
    
    return letter;
}

/*
    Se encarga de cargar todo el conjunto de archivos de texto que contienen los pixeles
    ... de las respectivas imagenes del dataset .
    Se crea una matriz de structs 'image_t', donde cada struct almacenará 
    ... los valores de los pixeles. Además, se crea el respectivo etiquetado para cada label
*/
// count_img, images_set, datasetType
void list_directory_files(int count_img, image_t *images_set, char * datasetType) {
    DIR *d;
    struct dirent *dir;
    int i = 0;
    char path[100];
    if(strcmp(datasetType, "numbers")==0){
        strcpy(path, "./dataset/pixels/numbers/");
    }
    else{
        strcpy(path, "./dataset/pixels/letters/");
    }
    
    d = opendir(path);
    if (d){
        while (((dir = readdir(d)) != NULL) && (i < count_img)) {
            images_set[i] = *read_pixels_image_file(dir->d_name, datasetType);
            cleanArrayInt(images_set[i].label, 8);
            char letter = read_label_image_file(dir->d_name, datasetType); 
            if(strcmp(datasetType, "numbers") == 0){
                if (letter == 'A') { // 65
                    int lb[] = {1, 0, 0, 0, 0, 0, 0, 0};
                    memcpy(images_set[i].label, lb, sizeof(lb));
                }
                else {
                    if (letter == 'B') { // 66
                        int lb[] = {0, 1, 0, 0, 0, 0, 0, 0};
                        memcpy(images_set[i].label, lb, sizeof(lb));
                    }
                    else {
                        if (letter == 'C') { // 67
                            int lb[] = {0, 0, 1, 0, 0, 0, 0, 0};
                            memcpy(images_set[i].label, lb, sizeof(lb));
                        }
                        else {
                            if (letter == 'D') { // 68
                                int lb[] = {0, 0, 0, 1, 0, 0, 0, 0};
                                memcpy(images_set[i].label, lb, sizeof(lb));
                            }
                            else {
                                if (letter == 'E') { // 69
                                    int lb[] = {0, 0, 0, 0, 1, 0, 0, 0};
                                    memcpy(images_set[i].label, lb, sizeof(lb));
                                }
                                else {
                                    if (letter == 'F') { // 70
                                        int lb[] = {0, 0, 0, 0, 0, 1, 0, 0};
                                        memcpy(images_set[i].label, lb, sizeof(lb));
                                    }
                                    else {
                                        if (letter == 'S') { // 83
                                            int lb[] = {0, 0, 0, 0, 0, 0, 1, 0};
                                            memcpy(images_set[i].label, lb, sizeof(lb));
                                        }
                                        else { // X
                                            int lb[] = {0, 0, 0, 0, 0, 0, 0, 1};
                                            memcpy(images_set[i].label, lb, sizeof(lb));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{
                if (letter == '0') { // 65
                    int lb[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                    memcpy(images_set[i].label, lb, sizeof(lb));
                }
                else {
                    if (letter == '1') { // 66
                        int lb[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
                        memcpy(images_set[i].label, lb, sizeof(lb));
                    }
                    else {
                        if (letter == '2') { // 67
                            int lb[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
                            memcpy(images_set[i].label, lb, sizeof(lb));
                        }
                        else {
                            if (letter == '3') { // 68
                                int lb[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
                                memcpy(images_set[i].label, lb, sizeof(lb));
                            }
                            else {
                                if (letter == '4') { // 69
                                    int lb[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
                                    memcpy(images_set[i].label, lb, sizeof(lb));
                                }
                                else {
                                    if (letter == '5') { // 70
                                        int lb[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
                                        memcpy(images_set[i].label, lb, sizeof(lb));
                                    }
                                    else {
                                        if (letter == '6') { // 83
                                            int lb[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
                                            memcpy(images_set[i].label, lb, sizeof(lb));
                                        }
                                        else { // X
                                            if (letter == '7') { // 83
                                                int lb[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
                                                memcpy(images_set[i].label, lb, sizeof(lb));
                                            }
                                            else { // X
                                                if (letter == '8') { // 83
                                                    int lb[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
                                                    memcpy(images_set[i].label, lb, sizeof(lb));
                                                }else{
                                                    int lb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
                                                    memcpy(images_set[i].label, lb, sizeof(lb));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
            i++;
        }
        closedir(d);
    }
}


void list_directory_files_single_img(int count_img, image_t * image_set, char * datasetType) {
    DIR *d;
    struct dirent *dir;
    int i = 0;
    char path[100];
    if(strcmp(datasetType, "numbers")==0){
        strcpy(path, "./dataset/pixels_compute/numbers/");
    }
    else{
        strcpy(path, "./dataset/pixels_compute/letters/");
    }
    
    d = opendir(path);
    if (d){
        while (((dir = readdir(d)) != NULL) && (i < count_img)) {
            *image_set = *read_pixels_image_file(dir->d_name, datasetType);
            char letter = read_label_image_file(dir->d_name, datasetType); 
            if(strcmp(datasetType, "numbers") == 0){
                if (letter == 'A') { // 65
                    int lb[] = {1, 0, 0, 0, 0, 0, 0, 0};
                    memcpy(image_set->label, lb, sizeof(lb));
                }
                else {
                    if (letter == 'B') { // 66
                        int lb[] = {0, 1, 0, 0, 0, 0, 0, 0};
                        memcpy(image_set->label, lb, sizeof(lb));
                    }
                    else {
                        if (letter == 'C') { // 67
                            int lb[] = {0, 0, 1, 0, 0, 0, 0, 0};
                            memcpy(image_set->label, lb, sizeof(lb));
                        }
                        else {
                            if (letter == 'D') { // 68
                                int lb[] = {0, 0, 0, 1, 0, 0, 0, 0};
                                memcpy(image_set->label, lb, sizeof(lb));
                            }
                            else {
                                if (letter == 'E') { // 69
                                    int lb[] = {0, 0, 0, 0, 1, 0, 0, 0};
                                    memcpy(image_set->label, lb, sizeof(lb));
                                }
                                else {
                                    if (letter == 'F') { // 70
                                        int lb[] = {0, 0, 0, 0, 0, 1, 0, 0};
                                        memcpy(image_set->label, lb, sizeof(lb));
                                    }
                                    else {
                                        if (letter == 'S') { // 83
                                            int lb[] = {0, 0, 0, 0, 0, 0, 1, 0};
                                            memcpy(image_set->label, lb, sizeof(lb));
                                        }
                                        else { // X
                                            int lb[] = {0, 0, 0, 0, 0, 0, 0, 1};
                                            memcpy(image_set->label, lb, sizeof(lb));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{
                if (letter == '0') { // 65
                    int lb[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                    memcpy(image_set->label, lb, sizeof(lb));
                }
                else {
                    if (letter == '1') { // 66
                        int lb[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
                        memcpy(image_set->label, lb, sizeof(lb));
                    }
                    else {
                        if (letter == '2') { // 67
                            int lb[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
                            memcpy(image_set->label, lb, sizeof(lb));
                        }
                        else {
                            if (letter == '3') { // 68
                                int lb[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
                                memcpy(image_set->label, lb, sizeof(lb));
                            }
                            else {
                                if (letter == '4') { // 69
                                    int lb[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
                                    memcpy(image_set->label, lb, sizeof(lb));
                                }
                                else {
                                    if (letter == '5') { // 70
                                        int lb[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
                                        memcpy(image_set->label, lb, sizeof(lb));
                                    }
                                    else {
                                        if (letter == '6') { // 83
                                            int lb[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
                                            memcpy(image_set->label, lb, sizeof(lb));
                                        }
                                        else { // X
                                            if (letter == '7') { // 83
                                                int lb[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
                                                memcpy(image_set->label, lb, sizeof(lb));
                                            }
                                            else { // X
                                                if (letter == '8') { // 83
                                                    int lb[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
                                                    memcpy(image_set->label, lb, sizeof(lb));
                                                }else{
                                                    int lb[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
                                                    memcpy(image_set->label, lb, sizeof(lb));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
            i++;
        }
        closedir(d);
    }
}