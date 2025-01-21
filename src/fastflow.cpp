#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/fastflow/ff/ff.hpp"
#include "lib/fastflow/ff/farm.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

using namespace ff;

struct Task {
    size_t row_start;
    size_t row_end;
    size_t kth_stage;
};

struct Emitter : ff_node_t<Task> {
    Emitter(double** matrix, size_t num_workers, size_t matrix_size, Task* task_set) : task_set(task_set), matrix(matrix), num_workers(num_workers), matrix_size(matrix_size) {}

    Task* svc(Task* task) override {
        if (matrix_size - kth_stage == 1) {  // all stages completed
            compute_task(matrix, 0, matrix_size - kth_stage, kth_stage);
            return EOS;
        }

        if (task != nullptr) {  // task received back from a worker
            active_workers--;
        }

        if (active_workers == 0) {  // ready for the next stage
            size_t num_computations = matrix_size - kth_stage;

            size_t base_tasks_assignment = num_computations / (num_workers + 1);
            size_t remainder = num_computations % (num_workers + 1);
            size_t prev = 0;
            size_t necessary_workers = base_tasks_assignment == 0 ? remainder - 1 : num_workers;

            for (size_t i = 0; i < necessary_workers; ++i) {
                size_t iterations_to_do = base_tasks_assignment + (i < remainder ? 1 : 0);

                task_set[i].row_start = prev;
                task_set[i].row_end = prev + iterations_to_do;
                task_set[i].kth_stage = kth_stage;

                ff_send_out(&task_set[i]);

                prev += iterations_to_do;
                active_workers++;
            }

            compute_task(matrix, prev, matrix_size - kth_stage, kth_stage);

            kth_stage++;
        }

        return GO_ON;
    }

    Task* task_set;
    double** matrix;
    size_t num_workers, matrix_size, active_workers = 0, kth_stage = 1;
};

struct Worker : ff_node_t<Task> {
    Worker(double** matrix) : matrix(matrix) {}

    Task* svc(Task* task) override {
        compute_task(matrix, task->row_start, task->row_end, task->kth_stage);
        return task;
    }

    matrix_t matrix;
};

int main(int argc, char* argv[]) {
    Chronometer chrono;
    size_t matrix_size = 1024, num_workers = ff_numCores() / 2;

    std::vector<size_t*> args;
    args.push_back(&matrix_size);
    args.push_back(&num_workers);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size> <num_workers>\n", argv[0]);
        printf("--> Defaulting to %ld matrix size and %ld workers <--\n", matrix_size, num_workers);
    }

    matrix_t matrix = build_matrix(matrix_size);

    Task* task_set = new Task[num_workers];

    std::vector<std::unique_ptr<ff_node>> workers;
    for (size_t i = 0; i < num_workers; ++i) {
        workers.push_back(std::make_unique<Worker>(matrix));
    }

    ff_Farm<Task> farm(std::move(workers), std::make_unique<Emitter>(matrix, num_workers, matrix_size, task_set));
    farm.remove_collector();
    farm.wrap_around();
    farm.set_scheduling_ondemand();

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    if (farm.run_and_wait_end() < 0) {
        error("Farm execution failed!");
        return -1;
    }

    /* ========================================== */

    double elapsed = chrono.elapsed();

    printf("Elapsed milliseconds: %.5f\n", elapsed);
    printf("Top Right Element: %.5f\n", matrix[0][matrix_size - 1]);

    free_matrix(matrix, matrix_size);

    delete[] task_set;

    return 0;
}
