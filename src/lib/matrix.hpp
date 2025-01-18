#include <iostream>

using namespace std;

void build_matrix(double* matrix[], size_t matrix_size) {
    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            if (i == j) {
                matrix[i][j] = static_cast<double>(i + 1) / matrix_size;
            } else {
                matrix[i][j] = 0.0;
            }
        }
    }
}

void print_matrix(double** matrix, size_t matrix_size) {
    for (size_t i = 0; i < matrix_size; i++) {
        for (size_t j = 0; j < matrix_size; j++) {
            printf("%.3f ", matrix[i][j]);
        }
        cout << "\n" << endl;
    }
}
