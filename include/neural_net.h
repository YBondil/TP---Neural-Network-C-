#pragma once
#include <vector>
#include "maths.h"

class NeuralNetwork {

    private :
        int nb_hidden_layers_ ;
        int* hidden_layers_size_ ;

        Matrix * hidden_layers ; 
        Matrix * weights ;
        Matrix * bias ; 
        
        Matrix input_layer_;
        Matrix output_layer_; 
    
    public : 
        NeuralNetwork(int nb_hidden_layers, int* hidden_layers_size) ;
        ~NeuralNetwork();
        

        



};