#include "../include/csv_reader.h"
#include "test_utils.h"
#include <fstream>
#include <cstdio>

void test_csv_read() {
    std::cout << "\n--- Testing CSV Reader ---" << std::endl;
    
    // Création d'un fichier CSV bidon
    std::string filename = "test_data.csv";
    std::ofstream file(filename);
    file << "1.0,2.0,3.0\n4.0,5.0,6.0";
    file.close();

    try {
        // Test read as vector
        auto data = CSVReader::read(filename);
        assert_eq((int)data.size(), 2, "Read correct number of rows");
        assert_eq((int)data[0].size(), 3, "Read correct number of cols");
        assert_float_eq(data[1][2], 6.0f, "Value verification");

        // Test read as Matrix
        Matrix<float> mat = CSVReader::readAsMatrix(filename);
        assert_eq(mat.get_rows(), 2, "Matrix rows");
        assert_eq(mat.get_cols(), 3, "Matrix cols");
        assert_float_eq(mat(0, 1), 2.0f, "Matrix value check");

    } catch (const std::exception& e) {
        assert_true(false, "CSV Read threw exception: " + std::string(e.what()));
    }

    // Cleanup
    std::remove(filename.c_str());
}

void run_csv_tests() {
    test_csv_read();
}