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
    vector<vector<double> > matrix;

    vector<size_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size>\n", argv[0]);
        printf("--> Defaulting to matrix size: %ld\n", matrix_size);
    }

    build_matrix(&matrix, matrix_size);

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    for (size_t k = 1; k < matrix_size; k++) {
        for (size_t m = 0; m < matrix_size - k; m++) {
            double dot = .0;
            for (size_t i = 0; i < k; i++) {
                double el1 = matrix[m][m + i];
                double el2 = matrix[m + i + 1][m + k];
                dot += el1 * el2;
            }
            matrix[m][m + k] = cbrt(dot);
        }
    }

    /* ========================================== */

    double elapsed = chrono.elapsed();

    printf("Elapsed milliseconds: %.5f\n", elapsed);
    printf("Top right element: %.5f\n", matrix[0][matrix_size - 1]);

    if (matrix_size <= 5) print_matrix(&matrix);

    return 0;
}
