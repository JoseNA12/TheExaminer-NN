#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "dataset_controller.h"
#include "neural_network.h"
#include "Vector.c"


// Returns a random value between 0 and 1
#define RAND_FLOAT() (((float) rand()) / ((float) RAND_MAX))
// Convert a pixel value from 0-255 to one from 0 to 1
#define PIXEL_SCALE(x) (abs((((float) (x)) / 255.0f) - 1) )
// Get the real used size of an array
#define ARRAY_SIZE(x) ((int)(sizeof(x) / sizeof((x)[0])))

#define STEPS 1000
#define BATCH_SIZE 100

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
    layer_t layer;
    for(int i = 0; i < topology_size; i++){
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

void regression_function (float * X, layer_t * layer, float * output){
    int value = 1;
    for(int i = 0; i < layer->neurons; i++ ){
        output[i] = layer->b[i];
        for(int j = 0; j < layer->connections; j++ ){
            output[i] += PIXEL_SCALE(X[j]) * layer->W[i][j];                
        }
    }
}

void print_array(float * array, int length){
    for(int i = 0; i < length; i++){
        printf("%d - %f \n",i, array[i]);
    }
}

float cross_entropy_loss(float * Ypredicted, float * Yreal, size_t vectorSize){
    float sum = 0.0f;
    for(int i = 0; i < vectorSize; i++){
        sum += Yreal[i] * log(Ypredicted[i]);
    }
    return 0.0f - sum;
}

void cross_entropy_loss_derivative(float * Ypredicted, int * Yreal, float * output){
    for(int i = 0; i < LABELS; i++){
        output[i] = Ypredicted[i] - Yreal[i];
    }
}

void calculateNewBias(float * bias, float * delta, float lr, size_t biasSize){
    for(int i = 0; i < biasSize; i++){
        delta[i] *= lr;
        bias[i] -= delta[i];
    }
}

void calculateNewWeights(layer_t layer, float * activations, float * deltas, size_t deltSize, float learning_rate){
    float result[IMAGE_SIZE];
    for(int i = 0; i < layer.neurons; i++){
        for(int j = 0; j < deltSize; j++){
            result[i] += activations[i] * deltas[j] * learning_rate;
        }
    }

    for(int i = 0; i < LABELS; i++){
        for(int j = 0; j < IMAGE_SIZE; j++){
            layer.W[i][j] -= result[j];
        }
    }
}

void neural_network_backpropagation(layer_t * neural_network, float * activations[LAYERS+1], int * Yreal, float learning_rate){
    float deltas[LAYERS][IMAGE_SIZE]; 
    for (unsigned layer = LAYERS ; layer-- > 0 ; ){ // Reversed loop
        if(layer == LAYERS - 1){ // Ultima capa
            int previousLayer = layer + 1; 
            cross_entropy_loss_derivative(activations[previousLayer], Yreal, deltas[layer]);
        }else{
            printf("Capas ocultas que no hay lol");
        }

        // Gradient Descent
        calculateNewBias(neural_network[layer].b, deltas[layer], learning_rate, LABELS);   
        calculateNewWeights(neural_network[layer], activations[layer], deltas[layer], ARRAY_SIZE(deltas[layer]),learning_rate);
    }
}

void neural_network_training_step(layer_t * neural_network, float * X, int * Y, float learning_rate, int train){
    float * activations_array[LAYERS+1];
    float activation[LABELS];
    activations_array[0] = X;
    for(int i = 0; i < LAYERS; i++){
        memset(activation, 0, sizeof(activation));
        regression_function(activations_array[i], &neural_network[i], activation); 
        softmax_activation_function(activation, neural_network[i].neurons);
        activations_array[i+1] = activation;
    }

    printf("Vector de Salida \n");
    print_array(activations_array[1], 8);
    if (train){
        neural_network_backpropagation(neural_network, activations_array, Y, learning_rate);
    }
}

/**
 * Calculate the accuracy of the predictions of a neural network on a dataset.

float calculate_accuracy(mnist_dataset_t * dataset, neural_network_t * network){
    float activations[MNIST_LABELS], max_activation;
    int i, j, correct, predict;

    // Loop through the dataset
    for (i = 0, correct = 0; i < dataset->size; i++) {
        // Calculate the activations for each image using the neural network
        neural_network_hypothesis(&dataset->images[i], network, activations);

        // Set predict to the index of the greatest activation
        for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
            if (max_activation < activations[j]) {
                max_activation = activations[j];
                predict = j;
            }
        }

        // Increment the correct count if we predicted the right label
        if (predict == dataset->labels[i]) {
            correct++;
        }
    }

    // Return the percentage we predicted correctly as the accuracy
    return ((float) correct) / ((float) dataset->size);
}
 */

void init(float * X , int * Y){
    layer_t neural_network[LAYERS];
    int topology[] = {IMAGE_SIZE, 8}; // 784 conexiones, 8 neuronas
    size_t topology_size = ARRAY_SIZE(topology) - 1;
    create_neural_network(neural_network, topology, topology_size);
    float learning_rate = 0.5f;
    for (int i = 0; i < 1; i++) {
        neural_network_training_step(neural_network, X, Y, learning_rate, 1);
    }
}

int main(int argc, char *argv[]){
    float X[] = {
252.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
254.0,
255.0,
255.0,
253.0,
255.0,
254.0,
255.0,
255.0,
253.0,
255.0,
253.0,
254.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
254.0,
254.0,
254.0,
254.0,
255.0,
250.0,
254.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
253.0,
255.0,
253.0,
255.0,
253.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
252.0,
255.0,
253.0,
255.0,
255.0,
253.0,
255.0,
253.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
252.0,
253.0,
234.0,
229.0,
250.0,
222.0,
228.0,
252.0,
254.0,
255.0,
255.0,
252.0,
252.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
254.0,
255.0,
254.0,
255.0,
171.0,
35.0,
7.0,
17.0,
25.0,
5.0,
33.0,
65.0,
255.0,
251.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
254.0,
255.0,
255.0,
255.0,
253.0,
88.0,
19.0,
4.0,
0.0,
1.0,
0.0,
0.0,
6.0,
3.0,
101.0,
255.0,
253.0,
253.0,
255.0,
253.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
252.0,
108.0,
8.0,
3.0,
31.0,
36.0,
25.0,
56.0,
106.0,
25.0,
1.0,
14.0,
224.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
250.0,
243.0,
2.0,
0.0,
20.0,
100.0,
255.0,
254.0,
255.0,
255.0,
56.0,
18.0,
0.0,
112.0,
254.0,
254.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
255.0,
253.0,
255.0,
190.0,
1.0,
0.0,
25.0,
139.0,
248.0,
255.0,
255.0,
252.0,
255.0,
254.0,
40.0,
1.0,
18.0,
254.0,
252.0,
255.0,
253.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
255.0,
254.0,
254.0,
255.0,
184.0,
3.0,
6.0,
83.0,
252.0,
255.0,
255.0,
254.0,
255.0,
251.0,
254.0,
89.0,
4.0,
1.0,
129.0,
255.0,
253.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
252.0,
255.0,
252.0,
20.0,
0.0,
44.0,
254.0,
255.0,
255.0,
255.0,
252.0,
255.0,
255.0,
255.0,
210.0,
35.0,
0.0,
31.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
251.0,
255.0,
255.0,
252.0,
77.0,
2.0,
54.0,
156.0,
255.0,
253.0,
252.0,
255.0,
255.0,
252.0,
254.0,
254.0,
255.0,
101.0,
14.0,
0.0,
126.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
253.0,
40.0,
2.0,
9.0,
251.0,
255.0,
252.0,
255.0,
254.0,
251.0,
255.0,
255.0,
255.0,
255.0,
225.0,
11.0,
12.0,
74.0,
255.0,
254.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
252.0,
254.0,
83.0,
16.0,
3.0,
138.0,
253.0,
255.0,
255.0,
254.0,
253.0,
255.0,
255.0,
255.0,
255.0,
252.0,
253.0,
15.0,
1.0,
7.0,
253.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
255.0,
255.0,
255.0,
106.0,
1.0,
17.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
252.0,
255.0,
255.0,
255.0,
62.0,
0.0,
54.0,
148.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
95.0,
6.0,
0.0,
130.0,
251.0,
255.0,
253.0,
255.0,
254.0,
253.0,
255.0,
254.0,
255.0,
254.0,
254.0,
254.0,
215.0,
3.0,
0.0,
108.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
172.0,
8.0,
2.0,
0.0,
30.0,
109.0,
119.0,
106.0,
49.0,
150.0,
139.0,
121.0,
93.0,
132.0,
94.0,
71.0,
79.0,
71.0,
1.0,
4.0,
72.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
204.0,
4.0,
0.0,
4.0,
0.0,
0.0,
1.0,
2.0,
7.0,
11.0,
4.0,
2.0,
6.0,
8.0,
2.0,
13.0,
0.0,
3.0,
5.0,
3.0,
74.0,
252.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
180.0,
15.0,
3.0,
6.0,
37.0,
11.0,
4.0,
29.0,
7.0,
40.0,
5.0,
20.0,
4.0,
12.0,
34.0,
19.0,
22.0,
4.0,
0.0,
2.0,
16.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
252.0,
120.0,
2.0,
7.0,
122.0,
252.0,
238.0,
244.0,
231.0,
228.0,
246.0,
242.0,
233.0,
194.0,
224.0,
129.0,
192.0,
233.0,
123.0,
73.0,
5.0,
6.0,
254.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
118.0,
1.0,
1.0,
245.0,
255.0,
253.0,
255.0,
255.0,
254.0,
254.0,
255.0,
255.0,
254.0,
254.0,
255.0,
253.0,
255.0,
254.0,
95.0,
0.0,
8.0,
175.0,
255.0,
255.0,
255.0,
255.0,
255.0,
253.0,
109.0,
0.0,
4.0,
252.0,
255.0,
255.0,
255.0,
252.0,
252.0,
255.0,
253.0,
252.0,
255.0,
255.0,
253.0,
253.0,
255.0,
255.0,
196.0,
7.0,
19.0,
144.0,
254.0,
255.0,
255.0,
255.0,
252.0,
254.0,
61.0,
1.0,
36.0,
254.0,
255.0,
255.0,
254.0,
255.0,
255.0,
253.0,
255.0,
255.0,
253.0,
251.0,
255.0,
255.0,
254.0,
251.0,
136.0,
9.0,
3.0,
128.0,
254.0,
254.0,
255.0,
255.0,
255.0,
250.0,
29.0,
1.0,
44.0,
253.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
255.0,
254.0,
255.0,
255.0,
255.0,
252.0,
255.0,
255.0,
248.0,
5.0,
1.0,
114.0,
254.0,
254.0,
255.0,
255.0,
255.0,
251.0,
17.0,
1.0,
62.0,
254.0,
252.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
254.0,
253.0,
9.0,
1.0,
108.0,
254.0,
255.0,
255.0,
255.0,
252.0,
255.0,
9.0,
2.0,
25.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
253.0,
255.0,
194.0,
24.0,
3.0,
79.0,
254.0,
255.0,
255.0,
255.0,
255.0,
244.0,
4.0,
4.0,
29.0,
250.0,
255.0,
252.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
253.0,
255.0,
255.0,
248.0,
78.0,
1.0,
2.0,
87.0,
254.0,
255.0,
255.0,
255.0,
255.0,
252.0,
46.0,
1.0,
140.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
255.0,
254.0,
252.0,
239.0,
18.0,
0.0,
4.0,
89.0,
254.0,
255.0,
255.0,
255.0,
    };
    int Y[] = {1,0,0,0,0,0,0,0};
    init(X, Y);
        
    return 0;
}