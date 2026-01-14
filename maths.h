#include <iostream> 


class Matrix {
   
    public :

        Matrix(int rows, int cols);
        Matrix (Matrix const& other) ;
        ~Matrix();

        float& operator()(int i, int j) const ;
        Matrix& operator= (const Matrix&  other) ;
        Matrix operator+  (const Matrix&  other) const ;
        Matrix& operator+=(const Matrix&  other) ;
        Matrix operator*  (const Matrix&  other) const ;
        
        void print() const ;
        void randomize(float min, float max) ;

    private :

       float* data_ ;
       int rows_ ;
       int cols_ ;

};