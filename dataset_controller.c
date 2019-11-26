#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "dataset_controller.h"

#define _GNU_SOURCE


x_set_t *read_image_file(char * filename) {
    x_set_t *set = malloc(sizeof(x_set_t));
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buffer[256];
    snprintf(buffer, sizeof buffer, "%s%s", "images_txt/", filename);
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
        set->pixel_matrix[i] = atof(line);
        i++;
    }

    fclose(fp);

    if (line)
        free(line);
    //exit(EXIT_SUCCESS);
    
    return set;
}

int list_directory_files(int count_img) { // recibo desde Python la cantidad de images procesadas
    printf("Python procesó %d imagenes", count_img);
    x_set_t *set_[count_img];
    DIR *d;
    struct dirent *dir;
    d = opendir("./pixels/");
    int i = 0;

    if (d){
        while (((dir = readdir(d)) != NULL) && (i < count_img)) {
            //printf("File: %s\n", dir->d_name);
            set_[i] = read_image_file(dir->d_name);
            //printf("Pixel retrieved: %lf:\n", set_[i]->pixel_matrix[4]);
            free(set_[i]);
            i++;
        }
        closedir(d);
    }
    return(0);
}