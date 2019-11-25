#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "dataset_controller.h"

#define _GNU_SOURCE

int read_image_file(char * filename){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char buf[256];
    snprintf(buf, sizeof buf, "%s%s", "images_txt/", filename);
    fp = fopen(buf, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int list_directory_files(){
    DIR *d;
    struct dirent *dir;
    d = opendir("./images_txt");
    if (d){
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
            read_image_file(dir->d_name);
        }
        closedir(d);
    }
    return(0);
}



