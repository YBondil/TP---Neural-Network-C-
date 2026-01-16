#pragma once

#include <string>
#include <vector>
#include "maths.h"

class CSVReader {
public:
    static std::vector<std::vector<float>> read(const std::string& filename, char delimiter = ',');
    static Matrix readAsMatrix(const std::string& filename, char delimiter = ',');
};


