#include "../include/maths.h"
#include "../include/exception.h"
#include "test_utils.h"

void test_matrix_construction() {
    std::cout << "\n--- Testing Matrix Construction ---" << std::endl;
    Matrix<float> m(3, 2);
    assert_eq(m.get_rows(), 3, "Rows initialized correctly");
    assert_eq(m.get_cols(), 2, "Cols initialized correctly");
    assert_float_eq(m(0, 0), 0.0f, "Default initialization to 0");

    float vals[] = {1, 2, 3, 4};
    Matrix<float> m2(2, 2, vals);
    assert_float_eq(m2(0, 1), 2.0f, "Array initialization correct");
}

void test_matrix_operations() {
    std::cout << "\n--- Testing Matrix Operations ---" << std::endl;
    float v1[] = {1, 2, 3, 4};
    float v2[] = {10, 20, 30, 40};
    Matrix<float> m1(2, 2, v1);
    Matrix<float> m2(2, 2, v2);

    // Addition
    Matrix<float> sum = m1 + m2;
    assert_float_eq(sum(0, 0), 11.0f, "Matrix Addition (0,0)");
    assert_float_eq(sum(1, 1), 44.0f, "Matrix Addition (1,1)");

    // Scalar mult
    Matrix<float> scaled = m1 * 2.0f;
    assert_float_eq(scaled(0, 1), 4.0f, "Scalar Multiplication");

    // Transpose
    Matrix<float> t = m1.transposed();
    assert_float_eq(t(0, 1), 3.0f, "Transpose logic (row becomes col)"); // m1(1,0) is 3
    
    // Matrix Multiplication
    // m1: [1 2]  m3: [1]
    //     [3 4]      [2]
    // res: 1*1 + 2*2 = 5
    //      3*1 + 4*2 = 11
    float v3[] = {1, 2};
    Matrix<float> m3(2, 1, v3);
    Matrix<float> prod = m1 * m3;
    
    assert_eq(prod.get_rows(), 2, "Product rows correct");
    assert_eq(prod.get_cols(), 1, "Product cols correct");
    assert_float_eq(prod(0, 0), 5.0f, "Matrix Product Value 1");
    assert_float_eq(prod(1, 0), 11.0f, "Matrix Product Value 2");
}

void test_matrix_maths() {
    std::cout << "\n--- Testing Matrix Maths ---" << std::endl;
    float vals[] = {1, 3, 
                    2, 4}; // 2x2
    Matrix<float> m(2, 2, vals);

    assert_float_eq(m.max(), 4.0f, "Max value");
    assert_float_eq(m.sum(), 10.0f, "Sum value");
    
    // Argmax (sur colonne 0 -> 1, 2 -> index 1 est le max ?) 
    // Attention: votre argmax regarde m(i,0), donc il suppose une matrice colonne ou regarde la première colonne
    float col_vals[] = {0.1f, 0.5f, 0.2f};
    Matrix<float> col(3, 1, col_vals);
    assert_eq(col.argmax(), 1, "Argmax index");

    // Softmax
    Matrix<float> s = col;
    s.softmax();
    float sum_s = s(0,0) + s(1,0) + s(2,0);
    assert_float_eq(sum_s, 1.0f, "Softmax sum is 1");
}

void test_matrix_exceptions() {
    std::cout << "\n--- Testing Matrix Exceptions ---" << std::endl;
    
    Matrix<float> m1(2, 2);
    Matrix<float> m2(3, 3);

    // Test Size Mismatch
    bool caught_size = false;
    try {
        Matrix<float> res = m1 + m2;
    } catch (const MatrixSizeOperationException& e) {
        caught_size = true;
        std::cout << "Caught expected error: " << e.what() << std::endl;
    } catch (...) {
        // Fallback si l'héritage d'exception est mal géré
    }
    assert_true(caught_size, "Catch MatrixSizeOperationException on +");

    // Test Index Out of Bounds
    bool caught_idx = false;
    try {
        float val = m1(5, 5);
    } catch (const MatrixIndexAccessException& e) {
        caught_idx = true;
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }
    assert_true(caught_idx, "Catch MatrixIndexAccessException on ()");
}

void run_matrix_tests() {
    test_matrix_construction();
    test_matrix_operations();
    test_matrix_maths();
    test_matrix_exceptions();
}