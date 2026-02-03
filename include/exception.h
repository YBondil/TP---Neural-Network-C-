#include <exception>
#include <string>

class MatrixException : public std::exception {
private:
    std::string message;
public:
    MatrixException(const std::string& msg) : message("Matrix Error: " + msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class DimensionMismatchException : public MatrixException {
public:
    DimensionMismatchException(int rowsA, int colsA, int rowsB, int colsB)
        : MatrixException("Incompatible dimensions for multiplication: (" 
          + std::to_string(rowsA) + "x" + std::to_string(colsA) + ") and (" 
          + std::to_string(rowsB) + "x" + std::to_string(colsB) + ")") {}
};