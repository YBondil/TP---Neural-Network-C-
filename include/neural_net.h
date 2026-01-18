#pragma once
#include <vector>
#include "maths.h"

class NeuralNetwork {

    private :

        int nb_layers_ ;
        int batch_size_;
        Matrix * layers ; 
        Matrix * weights ; 
        Matrix * bias ;
    
    public : 
        NeuralNetwork(int nb_hidden_layers, int* hidden_layers_size) ;
        NeuralNetwork(int nb_hidden_layers, int* hidden_layers_size, int batch_size) ;
        ~NeuralNetwork();

        void initialize_parameters();
        void display() const ;
        void forward(const Matrix& input) ; 
        void backward(const Matrix& target_y, float learning_rate);

        void learn(const Matrix& data, float learning_rate, int epochs, std::string save_path);

        void see_layer(int layer_nb){layers[layer_nb].print();};

        void save_csv(std::string filename);
        void load_from_csv(std::string filename);


        int prediction(const Matrix& image, int actual_label = -1);
};