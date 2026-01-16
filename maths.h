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
        Matrix& operator^ (float lambda) ;

        Matrix& transpose (const Matrix& matrix) ;
        
        void print() const ;
        int get_rows(){return rows_;};
        int get_cols(){return cols_;};
        void randomize(float min, float max) ;

    private :

       float* data_ ;
       int rows_ ;
       int cols_ ;

};