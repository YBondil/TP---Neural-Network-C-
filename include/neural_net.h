#pragma once
#include <vector>
#include "maths.h"

class NeuralNetwork {

    private :

        int nb_layers_ ;
        Matrix * layers ; 
        Matrix * weights ; 
        Matrix * bias ;
    
    public : 
        NeuralNetwork(int nb_hidden_layers, int* hidden_layers_size) ;
        ~NeuralNetwork();

        void initialize_parameters();
        void display() const ;
        void forward(const Matrix& input) ; 
};