#include "../include/neural_net.h"

NeuralNetwork::NeuralNetwork(int nb_hidden_layers, int* hidden_layers_sizes){
    
    nb_hidden_layers_ = nb_hidden_layers ;  
    hidden_layers_size_ = hidden_layers_sizes ;

    hidden_layers = new Matrix[nb_hidden_layers];
    weights = new Matrix[nb_hidden_layers+1];
    bias = new Matrix[nb_hidden_layers];

    input_layer_ = Matrix(784, 1);
    output_layer_ = Matrix(10,1);

    //Bias and hidden layers initialization
    bias[0] = Matrix(10,1);
    for(int i = 0; i<nb_hidden_layers; i++){

        hidden_layers[i]=Matrix(*(hidden_layers_sizes + i),1) ;
        bias[i+1] = Matrix(*(hidden_layers_sizes+i),1) ; 
    }
    //Weights initialization
    weights[0] = Matrix(*hidden_layers_sizes, 784)  ;
    for (int i = 1; i< nb_hidden_layers; i++){
        weights[i] = Matrix(*(hidden_layers_sizes+i),*(hidden_layers_sizes+i-1)) ;
    } 
    weights[nb_hidden_layers] = Matrix(10,hidden_layers_sizes[nb_hidden_layers-1]);

}

NeuralNetwork::~NeuralNetwork(){
    delete[] hidden_layers;
    delete[] weights;
    delete[] bias;
}