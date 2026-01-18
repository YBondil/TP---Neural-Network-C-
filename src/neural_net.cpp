#include "../include/neural_net.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "csv_reader.h" 



NeuralNetwork::NeuralNetwork(int nb_layers, int* layers_sizes) {
    nb_layers_ = nb_layers;
    batch_size_ = 1;
    
    layers = new Matrix[nb_layers_];
    weights = new Matrix[nb_layers_ - 1];
    bias = new Matrix[nb_layers_ - 1];

    for (int i = 0; i < nb_layers_; i++) {
        layers[i] = Matrix(layers_sizes[i], batch_size_);
        
        if (i < nb_layers_ - 1) {
            weights[i] = Matrix(layers_sizes[i+1], layers_sizes[i]);
            weights[i].randomize(-1.0, 1.0);
            
            bias[i] = Matrix(layers_sizes[i+1], 1);
            bias[i].randomize(-1.0, 1.0);
        }
    }
}
NeuralNetwork::NeuralNetwork(int nb_layers, int* layers_sizes,int batch_size) {
    nb_layers_ = nb_layers;
    batch_size_ = batch_size;
    
    layers = new Matrix[nb_layers_];
    weights = new Matrix[nb_layers_ - 1];
    bias = new Matrix[nb_layers_ - 1];

    for (int i = 0; i < nb_layers_; i++) {
        layers[i] = Matrix(layers_sizes[i], batch_size);
        
        if (i < nb_layers_ - 1) {
            weights[i] = Matrix(layers_sizes[i+1], layers_sizes[i]);
            weights[i].randomize(-1.0, 1.0);
            
            bias[i] = Matrix(layers_sizes[i+1], 1);
            bias[i].randomize(-1.0, 1.0);
        }
    }
}

NeuralNetwork::~NeuralNetwork() {
    delete[] layers;
    delete[] weights;
    delete[] bias;
}



void NeuralNetwork::initialize_parameters(){
   
    for(int i = 0; i<nb_layers_-1; i++){
        weights[i].randomize(-1.f,1.f);
        bias[i].randomize(-1.f,1.f);
    }
}

void NeuralNetwork::display() const {
    std::cout << "===========================================" << std::endl;
    std::cout << "ARCHITECTURE DU RESEAU NEURONAL (BATCH)" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Nombre total de couches : " << nb_layers_ << std::endl;
    std::cout << "Taille du batch : " << batch_size_ << std::endl;

    // Affichage de la couche d'entrée (Couche 0)
    std::cout << "\n--- COUCHE 0 (ENTREE) ---" << std::endl;
    std::cout << "Taille : " << layers[0].get_rows() << " neurones x " << batch_size_ << " echantillons" << std::endl;

    // Boucle pour les couches suivantes (Cachées et Sortie)
    for (int i = 1; i < nb_layers_; ++i) {
        std::string type = (i == nb_layers_ - 1) ? "SORTIE" : "CACHEE";
        std::cout << "\n--- COUCHE " << i << " (" << type << ") ---" << std::endl;
        std::cout << "Taille : " << layers[i].get_rows() << " neurones x " << batch_size_ << " echantillons" << std::endl;

        // Poids reliant la couche précédente (i-1) à la couche actuelle (i)
        std::cout << "Poids (weights[" << i - 1 << "]) reliant L" << i - 1 << " -> L" << i 
                  << " [" << weights[i - 1].get_rows() << "x" << weights[i - 1].get_cols() << "] :" << std::endl;
        weights[i - 1].print();

        // Biais appliqué à la couche actuelle (i)
        std::cout << "Biais (bias[" << i - 1 << "]) de la couche L" << i 
                  << " [" << bias[i - 1].get_rows() << "x" << bias[i - 1].get_cols() << "] :" << std::endl;
        bias[i - 1].print();
    }

    std::cout << "===========================================" << std::endl;
}

void NeuralNetwork::forward(const Matrix& input) {
    layers[0] = input;
    
    for (int i = 0; i < nb_layers_ - 1; i++) {
        layers[i+1] = weights[i] * layers[i];
        for (int j = 0; j < batch_size_; j++) {
            for (int k = 0; k < layers[i+1].get_rows(); k++) {
                layers[i+1](k, j) += bias[i](k, 0);
            }
        }
        
        layers[i+1].apply(Maths::sigmoid);
    }
        
    }


