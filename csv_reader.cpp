#include "csv_reader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<std::vector<float>> CSVReader::read(const std::string& filename, char delimiter) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<std::vector<float>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::vector<float> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, delimiter)) {
            row.push_back(std::stof(value));
        }
        
        if (!row.empty()) {
            data.push_back(row);
        }
    }

    file.close();
    return data;
}

Matrix CSVReader::readAsMatrix(const std::string& filename, char delimiter) {
    auto data = read(filename, delimiter);
    
    if (data.empty()) {
        throw std::runtime_error("Empty CSV file");
    }

    int rows = data.size();
    int cols = data[0].size();
    
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result(i, j) = data[i][j];
        }
    }
    
    return result;
}
