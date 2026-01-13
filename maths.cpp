#include "maths.h"

Matrix::Matrix(int rows, int cols){
    data_ = new float[rows*cols] ;
}
Matrix::~Matrix(){
    delete [] data_ ;
}