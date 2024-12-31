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

//#define VERBOSE

using namespace ff;

struct Task {
    uint64_t row_start;
    uint64_t row_end;
    uint64_t kth_stage;
    vector<vector<double> >* matrix;
};

struct Emitter : ff_node_t<Task> {
    Emitter(vector<vector<double> >* matrix, uint64_t num_workers, uint64_t matrix_size) : matrix(matrix), num_workers(num_workers), matrix_size(matrix_size) {}

    Task* svc(Task* task) override {
        if (matrix_size - kth_stage == 1) {  // we concluded all the stages but the last one

#ifdef VERBOSE
            printf("============ STARTING STAGE: %d ============\n", kth_stage);
#endif

            double dot = .0;
            for (uint64_t i = 0; i < kth_stage; i++) {
                double el1 = (*matrix)[0][0 + i];
                double el2 = (*matrix)[0 + i + 1][0 + kth_stage];
                dot += el1 * el2;
            }
            (*matrix)[0][0 + kth_stage] = cbrt(dot);

#ifdef VERBOSE
            printf("\n👌🏻: %.5f\n", (*matrix)[0][0 + kth_stage]);
#endif
            return EOS;
        }

        if (task != nullptr) {  // task received back from a worker
            active_workers--;
        }

        if (active_workers == 0) {  // previous stage is completed or we just started

#ifdef VERBOSE
            printf("============ STARTING STAGE: %d ============\n", kth_stage);
#endif

            // for the current kth_stage, i have to do γ = [matrix_size - k] task.
            // if n_workers = 10, and γ = 5, i want to use just the first 5 workers.
            // on the other hand, if num_workers = 10 and γ = 92, i want to assign 10 10 9 ... 9

            int num_iterations = matrix_size - kth_stage;

            if (num_iterations < num_workers) {
                for (uint64_t i = 0; i < num_iterations; i++) {
                    Task* task_i = new Task();

                    task_i->row_start = i;
                    task_i->row_end = i + 1;
                    task_i->matrix = matrix;
                    task_i->kth_stage = kth_stage;

                    active_workers++;
                    ff_send_out(task_i, i);
                }
            }

            else {
                int base_tasks_assignment = num_iterations / num_workers;
                int remainder = num_iterations % num_workers;
                int prev = 0;

                for (uint64_t i = 0; i < num_workers; i++) {
                    Task* task_i = new Task();
                    int iterations_to_do = base_tasks_assignment + (i < remainder ? 1 : 0);

                    task_i->row_start = prev;
                    task_i->row_end = prev + iterations_to_do;
                    task_i->matrix = matrix;
                    task_i->kth_stage = kth_stage;

                    prev += iterations_to_do;

                    active_workers++;
                    ff_send_out(task_i, i);
                }
            }

            kth_stage++;
        }

        return GO_ON;
    }

    vector<vector<double> >* matrix;
    uint64_t num_workers, matrix_size, active_workers = 0, kth_stage = 1;
};

struct Worker : ff_node_t<Task> {
    Task* svc(Task* task) override {

#ifdef VERBOSE
        printf("\tReceived task -> row_start :: %d, row_end:: %d, ID: %d\n", task->row_start, task->row_end, get_my_id());
#endif

        for (uint64_t m = task->row_start; m < task->row_end; m++) {
            double dot = .0;
            for (uint64_t i = 0; i < task->kth_stage; i++) {
                double el1 = (*task->matrix)[m][m + i];
                double el2 = (*task->matrix)[m + i + 1][m + task->kth_stage];
                dot += el1 * el2;
            }
            (*task->matrix)[m][m + task->kth_stage] = cbrt(dot);
        }

        return task;
    }
};

int main(int argc, char* argv[]) {
    Chronometer chrono;
    uint64_t matrix_size = 0, num_workers = 0;
    vector<vector<double> > matrix;

    vector<uint64_t*> args;
    args.push_back(&matrix_size);
    args.push_back(&num_workers);
    if (-1 == parse_args(argc, argv, args)) return EXIT_FAILURE;

    build_matrix(&matrix, matrix_size);

    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    ff_farm farm;
    Emitter emitter = Emitter(&matrix, num_workers, matrix_size);
    std::vector<ff_node*> workers;

    for (size_t i = 0; i < num_workers; ++i) {
        workers.push_back(new Worker());
    }

    farm.add_emitter(emitter);
    farm.add_workers(workers);
    farm.wrap_around();
    farm.set_scheduling_ondemand();

    printf("\n🚜 [FARM STARTING]\n\tmatrix_size => %llu\n\tnum_workers => %llu\n\n", matrix_size, num_workers);

    if (farm.run_and_wait_end() < 0) {
        error("running farm");
        return -1;
    }

    /* ========================================== */

    chrono.print_elapsed("WAVEFRONT");

    if (matrix_size <= 5) print_matrix(&matrix);
}
