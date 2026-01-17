#include "csv_reader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

void CSVReader::digit::visualize() const {
    std::cout << "Label: " << label << std::endl;
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {
            float val = pixels(i, j);
            if (val < 0.15) std::cout << " ";
            else if (val < 0.3) std::cout << ".";
            else if (val < 0.5) std::cout << "+";
            else if (val < 0.7) std::cout << "*";
            else if (val < 0.85) std::cout << "@";
            else std::cout << "#";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::vector<float>> CSVReader::read(const std::string& filename, char delimiter, int maxLines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<std::vector<float>> data;
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line) && (maxLines == -1 || lineCount < maxLines)) {
        std::vector<float> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, delimiter)) {
            row.push_back(std::stof(value));
        }
        
        if (!row.empty()) {
            data.push_back(row);
        }
        lineCount++;
    }

    file.close();
    return data;
}

Matrix CSVReader::readAsMatrix(const std::string& filename, char delimiter, int maxLines) {
    auto data = read(filename, delimiter, maxLines);
    
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

std::vector<CSVReader::digit> CSVReader::readDigits(const std::string& filename, int maxLines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<digit> digits;
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line) && (maxLines == -1 || lineCount < maxLines)) {
        std::stringstream ss(line);
        std::string value;
        
        // Read label (first column)
        std::getline(ss, value, ',');
        int label = std::stoi(value);
        
        // Read pixels (784 values)
        std::vector<float> pixels;
        while (std::getline(ss, value, ',')) {
            pixels.push_back(std::stof(value) / 255.0f);  // Normalize
        }
        
        // Create 28x28 matrix
        Matrix image(28, 28);
        for (int i = 0; i < 28; i++) {
            for (int j = 0; j < 28; j++) {
                image(i, j) = pixels[i * 28 + j];
            }
        }
        
        digits.push_back({label, image});
        lineCount++;
    }

    file.close();
    return digits;
}