//on utilise le fait que simga_prime(z) = a(1-a)
//permet l'economie du stockage de z
void NeuralNetwork::backward(const Matrix& target_y, float learning_rate) {
    Matrix delta = (layers[nb_layers_-1] - target_y);
    
    auto sig_deriv_from_a = [](float a) { return a * (1.0f - a); };
    
    Matrix output_deriv = layers[nb_layers_-1];
    output_deriv.apply(sig_deriv_from_a);
    delta = delta.hadamard(output_deriv);

    for (int i = nb_layers_ - 2; i >= 0; i--) {
        // dW = (delta * a_prev^T) / batch_size
        Matrix dW = delta * layers[i].transposed();
        dW = dW * (1.0f / batch_size_);
        
        Matrix db(bias[i].get_rows(), 1);
        for (int r = 0; r < delta.get_rows(); r++) {
            float sum_err = 0;
            for (int c = 0; c < batch_size_; c++) {
                sum_err += delta(r, c);
            }
            db(r, 0) = sum_err / batch_size_;
        }

        weights[i] -= dW * learning_rate;
        bias[i] -= db * learning_rate;

        if (i > 0) {
            // delta_prev = (W^T * delta) * sigma'(a_prev)
            delta = weights[i].transposed() * delta;
            Matrix prev_layer_deriv = layers[i];
            prev_layer_deriv.apply(sig_deriv_from_a);
            delta = delta.hadamard(prev_layer_deriv);
        }
    }
}




void NeuralNetwork::save_csv(std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible de créer le fichier " << filename << std::endl;
        return;
    }

    // Sauvegarde du nombre total de couches et de la taille du batch
    file << nb_layers_ << "," << batch_size_ << "\n";

    for (int i = 0; i < nb_layers_ - 1; i++) {
        // Sauvegarde des Poids (Weights)
        file << "W," << i << "," << weights[i].get_rows() << "," << weights[i].get_cols() << "\n";
        for (int r = 0; r < weights[i].get_rows(); r++) {
            for (int c = 0; c < weights[i].get_cols(); c++) {
                file << weights[i](r, c) << (c == weights[i].get_cols() - 1 ? "" : ",");
            }
            file << "\n";
        }

        // Sauvegarde des Biais (Bias)
        file << "B," << i << "," << bias[i].get_rows() << "," << bias[i].get_cols() << "\n";
        for (int r = 0; r < bias[i].get_rows(); r++) {
            for (int c = 0; c < bias[i].get_cols(); c++) {
                file << bias[i](r, c) << (c == bias[i].get_cols() - 1 ? "" : ",");
            }
            file << "\n";
        }
    }
    file.close();
    std::cout << "Reseau sauvegarde avec succes dans : " << filename << std::endl;
}

void NeuralNetwork::load_from_csv(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    std::string val;
    
    std::getline(ss, val, ',');
    int saved_nb_layers = std::stoi(val);
    std::getline(ss, val, ',');
    int saved_batch_size = std::stoi(val);

    // Vérification de sécurité : l'architecture doit correspondre
    if (saved_nb_layers != nb_layers_) {
        std::cerr << "Erreur : L'architecture du fichier (" << saved_nb_layers 
                  << " couches) ne correspond pas au reseau actuel (" << nb_layers_ << ")." << std::endl;
        return;
    }
    
    if (saved_batch_size != batch_size_) {
        std::cerr << "Avertissement : Taille de batch differente (fichier: " << saved_batch_size 
                  << ", actuel: " << batch_size_ << ")." << std::endl;
    }

    for (int i = 0; i < nb_layers_ - 1; i++) {
        // Lecture de l'en-tête des poids
        std::getline(file, line); 
        
        // Chargement des données de la matrice de poids
        for (int r = 0; r < weights[i].get_rows(); r++) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::string val;
            for (int c = 0; c < weights[i].get_cols(); c++) {
                std::getline(ss, val, ',');
                weights[i](r, c) = std::stof(val);
            }
        }

        // Lecture de l'en-tête des biais
        std::getline(file, line);

        // Chargement des données de la matrice de biais
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
    std::cout << "Reseau charge avec succes depuis : " << filename << std::endl;
}