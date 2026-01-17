#include <iostream>
#include "maths.h"
#include "csv_reader.h"

int main(){
    std::vector<CSVReader::digit> digits_dev = CSVReader::readDigits("data/Mnist/test.csv", 100) ;
    digits_dev[99].visualize();
    


    int a = 3;
    int atab [4] = {1,2,3,4} ; 
    for (int i = 0; i<4; i++){
        std::cout<<*(atab+i) << std::endl;
}

    return 0;
}