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

    std::vector<size_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size>\n", argv[0]);
        printf("--> Defaulting to matrix size: %ld\n", matrix_size);
    }

    matrix_t matrix = build_matrix(matrix_size);

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    for (size_t k = 1; k < matrix_size; k++) {
        compute_task(matrix, 0, matrix_size - k, k);
    }

    /* ========================================== */

    double elapsed = chrono.elapsed();

    printf("Elapsed milliseconds: %.5f\n", elapsed);

    if (matrix_size <= 5) print_matrix(matrix, matrix_size);

    free_matrix(matrix, matrix_size);

    return 0;
}