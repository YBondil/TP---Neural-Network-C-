#pragma once

#include <string>
#include <vector>
#include "maths.h"

class CSVReader {
public:
    struct digit{
        int label;
        Matrix pixels; // 28*28 image

        void print() ; 
        void visualize() const;
    };

    static std::vector<std::vector<float>> read(const std::string& filename, char delimiter = ',');
    static Matrix readAsMatrix(const std::string& filename, char delimiter = ',');
    static std::vector<digit> readDigits(const std::string& filename);
};


