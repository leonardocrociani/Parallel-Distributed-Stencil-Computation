#include <mpi.h>

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
    int rank, num_processes;

    vector<size_t*> args;
    args.push_back(&matrix_size);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size>\n", argv[0]);
        printf("--> Defaulting to matrix size: %ld\n", matrix_size);
    }

    matrix_t matrix = build_matrix(matrix_size);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    MPI_Barrier(MPI_COMM_WORLD);

    vector receive_counts(num_processes, 0); // moved outside avoiding re-allocation
    vector displacements(num_processes, 0);

    double start_time = MPI_Wtime();

    /* ============ CORE COMPUTATION ============ */

    for (size_t kth_stage = 1; kth_stage < matrix_size - 1; kth_stage++) {
        // compute which is my range of action
        size_t num_computations = matrix_size - kth_stage;

        size_t base_process_load = num_computations / num_processes;
        size_t remainder = num_computations % num_processes;
        size_t iterations_to_do = base_process_load + (static_cast<size_t>(rank) < remainder ? 1 : 0);

        for (size_t rank_i = 0; rank_i < static_cast<size_t>(num_processes); rank_i++) {
            receive_counts[rank_i] = base_process_load + (rank_i < remainder ? 1 : 0);
            displacements[rank_i] = rank_i == 0 ? 0 : displacements[rank_i - 1] + receive_counts[rank_i - 1];
        }

        size_t row_start = displacements[rank];
        size_t row_end = row_start + iterations_to_do;

        // compute the local result
        vector local_results(iterations_to_do, 0.0);

       compute_task_mpi(matrix, row_start, row_end, kth_stage, &local_results);

        // exchange the values with the other processes
        vector results(num_computations, 0.0);
        MPI_Allgatherv(local_results.data(), iterations_to_do, MPI_DOUBLE,                       // sendbuf, sendcount, sendtype
                       results.data(), receive_counts.data(), displacements.data(), MPI_DOUBLE,  // recvbuf, recvcounts, displacements, recvtype
                       MPI_COMM_WORLD                                                            // comm
        );

        // update the matrix values
        update_matrix_mpi(matrix, num_computations, kth_stage, &results);
    }

    // save last useless communication and compute only with the master that has to show the result (the others can reach the time measurement)
    if (!rank) {
        double dot = 0.0;
        for (size_t i = 0; i < matrix_size - 1; ++i) {
            dot += matrix[0][i] * matrix[matrix_size - 1][i + 1]; // matrix[0][0 + i] * matrix[0 + matrix_size - 1][0 + i + 1];
        }
        matrix[0][matrix_size - 1] = cbrt(dot);
    }

    /* ========================================== */

    double elapsed = (MPI_Wtime() - start_time) * 1000;
    double max_elapsed = 0.0;

    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (!rank) {
        printf("Elapsed milliseconds: %.5f\n", max_elapsed);    
        printf("Top right element: %.5f\n", get_top_right_element(matrix, matrix_size));

        if (matrix_size <= 5) print_matrix(matrix, matrix_size);
    }

    MPI_Finalize();
    
    free_matrix(matrix, matrix_size);

    return 0;
}