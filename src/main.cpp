#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

#include "maths.h"
#include "csv_reader.h"
#include "neural_net.h"
#include "exception.h"

int main() {
    try {
        
        int layers_config[] = {784, 512, 10};
        int nb_layers = 3;
        int nb_epoch = 30;
        int batch_size = 64;
        float learning_rate = 0.05f;

        NeuralNetwork net(nb_layers, layers_config, batch_size); 
    
        std::cout << "Enter model filename to load (or create): ";
        std::string model_path = "trained_model.csv";
        std::cin >> model_path;
        
        std::ifstream model_file(model_path);

        if (model_file.good()) {
            model_file.close();
            std::cout << "--- MODEL FOUND : LOADING ---" << std::endl;
            net.load_from_csv(model_path);
        } else {
            std::cout << "--- NO MODEL FOUND : BEGINNING TRAINING ---" << std::endl;
            
            Matrix<float> train_data = CSVReader::readAsMatrix("data/Mnist/train.csv", ',');
        
            net.learn(train_data, learning_rate, nb_epoch, model_path);
            std::cout << "Training ended. Model saved in " << model_path << std::endl;
        }

        std::cout << "Loading test data (data/Mnist/test.csv)..." << std::endl;
        Matrix<float> test_data = CSVReader::readAsMatrix("data/Mnist/test.csv", ',');
        
        std::ofstream error_file("test_python/errors.csv");
        if (!error_file.is_open()) {
             std::cerr << "Warning: Could not create error file at test_python/errors.csv" << std::endl;
        }
        
        int correct = 0;
        int errors_count = 0;

        std::cout << "Running evaluation on " << test_data.get_rows() << " samples..." << std::endl;

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
                if (error_file.is_open()) {
                    error_file << label << "," << predicted;
                    for (int j = 0; j < 784; j++) {
                        error_file << "," << static_cast<int>(image(j, 0) * 255.0f);
                    }
                    error_file << "\n";
                }
                errors_count++;
            }
        }
        if (error_file.is_open()) error_file.close();

        std::cout << "\nAccuracy: " << (correct * 100.0) / test_data.get_rows() << "%" << std::endl;
        std::cout << "Errors saved: " << errors_count << std::endl;



        std::cout << "Do you want to test the network manually? [Y/n]: ";
        std::string answer;
        std::cin >> answer;

        if (answer == "Y" || answer == "y" || answer == "yes") {
            while (true) {
                std::cout << "Enter index (0 to " << test_data.get_rows() - 1 << ", -1 to stop): ";
                int idx;
                if (!(std::cin >> idx)) { 
                    std::cin.clear(); 
                    std::string dummy; std::cin >> dummy; 
                    continue; 
                }
                
                if (idx == -1) break;
                if (idx < 0 || idx >= test_data.get_rows()) {
                    std::cout << "Index out of bounds!" << std::endl;
                    continue;
                }

                Matrix<float> row = test_data.sub_row(idx);
                int label = static_cast<int>(row(0, 0));
                
                Matrix<float> image(784, 1);
                for (int j = 0; j < 784; j++) {
                    image(j, 0) = row(0, j + 1) / 255.0f;
                }

                CSVReader::digit digit = CSVReader::matrix_to_digit(image, label);
                digit.visualize();
                
                int pred = net.prediction(image);
                std::cout << "Network prediction : " << pred << std::endl;
                std::cout << "Actual Label       : " << label << std::endl;
            }
        }

    } 
    catch (const MatrixException& e) {
        std::cerr << "\n[CRITICAL ERROR] Matrix/NeuralNet logic failed:\n" 
                  << "  -> " << e.what() << std::endl;
        return 1;
    } 
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Standard exception occurred:\n" 
                  << "  -> " << e.what() << std::endl;
        return 1;
    } 
    catch (...) {
        std::cerr << "\n[UNKNOWN ERROR] An unhandled exception caused a crash." << std::endl;
        return 1;
    }

    return 0;
}