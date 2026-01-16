#include "maths.h"
#include <iostream>
#include <random>
#include <stdexcept>

Matrix::Matrix(int rows, int cols){
    data_ = new float[rows*cols] ;
    rows_ = rows ;
    cols_ = cols ; 
}

Matrix::Matrix (Matrix const& other){
    this-> data_ = new float[other.rows_*other.cols_] ;
    this-> rows_ = other.rows_ ;
    this-> cols_ = other.cols_ ;
    for (int i=0; i<other.rows_; i++){
        for (int j = 0; j<other.cols_; j++){
            this-> data_[i*cols_ + j] = other.data_[i*cols_ + j];
        }
    }
} ;
Matrix::~Matrix(){
    delete [] data_ ;
}


float& Matrix::operator() (int i, int j) const {
    
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
        throw std::out_of_range("Index de matrice hors limites");
    }

    return data_[i * cols_ + j];
}

Matrix Matrix::operator+ (const Matrix& other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        throw std::invalid_argument("Matrix size not compatible for sum") ;
    }
    Matrix res = Matrix(rows_, cols_);
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            res(i,j) = (*this)(i,j) + other(i,j);
        }
    }
    return res ;
}

Matrix& Matrix::operator=(const Matrix&  other){
    if (this-> cols_ != other.cols_ || this -> rows_ != other.rows_){
        throw std::invalid_argument ("Matrix size not compatible for = operator");
    }
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            (*this)(i,j) =  other(i,j);
        }
        
    }
    return *this ;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_){
        throw std::invalid_argument("Matrix size not compatible for sum") ;
    }
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            (*this)(i,j) += other(i,j);
        }
    }
    return *this;
}

Matrix Matrix::operator* (const Matrix & other) const {
    if (cols_ != other.rows_){
        throw std::invalid_argument("Matix sizes not compatible for dot product") ;
    }
    Matrix res = Matrix(rows_, other.cols_) ;

    for (int i = 0; i < rows_; i++){
        for (int j = 0; j < other.cols_; j++){

            float coef = 0 ;
            for (int n = 0; n < cols_; n++){
                coef += (*this)(i,n)*other(n,j) ;
            }
            res(i,j) = coef;
        }
    }

    return res ; 
}

Matrix& Matrix::operator^(float lambda) {
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            (*this)(i,j) *= lambda;
        }
    }
    return *this;
}

void Matrix::print() const {
    for (int i = 0; i < rows_;i++){
        for (int j = 0; j< cols_ ; j++){
            std::cout<<" " << (*this)(i,j) << " " ;
        }
        std::cout << std::endl ;
    }
    std::cout << std::endl ; 
}

void Matrix::randomize(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    
    for (int i = 0; i < rows_ * cols_; ++i) {
        data_[i] = dis(gen);
    }
}