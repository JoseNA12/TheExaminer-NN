#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "dataset_controller.c"
#include "neural_network.h"

// Returns a random value between 0 and 1
#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))
// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE_BLACK_255(x) ((((float) (x)) / 255.0f))
#define PIXEL_SCALE_WHITE_255(x) (abs((((float) (x)) / 255.0f) - 1))
// Get the real used size of an array
#define ARRAY_SIZE(x) ((int)(sizeof(x) / sizeof((x)[0])))

#define STEPS 1000

/*
Esta Red Neuronal esta inspirada en el video de la implementación manual de una NN en Python del canal DotCV
Video en cuestión: https://youtu.be/W8AeOXa_FqU
*/
void create_layer(int n_connections, int n_neurons, layer_t * layer){
    for (int neuron = 0; neuron < n_neurons; neuron++) {
        layer->b[neuron] = RAND_FLOAT();
        for (int connection = 0; connection < n_connections; connection++) {
            layer->W[neuron][connection] = RAND_FLOAT();        
        }
    }

    layer->neurons = n_neurons;
    layer->connections = n_connections ;
}

void create_neural_network(layer_t * neural_network, int * topology, size_t topology_size){
    for(int i = 0; i < topology_size; i++){
        layer_t layer;
        create_layer(topology[i], topology[i+1], &layer);
        neural_network[i] = layer;
    }
 }

/* Autor: AndrewCarter 
   Función de activación sacada de 
   https://github.com/AndrewCarterUK/mnist-neural-network-plain-c/blob/master/neural_network.c
*/
void softmax_activation_function(float * activations, int length){
    int i;
    float sum, max;

    for (i = 1, max = activations[0]; i < length; i++) {
        if (activations[i] > max) {
            max = activations[i];
        }
    }

    for (i = 0, sum = 0; i < length; i++) {
        activations[i] = exp(activations[i] - max);
        sum += activations[i];
    }

    for (i = 0; i < length; i++) {
        activations[i] /= sum;
    }
}

void relu_activation_function(float * activations, int length){
    for (int i = 0; i < length; i++) {
        if(activations[i] < 0){
            activations[i] = 0;
        }
    }
}

/*
Derivada parcial de la funci
*/
void relu_activation_function_derivative(float * activations, int length, float * output){
    for (int i = 0; i < length; i++) {

        if(activations[i] > 0){
            output[i] = 1;
        }

        else if(activations[i] < 0.0){
            output[i] = 0.0;
        }

        else{
            output[i] = 0.0 / 0.0; // Representar NaN 
        }
    }
}

void regression_function (float * X, layer_t * layer, float * output){
    for(int i = 0; i < layer->neurons; i++ ){
        output[i] = layer->b[i];
        for(int j = 0; j < layer->connections; j++ ){
            output[i] += PIXEL_SCALE_BLACK_255(X[j]) * layer->W[i][j];                
        }
    }
}

void print_array(float * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %f \n",i, array[i]);
    }
}

void print_array_Y(int * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %d \n",i, array[i]);
    }
}

float cross_entropy_loss(float * Ypredicted, float * Yreal, size_t vectorSize){
    float sum = 0.0f;
    for(int i = 0; i < vectorSize; i++){
        sum += Yreal[i] * log(Ypredicted[i]);
    }
    return 0.0f - sum;
}

void softmax_and_cross_entropy_loss_derivative(float * Ypredicted, int * Yreal, float * newDelda, int n_neurons){
    for(int i = 0; i < n_neurons; i++){
        newDelda[i] = Ypredicted[i] - Yreal[i];
    }
}

void calculateNewBias(float * bias, float * delta, float lr, size_t biasSize){
    for(int i = 0; i < biasSize; i++){
        delta[i] *= lr;
        bias[i] -= delta[i];
    }
}

void calculateNewWeights(layer_t layer, float * activations, float * deltas, size_t deltSize, float learning_rate){
    float result[layer.neurons];
    for(int i = 0; i < layer.neurons; i++){
        for(int j = 0; j < layer.connections; j++){
            result[i] += activations[i] * deltas[j] * learning_rate;
        }
    }

    for(int i = 0; i < layer.neurons; i++){
        for(int j = 0; j < layer.connections; j++){
            layer.W[i][j] -= result[j];
        }
    }
}

void updateDeldaHiddenLayer(layer_t previousLayer, float * delta, float * activationsDerivative, float * newDelda){
    for(int i = 0; i < previousLayer.neurons; i++){
        for(int j = 0; j < previousLayer.connections; j++){
            newDelda[j] = 0;
            for(int k = 0; k < previousLayer.connections; k++){
                newDelda[j] += delta[k] * previousLayer.W[k][j];
            }
            newDelda[j] *= activationsDerivative[j];
        }
    }
}

