#pragma once 
#include <functional>
#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>

template <typename T>
class Matrix {

    private :
       T* data_ ;
       int rows_ ;
       int cols_ ;
   
    public :    
        Matrix(); // default constructoTr
        Matrix(int rows, int cols); //set to 0 
        Matrix(int rows, int cols, T* values);
        Matrix(Matrix<T> const& other) ;
        ~Matrix();  

        //Operators
        T& operator()(int i, int j) const ;
        Matrix<T>& operator= (const Matrix<T>&  other) ;
        Matrix<T> operator+  (const Matrix<T>&  other) const ;
        Matrix<T> operator+  (T value) const ;
        Matrix<T>& operator+=(const Matrix<T>&  other) ;
        Matrix<T> operator-  (const Matrix<T>&  other) const ;
        Matrix<T> operator-  (T value) const ;
        Matrix<T>& operator-=(const Matrix<T>&  other) ;
        Matrix<T> operator*  (const Matrix<T>&  other) const ;
        Matrix<T> operator* (T lambda) ;
        Matrix<T>& operator/ (T lambda) ;
         
        Matrix<T> hadamard (const Matrix<T>& other);
        void transpose() ;
        Matrix<T>transposed() const ; 
        Matrix<T>& apply(std::function<T (T)>);    
        
        //Other methods
        int get_rows()const {return rows_;};
        int get_cols()const {return cols_;};  
        void print() const ;
        void print(int i, int j) const;
        void print_col(int col) const;
        void print_row(int row) const;
        void randomize(T min, T max) ;
        //maths methods
        T sum() const;
        T mean() const {return sum()/(rows_*cols_) ; };
        T max() const ;
        int argmax() const ;
        Matrix<T>& softmax();

        Matrix<T> sub_col(int col) const;
        Matrix<T> sub_row(int row) const;
        Matrix<T> to_label_matrix() const;

        void shuffle_rows() ; 
};


namespace Maths_float{
    inline float values[9] = {1., 0., 0., 0., 1., 0., 0., 0., 1.}; 
    inline Matrix<float> id(3,3, values);

   
    inline float sigmoid(float x){
        return 1/(1+std::exp(-x)); 
    }
    inline float sigm_prime(float x){
        return x*(1-x) ;
    }

    inline float Relu(float x){
        return std::max(0.f,x)  ;
    }

    inline float Relu_prime(float x){
        if (x<=0){return 0.f;} else {return 1.f;}
    }
    inline float exp(float x){
        return std::exp(x);
    }

    inline std::function<float (float)> non_lin_func  = Relu ;
    inline std::function<float (float)> non_lin_deriv = Relu_prime ;

}
