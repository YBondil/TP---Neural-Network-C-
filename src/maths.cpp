#include "maths.h"
#include <iostream>
#include <random>
#include <stdexcept>
#include <functional>

Matrix::Matrix(){
    data_ = new float[1];
    rows_ = 1;
    cols_ = 1;
}

Matrix::Matrix(int rows, int cols){
    data_ = new float[rows*cols]{} ;
    rows_ = rows ;
    cols_ = cols ; 
}
Matrix::Matrix(int rows, int cols, float* values){
    data_ = new float[rows*cols] ;
    rows_ = rows ;
    cols_ = cols ; 

    for (int i = 0; i<rows_*cols_; i++){
        data_[i] = values[i];
    }
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
        std::cerr << "Erreur d'accès : indices (" << i << "," << j 
                  << ") pour une matrice de taille " << rows_ << "x" << cols_ << std::endl;
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

Matrix Matrix::operator- (const Matrix& other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        throw std::invalid_argument("Matrix size not compatible for sum") ;
    }
    Matrix res = Matrix(rows_, cols_);
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            res(i,j) = (*this)(i,j) - other(i,j);
        }
    }
    return res ;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_){
        throw std::invalid_argument("Matrix size not compatible for sum") ;
    }
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            (*this)(i,j) -= other(i,j);
        }
    }
    return *this;
}


Matrix& Matrix::operator=(const Matrix&  other){
    if (this == &other) return *this; // Self-assignment check
    
    if (this->cols_ != other.cols_ || this->rows_ != other.rows_){
        // Redimension: delete old data and allocate new
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = new float[rows_ * cols_];
    }
    
    // Copy data
    for (int i = 0; i < rows_ * cols_; i++){
        data_[i] = other.data_[i];
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

Matrix Matrix::operator*(float lambda) {
    Matrix res = Matrix(*this) ;
    for (int i = 0; i< rows_*cols_; i++){
        res.data_[i] *= lambda;
    }
    return res;
}

Matrix Matrix::hadamard(Matrix& other){
    Matrix res = Matrix(*this) ;
    for (int i = 0; i< rows_; i++){
        for (int j =0 ; j<cols_; j++){
            res(i,j) *= other(i,j);
        }
    }
    return res ;
}

void Matrix::transpose() {
    float* data_bis_ = new float[rows_ * cols_];
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            data_bis_[j * rows_ + i] = data_[i * cols_ + j];
        }
    }
    delete[] data_;
    data_ = data_bis_;
    std::swap(rows_, cols_);
}

Matrix Matrix::transposed() const {
    Matrix result(cols_, rows_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

Matrix& Matrix::apply(std::function<float (float)> func){
    for (int i = 0; i < rows_;i++){
        for (int j = 0; j< cols_ ; j++){
            (*this)(i,j) = func((*this)(i,j)) ;
        }
    }
    return *this ; 
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

void Matrix::print(int i, int j) const {
    std::cout << (*this)(i,j) << std::endl ;
}

void Matrix::print_col(int col) const{
    for (int i = 0; i< rows_; i++){
        std::cout << (*this)(i,col) << std::endl ;
    }
    std::cout << std::endl ;
}

void Matrix::print_row(int row) const{
    for (int i = 0; i< cols_; i++){
        std::cout << (*this)(row,i) << " " ;
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
float Matrix::sum() const {
    float res = 0. ;
    for (int i = 0; i < rows_*cols_; i++){
        res += data_[i] ;
    } 
    return res ; 
}


Matrix Matrix::sub_col(int col ) const {
    float*  res = new float[rows_];
    for (int i = 0 ; i<rows_; i++){
        res[i] = (*this)(i,col);
    } 
    Matrix matrix = Matrix(rows_, 1, res) ;
    delete[] res;
    return matrix;
}

Matrix Matrix::sub_row(int row ) const {
    float* res = new float[cols_];
    for (int i = 0 ; i<cols_; i++){
        res[i] = (*this)(row,i);
    } 
    Matrix matrix =  Matrix(1, cols_, res);
    delete [] res ;
    return matrix ;
}

