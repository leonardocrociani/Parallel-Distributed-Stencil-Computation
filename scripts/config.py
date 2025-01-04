import subprocess

MUST_EXISTS = ['../src/fastflow.cpp', '../src/mpi.cpp', '../src/sequential.cpp']
FOLDER_TO_CREATE = ['./data', './data/results', './data/plots']
CODE_VERSIONS = ['mpi', 'fastflow', 'sequential']

REPEAT_BENCHMARKS = False
CLUSTER_ENVIRONMENT = False
USE_ALL_CORES = True

BASE_CORES =  int(subprocess.check_output('lstopo-no-graphics | grep Core | wc -l', shell=True, text=True).strip())
BASE_MATRIX_SIZE = 512

BENCH_MATRIX_SIZES = [i * BASE_MATRIX_SIZE for i in range(1, 11)]
BENCH_MPI_PROCESSES = [i for i in range(1, BASE_CORES*2)] # oversubscription
BENCH_FF_WORKERS = [i for i in range(1, BASE_CORES*2)] # hyperthreading