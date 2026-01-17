#include <iostream>
#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"

int main(){
    //std::vector<CSVReader::digit> digits_dev = CSVReader::readDigits("data/Mnist/test.csv", 100) ;
    //digits_dev[99].visualize();
    
    int size [4] = {2,3,1};
    NeuralNetwork net = NeuralNetwork(4,size);

    //net.display() ;
    net.initialize_parameters();
    net.display();
    net.save_csv("test.csv");

    NeuralNetwork see = NeuralNetwork(0,nullptr);
    see.load_from_csv("test.csv");
    see.display();

    
    return 0;
}