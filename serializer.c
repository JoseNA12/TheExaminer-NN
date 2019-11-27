#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "neural_network.h"
  
#define PATH "serialized_data/trained_model.txt"


void serialize(struct layer_t_ input) {
    // open file for writing 
    FILE *outfile = fopen (PATH, "w"); 
    if (outfile == NULL) { 
        fprintf(stderr, "\nError opend file\n"); 
        //exit (1); 
    }
    // write struct to file 
    fwrite (&input, sizeof(input), 1, outfile); 
      
    if(fwrite != 0)  
        printf("The trained model has been saved correctly!\n"); 
    else 
        printf("Error writing trained model file!\n"); 
  
    // close file 
    fclose (outfile);
}

void deserialize(struct layer_t_ input) {
    // Open file for reading 
    FILE *infile = fopen (PATH, "r"); 
    if (infile == NULL) {
        fprintf(stderr, "\nError opening trained model file\n"); 
        exit (1); 
    } 
    
    if (fread(&input, sizeof(input), 1, infile) == 1) { // success
        printf("The trained model has been loaded correctly!\n"); 
    } 
    else {
        printf("Error reading trained model file!\n"); 
    }

    // read file contents till end of file 
    //while(fread(&input, sizeof(input), 1, infile))
  
    // close file 
    fclose (infile); 

}