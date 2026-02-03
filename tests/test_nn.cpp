#include "../include/neural_net.h"
#include "../include/exception.h"
#include "test_utils.h"
#include <fstream>
#include <cstdio> // pour remove()

void test_nn_topology() {
    std::cout << "\n--- Testing Neural Network Topology ---" << std::endl;
    
    int layers[] = {784, 128, 10};
    NeuralNetwork nn(3, layers);
    
    // On ne peut pas facilement tester les membres privés (weights) sans modifier la classe,
    // mais on peut tester le forward pass.
    
    Matrix<float> input(784, 1);
    input.randomize_uni(0, 1);
    
    try {
        nn.forward(input);
        // Si on est ici, pas de crash
        assert_true(true, "Forward pass executes without crash");
        
        int pred = nn.prediction(input);
        assert_true(pred >= 0 && pred <= 9, "Prediction returns valid label range");
        
    } catch (const std::exception& e) {
        assert_true(false, std::string("Forward crashed: ") + e.what());
    }
}

void test_nn_save_load() {
    std::cout << "\n--- Testing NN Save/Load ---" << std::endl;
    
    int layers[] = {10, 5, 2};
    NeuralNetwork nn(3, layers);
    std::string filename = "test_model_temp.csv";
    
    // Save
    try {
        nn.save_csv(filename);
        assert_true(true, "Save model to CSV");
    } catch (const std::exception& e) {
        assert_true(false, "Save failed: " + std::string(e.what()));
    }
    
    // Check file exists
    std::ifstream f(filename);
    assert_true(f.good(), "File actually created on disk");
    f.close();

    // Load
    try {
        NeuralNetwork nn2(3, layers);
        nn2.load_from_csv(filename);
        assert_true(true, "Load model from CSV");
    } catch (const std::exception& e) {
        assert_true(false, "Load failed: " + std::string(e.what()));
    }

    // Load non-existent
    bool caught = false;
    try {
        nn.load_from_csv("non_existent_file.csv");
    } catch (const ModelLoadException& e) {
        caught = true;
    }
    assert_true(caught, "Catch ModelLoadException for missing file");
    
    // Cleanup
    std::remove(filename.c_str());
}

void run_nn_tests() {
    test_nn_topology();
    test_nn_save_load();
}