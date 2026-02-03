#include "exception.h"
#include "csv_reader.h" 
#include "neural_net.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <numeric>   
#include <random>   


NeuralNetwork::NeuralNetwork(int nb_layers, int* layers_sizes) {
    nb_layers_ = nb_layers;
    batch_size_ = 1;
    
    layers = new Matrix<float>[nb_layers_];
    weights = new Matrix<float>[nb_layers_ - 1];
    bias = new Matrix<float>[nb_layers_ - 1];
    velocity_weights = new Matrix<float>[nb_layers_ - 1];
    velocity_bias = new Matrix<float>[nb_layers_ - 1];

    for (int i = 0; i < nb_layers_; i++) {
        layers[i] = Matrix<float>(layers_sizes[i], batch_size_);
        
        if (i < nb_layers_ - 1) {
            int n_in = layers_sizes[i]; 
            float std_dev = std::sqrt(2.f / n_in);
            
            weights[i] = Matrix<float>(layers_sizes[i+1], layers_sizes[i]);
            weights[i].randomize_normal(0.f, std_dev);
            
            bias[i] = Matrix<float>(layers_sizes[i+1], 1);
            bias[i].randomize_uni(0.f, 0.f);

            velocity_weights[i] = Matrix<float>(layers_sizes[i+1], layers_sizes[i]);
            velocity_bias[i] = Matrix<float>(layers_sizes[i+1], 1);
        }
    }
}
NeuralNetwork::NeuralNetwork(int nb_layers, int* layers_sizes, int batch_size) {
    nb_layers_ = nb_layers;
    batch_size_ = batch_size;
    
    layers = new Matrix<float>[nb_layers_];
    weights = new Matrix<float>[nb_layers_ - 1];
    bias = new Matrix<float>[nb_layers_ - 1];
    velocity_weights = new Matrix<float>[nb_layers_ - 1];
    velocity_bias = new Matrix<float>[nb_layers_ - 1];

    for (int i = 0; i < nb_layers_; i++) {
        layers[i] = Matrix<float>(layers_sizes[i], batch_size);
        
        if (i < nb_layers_ - 1) {
            int n_in = layers_sizes[i]; 
            float std_dev = std::sqrt(2.f / n_in);
            
            weights[i] = Matrix<float>(layers_sizes[i+1], layers_sizes[i]);
            weights[i].randomize_normal(0.f, std_dev);
            
            bias[i] = Matrix<float>(layers_sizes[i+1], 1);
            bias[i].randomize_uni(0.f, 0.f);

            velocity_weights[i] = Matrix<float>(layers_sizes[i+1], layers_sizes[i]);
            velocity_bias[i] = Matrix<float>(layers_sizes[i+1], 1);
        }
    }
}

NeuralNetwork::~NeuralNetwork() {
    delete[] layers;
    delete[] weights;
    delete[] bias;
    delete[] velocity_bias;
    delete[] velocity_weights;
}


void NeuralNetwork::display() const {
    std::cout << "===========================================" << std::endl;
    std::cout << "NETWORK ARCHITECTURE" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Number of layers : " << nb_layers_ << std::endl;
    std::cout << "Batch size : " << batch_size_ << std::endl;

    std::cout << "\n--- LAYER 0 (ENTRY) ---" << std::endl;
    std::cout << "Size : " << layers[0].get_rows() << " neurones x " << batch_size_ << " batch" << std::endl;

    for (int i = 1; i < nb_layers_; ++i) {
        std::string type = (i == nb_layers_ - 1) ? "OUT" : "HIDDEN";
        std::cout << "\n--- LAYER " << i << " (" << type << ") ---" << std::endl;
        std::cout << "Size : " << layers[i].get_rows() << " neurones x " << batch_size_ << " batch" << std::endl;

        // Poids reliant la couche précédente et la couche actuelle
        std::cout << "Weights (weights[" << i - 1 << "]) linking L" << i - 1 << " -> L" << i 
                  << " [" << weights[i - 1].get_rows() << "x" << weights[i - 1].get_cols() << "] :" << std::endl;
        weights[i - 1].print();

        // Biais applique à la couche actuelle 
        std::cout << "Bias (bias[" << i - 1 << "]) of layer L" << i 
                  << " [" << bias[i - 1].get_rows() << "x" << bias[i - 1].get_cols() << "] :" << std::endl;
        bias[i - 1].print();
    }

    std::cout << "===========================================" << std::endl;
}

void NeuralNetwork::forward(const Matrix<float>& input) {
    layers[0] = input;
    
    for (int i = 0; i < nb_layers_ - 1; i++) {
        layers[i+1] = weights[i] * layers[i];

        int current_batch = layers[i+1].get_cols();

        for (int j = 0; j < current_batch; j++) {
            for (int k = 0; k < layers[i+1].get_rows(); k++) {
                layers[i+1](k, j) += bias[i](k, 0);
            }
        }
        if (i == nb_layers_-2){
            layers[i+1].softmax() ;
        }else {
            layers[i+1].apply(Maths_float::non_lin_func);
            }
    } 
}

