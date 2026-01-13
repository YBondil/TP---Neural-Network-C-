#include <iostream> 


class Matrix {
   
    public :

    Matrix(int rows, int cols);
    ~Matrix();


    private :
       float* data_ ;
       int rows_ ;
       int cols_ ;
        

}