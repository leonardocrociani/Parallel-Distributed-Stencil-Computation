#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "lib/chronometer.hpp"
#include "lib/fastflow/ff/ff.hpp"
#include "lib/fastflow/ff/farm.hpp"
#include "lib/matrix.hpp"
#include "lib/utils.hpp"

using namespace ff;

struct Task {
    uint64_t start_idx;
    uint64_t end_idx;
    vector<vector<double> >* matrix;
};

struct Emitter : ff_node_t<Task> {
    Emitter(vector<vector<double> >* matrix, uint64_t num_workers, uint64_t matrix_size) : matrix(matrix), num_workers(num_workers), matrix_size(matrix_size) {}

    Task* svc(Task* task) override {
        if (matrix_size - kth_stage == 1) {  // we concluded all the stages but the last one
            // TODO: compute the last stage and return
            return EOS;
        }

        if (task != nullptr) {  // task received back from a worker
            //printf("Received back task! %d\n", task->start_idx);
            active_workers--;
        }

        if (active_workers == 0) {  // previous stage is completed or we just started

            printf("============ STARTING STAGE: %d ============\n", kth_stage);

            for (uint64_t i = 0; i < num_workers; i++) {
                Task* task_i = new Task();

                task_i->start_idx = i;
                task_i->end_idx = i;

                active_workers++;
                ff_send_out(task_i, i);
            }

            kth_stage++;
        }

        printf("👷🏻Active workers: %d\n", active_workers);

        return GO_ON;
    }

    vector<vector<double> >* matrix;
    uint64_t num_workers, matrix_size, active_workers = 0, kth_stage = 1;
};

struct Worker : ff_node_t<Task> {
    Task* svc(Task* task) override {
        printf("\tReceived task -> start_idx :: %d\n", task->start_idx);
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
