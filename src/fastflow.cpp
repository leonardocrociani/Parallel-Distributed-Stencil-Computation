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
    Emitter(vector<vector<double> >* matrix, size_t num_workers, size_t matrix_size) : matrix(matrix), num_workers(num_workers), matrix_size(matrix_size) {}

    Task* svc(Task* task) override {

        if (matrix_size - kth_stage == 0) {  // we concluded all the stages
            return EOS;
        }

        if (task != nullptr) {  // task received back from a worker
            active_workers--;
        }

        if (active_workers == 0) {  // previous stage is completed or we just started
            size_t num_computations = matrix_size - kth_stage;

            size_t base_tasks_assignment = num_computations / num_workers;
            size_t remainder = num_computations % num_workers;
            size_t prev = 0;

            for (size_t i = 0; i < num_workers; i++) {
                Task* task_i = new Task();
                int iterations_to_do = base_tasks_assignment + (i < remainder ? 1 : 0);

                task_i->row_start = prev;
                task_i->row_end = prev + iterations_to_do;
                task_i->kth_stage = kth_stage;

                prev += iterations_to_do;

                active_workers++;
                ff_send_out(task_i, i);
            }

            kth_stage++;
        }

        return GO_ON;
    }

    vector<vector<double> >* matrix;
    size_t num_workers, matrix_size, active_workers = 0, kth_stage = 1;
};

struct Worker : ff_node_t<Task> {
    Worker(vector<vector<double> >* matrix) : matrix(matrix) {}

    Task* svc(Task* task) override {

        //printf("Worker! Id: %d, core: %d\n", get_my_id(), ff_getMyCore());

        for (size_t m = task->row_start; m < task->row_end; m++) {
            double dot = .0;
            for (size_t i = 0; i < task->kth_stage; i++) {
                double el1 = (*matrix)[m][m + i];
                double el2 = (*matrix)[m + i + 1][m + task->kth_stage];
                dot += el1 * el2;
            }
            (*matrix)[m][m + task->kth_stage] = cbrt(dot);
        }

        return task;
    }

    vector<vector<double> >* matrix;
};

int main(int argc, char* argv[]) {
    Chronometer chrono;
    size_t matrix_size = 1024, num_workers = ff_numCores() / 2;
    vector<vector<double> > matrix;

    vector<size_t*> args;
    args.push_back(&matrix_size);
    args.push_back(&num_workers);
    if (-1 == parse_args(argc, argv, args)) {
        printf("The correct usage is: %s <matrix_size> <num_workers>\n", argv[0]);
        printf("--> Defaulting to %ld matrix size and %ld workers <--\n", matrix_size, num_workers);
    }

    build_matrix(&matrix, matrix_size);

    //printf("FF cores: %zd\n", ff_numCores());
    
    chrono.reset();

    /* ============ CORE COMPUTATION ============ */

    ff_farm farm;
    Emitter emitter = Emitter(&matrix, num_workers, matrix_size);
    std::vector<ff_node*> workers;

    for (size_t i = 0; i < num_workers; ++i) {
        workers.push_back(new Worker(&matrix));
    }

    farm.add_emitter(emitter);
    farm.add_workers(workers);
    farm.wrap_around();

    if (farm.run_and_wait_end() < 0) {
        error("Farm execution failed!");
        return -1;
    }

    /* ========================================== */

    double elapsed = chrono.elapsed();

    printf("Elapsed milliseconds: %.5f\n", elapsed);
    printf("Top Right Element: %.5f\n", (matrix)[0][0 + matrix_size - 1]);

    if (matrix_size <= 5) print_matrix(&matrix);

    return 0;
}
