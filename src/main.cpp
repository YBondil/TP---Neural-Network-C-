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
        // CASE: Model exists - Load and Test
        model_file.close();
        std::cout << "--- MODÈLE TROUVÉ ---" << std::endl;
        net.load_from_csv(model_path);

        std::cout << "Chargement de data/Mnist/test.csv..." << std::endl;
        Matrix test_data = CSVReader::readAsMatrix("data/Mnist/test.csv", ',', 10000);

        int correct = 0;
        for (int i = 0; i < test_data.get_rows(); i++) {
            Matrix row = test_data.sub_row(i);
            int label = static_cast<int>(row(0, 0));
            
            Matrix image(784, 1);
            for (int j = 0; j < 784; j++) {
                image(j, 0) = row(0, j + 1) / 255.0f;
            }

            if (net.prediction(image, label) == label) {
                correct++;
            }
        }
        std::cout << "\nPrécision sur le test : " << correct/100 << "%" << std::endl;

    } else {
        // CASE: No model - Train and Save
        std::cout << "--- AUCUN MODÈLE TROUVÉ : DÉBUT DE L'APPRENTISSAGE ---" << std::endl;
        Matrix train_data = CSVReader::readAsMatrix("data/Mnist/train.csv", ',', 100000);
        
        net.learn(train_data, 0.1f, 5, model_path);
        std::cout << "Apprentissage terminé et sauvegardé sous " << model_path << std::endl;
    }

    return 0;
}