#include <iostream>

using namespace std;

using matrix_t = double**;

matrix_t build_matrix(size_t matrix_size) {
    matrix_t matrix = new double*[matrix_size];
    new double*[matrix_size];
    for (size_t i = 0; i < matrix_size; ++i) {
        matrix[i] = new double[matrix_size]();
        for (size_t j = 0; j < matrix_size; ++j) {
            matrix[i][j] = (i == j) ? static_cast<double>(i + 1) / matrix_size : 0.0;
        }
    }
    return matrix;
}

__attribute__((always_inline)) inline void compute_task(matrix_t matrix, size_t row_start, size_t row_end, size_t kth_stage) {
    for (size_t m = row_start; m < row_end; ++m) {
        double dot = .0;
        for (size_t i = 0; i < kth_stage; ++i) {
            dot += matrix[m][m + i] * matrix[m + kth_stage][m + i + 1];
        }
        dot = cbrt(dot);
        matrix[m][m + kth_stage] = dot;
        matrix[m + kth_stage][m] = dot;
    }
}

__attribute__((always_inline)) inline void compute_task_mpi(matrix_t matrix, size_t row_start, size_t row_end, size_t kth_stage, vector<double>* local_results) {
    for (size_t m = row_start; m < row_end; m++) {
        double dot = 0.0;
        for (size_t i = 0; i < kth_stage; i++) {
            dot += matrix[m][m + i] * matrix[m + kth_stage][m + i + 1];
        }
        (*local_results)[m - row_start] = cbrt(dot);
    }
}

__attribute__((always_inline)) inline void update_matrix_mpi(matrix_t matrix, size_t num_computations, size_t kth_stage, vector<double>* results) {
    for (size_t m = 0; m < num_computations; m++) {
        matrix[m][m + kth_stage] = (*results)[m];
        matrix[m + kth_stage][m] = matrix[m][m + kth_stage];
    }
}

void print_matrix(matrix_t matrix, size_t matrix_size) {
    for (size_t i = 0; i < matrix_size; i++) {
        for (size_t j = 0; j < matrix_size; j++) {
            printf("%.3f ", matrix[i][j]);
        }
        cout << "\n" << endl;
    }
}

void free_matrix(matrix_t matrix, size_t matrix_size) {
    for (size_t i = 0; i < matrix_size; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

double get_top_right_element(matrix_t matrix, size_t matrix_size) {
    return matrix[0][matrix_size-1];
}