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
class MatrixSizeOperationException : public MatrixException {
    MatrixSizeOperationException(int row_1, int col_1, int row_2,  int col_2, std::string operation) 
        : MatrixException("Size (" 
            + std::to_string(row_1) + ", " + std::to_string(col_1) + ") and ("
            + std::to_string(row_1) + ", " + std::to_string(col_1) + ") not compatible for operation " 
            + operation ){}
};
class MatrixIndexAccessException : public MatrixException {
    MatrixIndexAccessException(int index_1, int index_2) 
        : MatrixException("Index " 
            + std::to_string(index_1) + " and " 
            + std::to_string(index_2) + " out of range." ){}
};


class ModelLoadException : public MatrixException {
public:
    ModelLoadException(const std::string& filename, const std::string& details)
        : MatrixException("Failed to load model from " + filename + ": " + details) {}
};

class ModelSaveException : public MatrixException {
public:
    ModelSaveException(const std::string& filename, const std::string& details)
        : MatrixException("Failed to save model in " + filename + ": " + details) {}
};