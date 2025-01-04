#include <mpi.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

// #define VERBOSE

int main(int argc, char* argv[]) {
    Chronometer chrono;
    uint64_t matrix_size = 0;
    vector<vector<double> > matrix;
    int rank, num_processes;

    vector<uint64_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) exit(-1);

    build_matrix(&matrix, matrix_size);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    MPI_Barrier(MPI_COMM_WORLD);

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    for (uint64_t kth_stage = 1; kth_stage < matrix_size; kth_stage++) {
        // compute which is my range of action
        int num_computations = matrix_size - kth_stage;

        int base_process_load = num_computations / num_processes;
        int remainder = num_computations % num_processes;
        int iterations_to_do = base_process_load + (rank < remainder ? 1 : 0);

        vector receive_counts(num_processes, 0);
        vector displacements(num_processes, 0);

        for (int rank_i = 0; rank_i < num_processes; rank_i++) {
            receive_counts[rank_i] = base_process_load + (rank_i < remainder ? 1 : 0);
            displacements[rank_i] = rank_i == 0 ? 0 : displacements[rank_i - 1] + receive_counts[rank_i - 1];
        }

        int row_start = displacements[rank];
        int row_end = row_start + iterations_to_do;

        // compute the local results
        vector local_results(iterations_to_do, 0.0);

        for (int m = row_start; m < row_end; m++) {
            double dot = 0.0;
            for (int i = 0; i < kth_stage; i++) {
                double el1 = matrix[m][m + i];
                double el2 = matrix[m + i + 1][m + kth_stage];

                dot += el1 * el2;
            }
            local_results[m - row_start] = cbrt(dot);
        }

        // exchange the values with the other processes. Does the AllGatherv implicitly synchronizes? all the processes?
        vector results(num_computations, 0.0);
        MPI_Allgatherv(local_results.data(), iterations_to_do, MPI_DOUBLE,                       // sendbuf, sendcount, sendtype
                       results.data(), receive_counts.data(), displacements.data(), MPI_DOUBLE,  // recvbuf, recvcounts, displacements, recvtype
                       MPI_COMM_WORLD                                                            // comm
        );

        // update the matrix values
        for (int m = 0; m < num_computations; m++) {
            matrix[m][m + kth_stage] = results[m];
        }
    }

    /* ========================================== */

    double elapsed = chrono.elapsed();
    double max_elapsed = 0.0;

    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (!rank) {
        printf("Elapsed milliseconds: %.5f\n", max_elapsed);    
        printf("Top right element: %.5f\n", matrix[0][matrix_size - 1]);

        if (matrix_size <= 5) print_matrix(&matrix);
    }

    MPI_Finalize();
    
    return 0;
}