#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

int main(int argc, char* argv[]) {
    vector<vector<double> > matrix;
    Chronometer chrono;
    uint64_t matrix_size = 0;
    vector<uint64_t*> args;
    args.push_back(&matrix_size);

    parse_args(argc, argv, args);

    build_matrix(&matrix, matrix_size);

    chrono.reset();
    compute_wavefront(&matrix);
    chrono.print_elapsed("WAVEFRONT");

    if (matrix_size <= 5) print_matrix(&matrix);
}
