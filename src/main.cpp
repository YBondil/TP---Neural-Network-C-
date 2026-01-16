#include <iostream>
#include "maths.h"
#include "csv_reader.h"

int main(){
    std::vector<CSVReader::digit> digits = CSVReader::readDigits("data/Mnist/test.csv") ;
    digits[99].visualize();
    return 0;
}