#include <iostream>
#include <fstream>
#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"



int main() {
    //Parameters
    int layers_config[] = {784, 128, 10};
    int nb_epoch = 15;
    int batch_size = 16 ;
    float learning_rate = 0.1f;


    NeuralNetwork net(4, layers_config, batch_size); 
   
    std::cout << "Save file ?" << std::endl;
    std::string model_path = "trained_model.csv" ;
    std::cin>>model_path;
    std::ifstream model_file(model_path);

    if (model_file.good()) {
        model_file.close();
        std::cout << "--- MODEL FOUND ---" << std::endl;
        net.load_from_csv(model_path);
    } else {
            std::cout << "--- NO MODEL FOUND : BEGINNING OF LEARNING ---" << std::endl;
        
            Matrix<float> train_data = CSVReader::readAsMatrix("data/Mnist/train.csv", ',');
        
            net.learn(train_data, learning_rate, nb_epoch, model_path);
            std::cout << "Leaning ended model saved in " << model_path << std::endl;
            std::cout << "Loading data/Mnist/test.csv..." << std::endl;
    }

    std::cout << "Loading data/Mnist/test.csv..." << std::endl;
    Matrix<float> test_data = CSVReader::readAsMatrix("data/Mnist/test.csv", ',');
    std::ofstream error_file("test_python/errors.csv");
    
    int correct = 0;
    int errors_count = 0;

    for (int i = 0; i < test_data.get_rows(); i++) {
        Matrix<float> row = test_data.sub_row(i);
        int label = static_cast<int>(row(0, 0));

        Matrix<float> image(784, 1);
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

    std::cout << "Do you want to test the network on some examples ? [Y/n]"<< std::endl ;
    std::string answer ;
    std::cin >> answer ;

    if (answer == "Y"||answer == "y"||answer == "yes"||answer == "Yes"){
        while(true){
            std::cout << "Which index ? (0~9999, -1 to stop)"<< std::endl ;
            int idx ;
            std::cin >> idx ; 
            if (idx == -1){break;};
            Matrix<float> row = test_data.sub_row(idx);
            int label = row(0,0) ;
            Matrix<float> image(784, 1);
            for (int j = 0; j < 784; j++) {
                image(j, 0) = row(0, j + 1) / 255.0f;
                }
            CSVReader::digit digit = CSVReader::matrix_to_digit(image,label);
            digit.visualize();
            std::cout<< "Network prediction : "<< net.prediction(image)<<std::endl ;
         }
        }

    return 0;
}