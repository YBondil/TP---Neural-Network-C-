#include <iostream>
#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"

int main(){
    int layers[] = {784, 128, 10};
    NeuralNetwork net(3, layers, 2);
    
    Matrix batch = CSVReader::readAsMatrix("data/Mnist/train.csv", ',', 6);
    
    // Extract labels (first column) from the batch
    Matrix labels = batch.sub_col(0);
    Matrix y = labels.transposed().to_label_matrix();
    
    std::cout << "Labels:" << std::endl;
    labels.transposed().print();
    std::cout << "One-hot encoded:" << std::endl;
    y.print();
    
    return 0;
}