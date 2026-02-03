#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

// Couleurs pour la console
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

inline int g_tests_passed = 0;
inline int g_tests_failed = 0;

template <typename T>
void assert_eq(T a, T b, const std::string& test_name) {
    if (a == b) {
        std::cout << GREEN << "[PASS] " << test_name << RESET << std::endl;
        g_tests_passed++;
    } else {
        std::cout << RED << "[FAIL] " << test_name << " (Expected " << a << ", got " << b << ")" << RESET << std::endl;
        g_tests_failed++;
    }
}

inline void assert_true(bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << GREEN << "[PASS] " << test_name << RESET << std::endl;
        g_tests_passed++;
    } else {
        std::cout << RED << "[FAIL] " << test_name << RESET << std::endl;
        g_tests_failed++;
    }
}

// Comparaison de flottants avec tolérance
inline void assert_float_eq(float a, float b, const std::string& test_name, float epsilon = 1e-5f) {
    if (std::abs(a - b) < epsilon) {
        std::cout << GREEN << "[PASS] " << test_name << RESET << std::endl;
        g_tests_passed++;
    } else {
        std::cout << RED << "[FAIL] " << test_name << " (Expected " << a << ", got " << b << ")" << RESET << std::endl;
        g_tests_failed++;
    }
}

inline void print_summary() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "TEST SUMMARY" << std::endl;
    std::cout << GREEN << "PASSED: " << g_tests_passed << RESET << std::endl;
    if (g_tests_failed > 0) {
        std::cout << RED << "FAILED: " << g_tests_failed << RESET << std::endl;
    } else {
        std::cout << "ALL TESTS PASSED PERFECLY!" << std::endl;
    }
    std::cout << "========================================" << std::endl;
}