/*
Permite el aprendizaje de la red. Calcula función de Coste del resultado predicho con respecto al resultado esperado. A partir de este coste
calcula valores Delta, los cuales representan lo que falta para reducir el coste. Estos valores se utilizan para actualizar y mejorar los
pesos y bias de cada capa.
*/
void neural_network_backpropagation(layer_t * neural_network, float * activations[LAYERS+1], int * Yreal, float learning_rate){
    float * deltas[LAYERS]; 
    layer_t  layer_before_update;
    int activationsCurrentLayer;

     // Ciclo For en reversa para iniciar desde la última capa
    for (unsigned i_layer = LAYERS ; i_layer-- > 0 ; ){

        /* 
            Se suma uno debido a que la matriz de activaciones tiene a X en la pos 0, 
            por lo que las demás salidas tiene un indice de más
        */
        activationsCurrentLayer = i_layer + 1; 
        float newDelta[neural_network[i_layer].connections];
        if(i_layer == LAYERS - 1){ 
            // Calculo de los nuevos Deltas para la Ultima capa
            softmax_and_cross_entropy_loss_derivative(activations[activationsCurrentLayer], Yreal, newDelta, neural_network[i_layer].neurons);   
            deltas[i_layer] = newDelta;
        }
        else{ 
            // Calculo de los nuevos Deltas para las Capas Ocultas
            float activationsDerivative[ARRAY_SIZE(activations[activationsCurrentLayer])]; 
            relu_activation_function_derivative(activations[activationsCurrentLayer], ARRAY_SIZE(activations[i_layer]), activationsDerivative);
            updateDeldaHiddenLayer(layer_before_update, deltas[i_layer+1], activationsDerivative, newDelta);
            deltas[i_layer] = newDelta;
        }

        /* Guardar los pesos de la capa anterior (La más adelante ya que va en reversa) ya que estos van a ser modificados
        y se necesita guardar los pesos sin modificar */
        layer_before_update = neural_network[i_layer];

        // Gradient Descent. Actualiza los pesos y el bias a partir de los deltas y activaciones calculadas
        calculateNewBias(neural_network[i_layer].b, deltas[i_layer], learning_rate, LABELS);   
        calculateNewWeights(neural_network[i_layer], activations[i_layer], deltas[i_layer], ARRAY_SIZE(deltas[i_layer]),learning_rate);
    }
}

/*
Realiza el feed forward de la NN. Calcula la suma ponderada de cada neurona, así como la función de activación softmax. 
Las activaciones se concentran en la activations_matrix que almacena las entradas y salidas de cada capa. 
En caso de que el valor booleano train esté True, se realiza el backpropagation con el fin de mejorar los pesos de la NN.
*/
void neural_network_training_step(layer_t * neural_network, float * X, int * Y, float learning_rate, int train){
    float * activations_matrix[LAYERS+1]; // Se colocan todas las entradas y salidas de cada capa.
    activations_matrix[0] = X;
    float activation[IMAGE_SIZE];
    for(int i = 0; i < LAYERS; i++){
        
        memset(activation, 0, sizeof(activation));

        regression_function(activations_matrix[i], &neural_network[i], activation); 
        printf("Reg F \n");
        print_array(activation, neural_network[i].neurons);
        if (i == LAYERS - 1){
            softmax_activation_function(activation, neural_network[i].neurons);
            printf("Softmax\n");
            print_array(activation, neural_network[i].neurons);
        }
        else{
            relu_activation_function(activation, neural_network[i].neurons);
            printf("RELU\n");
            print_array(activation, neural_network[i].neurons);
        }
        activations_matrix[i+1] = activation;
    }

    printf("Vector de Salida \n");
    print_array(activations_matrix[LAYERS], neural_network[LAYERS-1].neurons);
    printf("Vector de Salida Esperado \n");
    print_array_Y(Y, neural_network[LAYERS-1].neurons);

    if (train){
        neural_network_backpropagation(neural_network, activations_matrix, Y, learning_rate);
    }
}

/*
Inicializa el dataset de imágenes, la tología de NN y la crea. Además empieza el procesamiento del dataset. 
*/
void start(int count_img, char * datasetType) { 
    float learning_rate = 0.5f;
    int topology[] = {IMAGE_SIZE, 8, 10}; // Topología de 784 conexiones, 8 neuronas, 8 conexiones y 10 neuronas. 
    layer_t neural_network[LAYERS];
    image_t * images_set = malloc(count_img * sizeof(image_t)); 

    list_directory_files(count_img, images_set);
    create_neural_network(neural_network, topology, ARRAY_SIZE(topology) - 1);
    for (int i = 0; i < STEPS; i++) {
        for (int i = 0; i < count_img; i++) {
            neural_network_training_step(neural_network, images_set[i].pixels, images_set[i].label, learning_rate, 1);
        }
    }
}
