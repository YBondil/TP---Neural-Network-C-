#pragma once

#include <string>
#include <vector>
#include "maths.h"

class CSVReader {
public:
    struct digit{
        int label;
        Matrix<float> pixels; // 28*28 image

        void visualize() const;
    };

    static digit matrix_to_digit(Matrix<float>& mat, int label);
    static std::vector<std::vector<float>> read(const std::string& filename, char delimiter = ',', int maxLines = -1);
    static Matrix<float> readAsMatrix(const std::string& filename, char delimiter = ',', int maxLines = -1);
    static std::vector<digit> readDigits(const std::string& filename, int maxLines = -1);

    
};


