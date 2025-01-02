#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

int main(int argc, char* argv[]) {
    Chronometer chrono;
    uint64_t matrix_size = 0;
    vector<vector<double> > matrix;

    vector<uint64_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) exit(-1);

    build_matrix(&matrix, matrix_size);

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    for (uint64_t k = 1; k < matrix_size; k++) {
        for (uint64_t m = 0; m < matrix_size - k; m++) {
            double dot = .0;
            for (uint64_t i = 0; i < k; i++) {
                double el1 = matrix[m][m + i];
                double el2 = matrix[m + i + 1][m + k];
                dot += el1 * el2;
            }
            matrix[m][m + k] = cbrt(dot);
        }
    }

    /* ========================================== */

    chrono.print_elapsed("WAVEFRONT");

    if (matrix_size <= 5) print_matrix(&matrix);

    printf("Top Right Element: %.5f\n", (matrix)[0][0 + matrix_size - 1]);
}
