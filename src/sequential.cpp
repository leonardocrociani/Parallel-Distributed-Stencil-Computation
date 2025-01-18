#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

int main(int argc, char* argv[]) {
    Chronometer chrono;
    size_t matrix_size = 1024;

    printf("Start...");

    std::vector<size_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size>\n", argv[0]);
        printf("--> Defaulting to matrix size: %ld\n", matrix_size);
    }

    printf("Initializing matrix size with %d...\n", matrix_size);

    // Allocazione dinamica per un array monodimensionale
    double* matrix = new double[matrix_size * matrix_size]();

    // Inizializzazione della matrice
    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            if (i == j) {
                matrix[i * matrix_size + j] = static_cast<double>(i + 1) / matrix_size;
            } else {
                matrix[i * matrix_size + j] = 0.0;
            }
        }
    }

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    for (size_t k = 1; k < matrix_size; k++) {
        for (size_t m = 0; m < matrix_size - k; m++) {
            double dot = .0;
            for (size_t i = 0; i < k; i++) {
                double el1 = matrix[m * matrix_size + (m + i)];
                double el2 = matrix[(m + k) * matrix_size + (m + i + 1)];
                dot += el1 * el2;
            }

            dot = cbrt(dot);
            matrix[m * matrix_size + (m + k)] = dot;
            matrix[(m + k) * matrix_size + m] = dot;
        }
    }

    double elapsed = chrono.elapsed();

    printf("Elapsed milliseconds: %.5f\n", elapsed);
    printf("Top Right Element: %.5f\n", matrix[matrix_size*matrix_size - 1]);


    if (matrix_size <= 5) {
        for (size_t i = 0; i < matrix_size; i++) {
            for (size_t j = 0; j < matrix_size; j++) {
                printf("%.3f ", matrix[i * matrix_size + j]);
            }
            std::cout << "\n";
        }
    }

    delete[] matrix;

    return 0;
}
