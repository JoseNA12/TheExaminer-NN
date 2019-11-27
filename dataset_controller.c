#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "dataset_controller.h"

#define _GNU_SOURCE

/*
    Se encarga de leer el conjunto de archivos de texto que contienen los pixeles
    de las respectivas imagenes del dataset.
    Se crea un struct donde almacenará los valores de los pixeles.
*/
image_t *read_pixels_image_file(char * filename) {
    image_t *set = malloc(sizeof(image_t));
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buffer[256];
    snprintf(buffer, sizeof buffer, "%s%s", "dataset/pixels/", filename);
    fp = fopen(buffer, "r");
    
    if (fp == NULL) {
        printf("exit(EXIT_FAILURE);");
        return NULL;
        //exit(EXIT_FAILURE);
    }

    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%d", i);
        set->pixels[i] = atof(line);
        i++;
    }

    fclose(fp);

    if (line)
        free(line);
    //exit(EXIT_SUCCESS);
    
    return set;
}

/*
    Lee de un archivo de texto las respectivas anotaciones/labels de las imagenes
*/
char read_label_image_file(char * filename) {
    char letter;

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buffer[256];
    snprintf(buffer, sizeof buffer, "%s%s", "dataset/annotations/", filename);
    fp = fopen(buffer, "r");
    
    if (fp == NULL) {
        printf("exit(EXIT_FAILURE);");
        return '\0';
        //exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        letter = line[0];
        //printf("label: %c\n", letter);
    }

    fclose(fp);

    if (line)
        free(line);
    //exit(EXIT_SUCCESS);
    
    return letter;
}

/*
    Se encarga de cargar todo el conjunto de archivos de texto que contienen los pixeles
    ... de las respectivas imagenes del dataset .
    Se crea una matriz de structs 'image_t', donde cada struct almacenará 
    ... los valores de los pixeles. Además, se crea el respectivo etiquetado para cada label
*/
void list_directory_files(int count_img, image_t *images_set, char * datasetType) {
    DIR *d;
    struct dirent *dir;

    if (strcmp(datasetType, str2))
    d = opendir("./dataset/pixels/");
    int i = 0;

    if (d){
            while (((dir = readdir(d)) != NULL) && (i < count_img)) {
                //printf("File: %s\n", dir->d_name);
                images_set[i] = *read_pixels_image_file(dir->d_name);
                char letter = read_label_image_file(dir->d_name); 
                //printf("label: %c\n", letter);

                if (letter == 'A') { // 65
                    int lb[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                    memcpy(images_set[i].label, lb, sizeof(lb));
                }
                else {
                    if (letter == 'B') { // 66
                        int lb[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
                        memcpy(images_set[i].label, lb, sizeof(lb));
                    }
                    else {
                        if (letter == 'C') { // 67
                            int lb[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
                            memcpy(images_set[i].label, lb, sizeof(lb));
                        }
                        else {
                            if (letter == 'D') { // 68
                                int lb[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
                                memcpy(images_set[i].label, lb, sizeof(lb));
                            }
                            else {
                                if (letter == 'E') { // 69
                                    int lb[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
                                    memcpy(images_set[i].label, lb, sizeof(lb));
                                }
                                else {
                                    if (letter == 'F') { // 70
                                        int lb[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
                                        memcpy(images_set[i].label, lb, sizeof(lb));
                                    }
                                    else {
                                        if (letter == 'X') { // 83
                                            int lb[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
                                            memcpy(images_set[i].label, lb, sizeof(lb));
                                        }
                                        else { // X
                                            if (letter == 'S') { // 83
                                                int lb[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
                                                memcpy(images_set[i].label, lb, sizeof(lb));
                                            }
                                            else { // X
                                                if (letter == 'N') { // 83
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
                
                //printf("Pixel retrieved: %lf:\n", images_set[i]->pixels[4]);
                //free(images_set[i]);
                i++;
            }
            closedir(d);
        }
}