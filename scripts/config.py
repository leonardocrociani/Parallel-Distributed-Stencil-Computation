import subprocess

MUST_EXISTS = ['../src/fastflow.cpp', '../src/mpi.cpp', '../src/sequential.cpp']
FOLDER_TO_CREATE = ['./data', './data/results', './data/plots', './data/plots/sequential', './data/plots/mpi', './data/plots/fastflow']
CODE_VERSIONS = ['mpi', 'fastflow', 'sequential']

REPEAT_BENCHMARKS = False
USE_ALL_CORES = True

BASE_CORES =  int(subprocess.check_output('lstopo-no-graphics | grep Core | wc -l', shell=True, text=True).strip())
BASE_MATRIX_SIZE = 512
CLUSTER_NODES = 7

BENCH_MATRIX_SIZES = [i * BASE_MATRIX_SIZE for i in range(1, 11)]
BENCH_MPI_PROCESSES = [i for i in range(1, CLUSTER_NODES+1)]
BENCH_FF_WORKERS = [i for i in range(1, 11)]
