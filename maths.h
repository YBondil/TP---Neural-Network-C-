#include <iostream> 


class Matrix {
   
    public :

        Matrix(int rows, int cols);
        ~Matrix();

        float& operator()(int i, int j) const ;
        Matrix operator+ (Matrix& other) const ;
        Matrix& operator+=(Matrix& other) ;
        Matrix operator*(Matrix& other) const ;
        
        void print();
        void randomize(float min, float max) ;

    private :

       float* data_ ;
       int rows_ ;
       int cols_ ;

};