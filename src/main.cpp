#include <iostream>
#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"

int main() {
    int layers_config[] = {784, 128, 10};
    NeuralNetwork net(3, layers_config, 4); 
    net.initialize_parameters();

    
    Matrix raw_data = CSVReader::readAsMatrix("data/Mnist/train.csv", ',', 1000);
    
    
    net.learn(raw_data, 0.1f, 5, "model.csv");

    while(true){
    int test;
        std::cout << "quel test ?" ;
        std::cin >> test ;
        
        Matrix test_row = raw_data.sub_row(test); 
        
        
        Matrix image(784, 1);
        for (int j = 0; j < 784; j++) {
            image(j, 0) = test_row(0, j + 1) / 255.0f; 
        }
        
        
        net.prediction(image);
    }    
    return 0;
}