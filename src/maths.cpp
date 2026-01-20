#include "../include/maths.h"


template <typename T>
Matrix<T>::Matrix() {
    data_ = new T[1];
    rows_ = 1;
    cols_ = 1;
}

template <typename T>
Matrix<T>::Matrix(int rows, int cols) {
    data_ = new T[rows * cols]{};
    rows_ = rows;
    cols_ = cols; 
}

template <typename T>
Matrix<T>::Matrix(int rows, int cols, T* values) {
    data_ = new T[rows * cols];
    rows_ = rows;
    cols_ = cols; 

    for (int i = 0; i < rows_ * cols_; i++) {
        data_[i] = values[i];
    }
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> const& other) {
    this->data_ = new T[other.rows_ * other.cols_];
    this->rows_ = other.rows_;
    this->cols_ = other.cols_;
    for (int i = 0; i < other.rows_; i++) {
        for (int j = 0; j < other.cols_; j++) {
            this->data_[i * cols_ + j] = other.data_[i * cols_ + j];
        }
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    delete[] data_;
}

template <typename T>
T& Matrix<T>::operator()(int i, int j) const {
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
        std::cerr << "Erreur d'accès : indices (" << i << "," << j 
                  << ") pour une matrice de taille " << rows_ << "x" << cols_ << std::endl;
        throw std::out_of_range("Index de matrice hors limites");
    }
    return data_[i * cols_ + j];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size not compatible for sum");
    }
    Matrix<T> res(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            res(i, j) = (*this)(i, j) + other(i, j);
        }
    }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(T value) const {
    Matrix<T> res(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            res(i, j) = (*this)(i, j) + value;
        }
    }
    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size not compatible for sum");
    }
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            (*this)(i, j) += other(i, j);
        }
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size not compatible for sum");
    }
    Matrix<T> res(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            res(i, j) = (*this)(i, j) - other(i, j);
        }
    }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(T value) const {
    Matrix<T> res(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            res(i, j) = (*this)(i, j) - value;
        }
    }
    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size not compatible for sum");
    }
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            (*this)(i, j) -= other(i, j);
        }
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this == &other) return *this;
    
    if (this->cols_ != other.cols_ || this->rows_ != other.rows_) {
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = new T[rows_ * cols_];
    }
    
    for (int i = 0; i < rows_ * cols_; i++) {
        data_[i] = other.data_[i];
    }
    
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    if (cols_ != other.rows_) throw std::invalid_argument("Incompatible dimensions");
    
    Matrix<T> res(rows_, other.cols_); 
    for (int i = 0; i < rows_; ++i) {
        for (int k = 0; k < cols_; ++k) {
            T temp = (*this)(i, k);
            for (int j = 0; j < other.cols_; ++j) {
                res(i, j) += temp * other(k, j);
            }
        }
    }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T lambda) {
    Matrix<T> res(*this);
    for (int i = 0; i < rows_ * cols_; i++) {
        res.data_[i] *= lambda;
    }
    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/(T lambda) {
    if (lambda == static_cast<T>(0)) {
        throw std::invalid_argument("Division by 0");
    }
    (*this) = (*this) * (static_cast<T>(1) / lambda);
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::hadamard(const Matrix<T>& other) {
    Matrix<T> res(*this);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            res(i, j) *= other(i, j);
        }
    }
    return res;
}

template <typename T>
void Matrix<T>::transpose() {
    T* data_bis_ = new T[rows_ * cols_];
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            data_bis_[j * rows_ + i] = data_[i * cols_ + j];
        }
    }
    delete[] data_;
    data_ = data_bis_;
    std::swap(rows_, cols_);
}

template <typename T>
Matrix<T> Matrix<T>::transposed() const {
    Matrix<T> result(cols_, rows_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::apply(std::function<T(T)> func) {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            (*this)(i, j) = func((*this)(i, j));
        }
    }
    return *this;
}

template <typename T>
void Matrix<T>::print() const {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            std::cout << " " << (*this)(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
}

template <typename T>
void Matrix<T>::print(int i, int j) const {
    std::cout << (*this)(i, j) << std::endl;
}

template <typename T>
void Matrix<T>::print_col(int col) const {
    for (int i = 0; i < rows_; i++) {
        std::cout << (*this)(i, col) << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
void Matrix<T>::print_row(int row) const {
    for (int i = 0; i < cols_; i++) {
        std::cout << (*this)(row, i) << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void Matrix<T>::randomize(T min, T max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(static_cast<double>(min), static_cast<double>(max));
    
    for (int i = 0; i < rows_ * cols_; ++i) {
        data_[i] = static_cast<T>(dis(gen));
    }
}

template <typename T>
T Matrix<T>::sum() const {
    T res = static_cast<T>(0);
    for (int i = 0; i < rows_ * cols_; i++) {
        res += data_[i];
    } 
    return res; 
}

template <typename T>
T Matrix<T>::max() const {
    T m = (*this)(0, 0); 
    for (int i = 0; i < rows_ * cols_; i++) {
        if (data_[i] > m) {
            m = data_[i];
        }
    }
    return m;
}

template <typename T>
int Matrix<T>::argmax() const {
    T m = data_[0]; 
    int arg_max = 0; 

    for (int i = 0; i < rows_; i++) {
        if ((*this)(i, 0) > m) {
            m = (*this)(i, 0);
            arg_max = i;
        }
    }
    return arg_max;
}

template <typename T>
Matrix<T>& Matrix<T>::softmax() {
    for (int j = 0; j < cols_; j++) {
        T max_val = (*this)(0, j);
        for (int i = 1; i < rows_; i++) {
            max_val = std::max(max_val, (*this)(i, j));
        }

        T s = static_cast<T>(0);
        for (int i = 0; i < rows_; i++) {
            (*this)(i, j) = std::exp(static_cast<double>((*this)(i, j) - max_val));
            s += (*this)(i, j);
        }

        for (int i = 0; i < rows_; i++) {
            (*this)(i, j) /= s;
        }
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::sub_col(int col) const {
    T* res = new T[rows_];
    for (int i = 0; i < rows_; i++) {
        res[i] = (*this)(i, col);
    } 
    Matrix<T> matrix(rows_, 1, res);
    delete[] res;
    return matrix;
}

template <typename T>
Matrix<T> Matrix<T>::sub_row(int row) const {
    T* res = new T[cols_];
    for (int i = 0; i < cols_; i++) {
        res[i] = (*this)(row, i);
    } 
    Matrix<T> matrix(1, cols_, res);
    delete[] res;
    return matrix;
}

template <typename T>
Matrix<T> Matrix<T>::to_label_matrix() const {
    Matrix<T> result(10, cols_);
    for (int col = 0; col < cols_; col++) {
        int label = static_cast<int>((*this)(0, col));
        for (int row = 0; row < 10; row++) {
            result(row, col) = (row == label) ? static_cast<T>(1) : static_cast<T>(0);
        }
    }
    return result;
}
// Explicit instantiations
template class Matrix<float>;
template class Matrix<double>;
template class Matrix<int>;
