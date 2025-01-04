MUST_EXISTS = ['../src/fastflow.cpp', '../src/mpi.cpp', '../src/sequential.cpp']
FOLDER_TO_CREATE = ['./data', './data/results', './data/plots']
VERSIONS = ['mpi', 'fastflow', 'sequential']

REPEAT_BENCHMARKS = True
CLUSTER_ENVIRONMENT = False

BASE_CORES =  int(subprocess.check_output('lstopo | grep Core | wc -l', shell=True, text=True).strip())
BASE_MATRIX_SIZE = 1000
BASE_NODES = 1

BENCH_MATRIX_SIZES = [i * BASE_MATRIX_SIZE for i in range(1, 5)]
BENCH_CORES = [i for i in range(1, BASE_CORES + 1)]
BENCH_NODES = [i for i in range(1, BASE_NODES + 1)]