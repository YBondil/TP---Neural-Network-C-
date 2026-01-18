#pragma once 
#include <functional>
#include <cmath>


class Matrix {

    private :

       float* data_ ;
       int rows_ ;
       int cols_ ;
   
    public :    
        Matrix(); // default constructor
        Matrix(int rows, int cols);
        Matrix(int rows, int cols, float* values);
        Matrix (Matrix const& other) ;
        ~Matrix();  
        
        //Operators
        float& operator()(int i, int j) const ;
        Matrix& operator= (const Matrix&  other) ;
        Matrix operator+  (const Matrix&  other) const ;
        Matrix operator+  (float value) const ;
        Matrix& operator+=(const Matrix&  other) ;
        Matrix operator-  (const Matrix&  other) const ;
        Matrix operator-  (float value) const ;
        Matrix& operator-=(const Matrix&  other) ;
        Matrix operator*  (const Matrix&  other) const ;
        Matrix operator* (float lambda) ;
         
        Matrix hadamard (const Matrix& other);
        void transpose() ;
        Matrix transposed() const ; 
        Matrix& apply(std::function<float (float)>);    
        
        //Other methods
        int get_rows()const {return rows_;};
        int get_cols()const {return cols_;};  
        void print() const ;
        void print(int i, int j) const;
        void print_col(int col) const;
        void print_row(int row) const;
        void randomize(float min, float max) ;
        float sum() const;
        float mean() const {return sum()/(rows_*cols_) ; };
        float max() const ;
        int* argmax() const ;

        Matrix sub_col(int col) const;
        Matrix sub_row(int row) const;
        Matrix to_label_matrix() const;

    
};


namespace Maths{
    inline float values[9] = {1.0, 0.0, 0., 0., 1., 0., 0., 0., 1.}; 
    inline Matrix id = Matrix(3,3, values);

   
    static float sigmoid(float x){
        return 1/(1+std::exp(-x)); 
    }

//    static float sigmoid_prime(float x) {
//       float s = sigmoid(x);
//       return s * (1 - s);
//   }
}