void NeuralNetwork::backward(const Matrix<float>& target_y, float learning_rate) {
    Matrix<float> delta = (layers[nb_layers_-1] - target_y);
    int current_batch = delta.get_cols();
    
    for (int i = nb_layers_ - 2; i >= 0; i--) {
        // dW = (delta * a_prev^T) / current_batch
        Matrix<float> dW = delta * layers[i].transposed();
        dW = dW * (1.0f / current_batch);
        
        Matrix<float> db(bias[i].get_rows(), 1);
        for (int r = 0; r < delta.get_rows(); r++) {
            float sum_err = 0;
            for (int c = 0; c < current_batch; c++) {
                sum_err += delta(r, c);
            }
            db(r, 0) = sum_err / current_batch;
        }

        if (i > 0) {
            delta = weights[i].transposed() * delta;
            Matrix<float> prev_layer_deriv = layers[i];
            prev_layer_deriv.apply(Maths_float::non_lin_deriv);
            delta = delta.hadamard(prev_layer_deriv);
        }

    float mu = 0.9f; //coef de frottement
    velocity_weights[i] = velocity_weights[i] * mu - dW * learning_rate;
    weights[i] += velocity_weights[i];

    velocity_bias[i] = velocity_bias[i] * mu - db * learning_rate;
    bias[i] += velocity_bias[i];
    }
}

void NeuralNetwork::learn(const Matrix<float>& data, float learning_rate, int epochs, std::string save_path) {
    
    
    int total_samples = data.get_rows();
    int input_dim = data.get_cols() - 1;

    int expected_dim = weights[0].get_cols(); // premiere couche=entrée attendue

    if (input_dim != expected_dim) {
        throw MatrixException("Training data columns (" + std::to_string(input_dim) + 
                              ") do not match network input (" + std::to_string(expected_dim) + ")");
    }
    std::vector<int> indices(total_samples);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::cout << "Starting training for " << epochs << " epochs..." << std::endl;

    for (int e = 0; e < epochs; e++) {
        //suffle des données au début de l'epoch
        std::shuffle(indices.begin(), indices.end(), g);
        if (e > 5) {
            learning_rate *= 0.9f; //pour la stabilisation
        }     
        for (int i = 0; i <= total_samples - batch_size_; i += batch_size_) {
            
            Matrix<float> batch_features(input_dim, batch_size_);
            Matrix<float> batch_labels_raw(1, batch_size_);

            for (int b = 0; b < batch_size_; b++) {
                // Utiliser l'indice mélangé au lieu de i + b
                int sample_idx = indices[i + b]; 
                
                batch_labels_raw(0, b) = data(sample_idx, 0);
                
                for (int j = 0; j < input_dim; j++) {
                    batch_features(j, b) = data(sample_idx, j + 1) / 255.0f;
                }
            }

            Matrix<float> target_y = batch_labels_raw.to_label_matrix();
            forward(batch_features);
            backward(target_y, learning_rate);
        }
              
        std::cout << "Epoch " << e + 1 << "/" << epochs << " completed." << std::endl;
    }

    save_csv(save_path);
}

int NeuralNetwork::prediction(const Matrix<float>& image) {

    forward(image);
    Matrix<float>& output = layers[nb_layers_ - 1];
    int guess = output.argmax();
    return guess;

}

void NeuralNetwork::save_csv(std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw ModelSaveException(filename, "File not found") ;
    }

    file << nb_layers_ << "," << batch_size_ << "\n";

    for (int i = 0; i < nb_layers_ - 1; i++) {
        file << "W," << i << "," << weights[i].get_rows() << "," << weights[i].get_cols() << "\n";
        for (int r = 0; r < weights[i].get_rows(); r++) {
            for (int c = 0; c < weights[i].get_cols(); c++) {
                file << weights[i](r, c) << (c == weights[i].get_cols() - 1 ? "" : ",");
            }
            file << "\n";
        }

        file << "B," << i << "," << bias[i].get_rows() << "," << bias[i].get_cols() << "\n";
        for (int r = 0; r < bias[i].get_rows(); r++) {
            for (int c = 0; c < bias[i].get_cols(); c++) {
                file << bias[i](r, c) << (c == bias[i].get_cols() - 1 ? "" : ",");
            }
            file << "\n";
        }
    }
    file.close();
    std::cout << "Network saved correctly in : " << filename << std::endl;
}

void NeuralNetwork::load_from_csv(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ModelLoadException(filename, "File not found"); 
    }

    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string val;
    
    std::getline(ss, val, ',');
    int saved_nb_layers = std::stoi(val);
    std::getline(ss, val, ',');
    int saved_batch_size = std::stoi(val);

    
    if (saved_nb_layers != nb_layers_) {
        throw ModelLoadException(filename, "Architecture mismatch"); 
    
    if (saved_batch_size != batch_size_) {
        std::cerr << "Warning : Batch size different (file: " << saved_batch_size 
                  << ", current: " << batch_size_ << ")." << std::endl;
    }

    for (int i = 0; i < nb_layers_ - 1; i++) {
        
        std::getline(file, line); 
        

        for (int r = 0; r < weights[i].get_rows(); r++) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::string val;
            for (int c = 0; c < weights[i].get_cols(); c++) {
                std::getline(ss, val, ',');
                weights[i](r, c) = std::stof(val);
            }
        }

        // Lecture de l'en-tete des biais
        std::getline(file, line);

        for (int r = 0; r < bias[i].get_rows(); r++) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::string val;
            for (int c = 0; c < bias[i].get_cols(); c++) {
                std::getline(ss, val, ',');
                bias[i](r, c) = std::stof(val);
            }
        }
    }
    file.close();
    std::cout << "Network loaded correcly from : " << filename << std::endl;
}
}
