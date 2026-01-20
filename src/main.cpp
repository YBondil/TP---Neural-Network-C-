#include <iostream>
#include <fstream>
#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"



int main() {
    int layers_config[] = {784, 128, 10};
    NeuralNetwork net(3, layers_config, 4); 
   
    net.initialize_parameters();

    std::string model_path = "trained_model.csv";
    std::ifstream model_file(model_path);

    if (model_file.good()) {
        model_file.close();
        std::cout << "--- MODEL FOUND ---" << std::endl;
        net.load_from_csv(model_path);

std::cout << "Loading data/Mnist/test.csv..." << std::endl;
Matrix test_data = CSVReader::readAsMatrix("data/Mnist/test.csv", ',');

std::ofstream error_file("test_python/errors.csv");
int correct = 0;
int errors_count = 0;

for (int i = 0; i < test_data.get_rows(); i++) {
    Matrix row = test_data.sub_row(i);
    int label = static_cast<int>(row(0, 0));
    
    Matrix image(784, 1);
    for (int j = 0; j < 784; j++) {
        image(j, 0) = row(0, j + 1) / 255.0f;
    }

    int predicted = net.prediction(image);
    if (predicted == label) {
        correct++;
    } else {
        // Sauvegarde de l'erreur 
        error_file << label << "," << predicted;
        for (int j = 0; j < 784; j++) {
            error_file << "," << static_cast<int>(image(j, 0) * 255.0f); // retour format de base 
        }
        error_file << "\n";
        errors_count++;
    }
}
error_file.close();
std::cout << "\nAccuracy: " << (correct * 100.0) / test_data.get_rows() << "%" << std::endl;
std::cout << "Number of errors saved in test_python/errors.csv : " << errors_count << std::endl;

    } else {
        std::cout << "--- NO MODEL FOUND : BEGINNING OF LEARNING ---" << std::endl;
        Matrix train_data = CSVReader::readAsMatrix("data/Mnist/train.csv", ',');
        
        net.learn(train_data, 0.1f, 10, model_path);
        std::cout << "Leaning ended model saved in " << model_path << std::endl;
    }

    return 0;
}