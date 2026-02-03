#include "test_utils.h"

// Déclarations avant utilisation
void run_matrix_tests();
void run_nn_tests();
void run_csv_tests();

int main() {
    std::cout << "STARTING FULL TEST SUITE" << std::endl;
    
    try {
        run_matrix_tests();
        run_csv_tests();
        run_nn_tests();
    } catch (const std::exception& e) {
        std::cout << RED << "CRITICAL FAILURE: Uncaught exception in test runner: " << e.what() << RESET << std::endl;
    }

    print_summary();
    return (g_tests_failed == 0) ? 0 : 1;
}