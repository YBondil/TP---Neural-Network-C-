#include "maths.h"

int main(){
    
    Matrix test = Matrix(3,1);
    test(0,0) = 1;    
        
    
    test(1,0) = 4;
    
    
    test(2,0) = 7;
    
    

    Matrix id = Matrix(3,3) ;
    id(0,0) = 1;    
    id(0,1) = 0;    
    id(0,2) = 0;
    id(1,0) = 0;
    id(1,1) = 1;
    id(1,2) = 0;
    id(2,0) = 0;
    id(2,1) = 0;
    id(2,2) = 1;

    
    
    test.print();
    id.print();
    (id*test).print();

    id += id ;
    id.print() ; 


    return 0;
}