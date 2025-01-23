import subprocess

MUST_EXISTS = ['../src/fastflow.cpp', '../src/mpi.cpp', '../src/sequential.cpp']
FOLDER_TO_CREATE = ['./data', './data/results', './data/plots', './data/plots/sequential', './data/plots/mpi', './data/plots/fastflow', './tmp']
CODE_VERSIONS = ['mpi', 'fastflow', 'sequential']

REPEAT_BENCHMARKS = False
USE_ALL_CORES = True

BASE_CORES =  int(subprocess.check_output('lstopo-no-graphics | grep Core | wc -l', shell=True, text=True).strip())
MPI_WEAK_SCALING_MATRIX_SIZE = 512
FF_WEAK_SCALING_MATRIX_SIZE = 256
MAX_MPI_PROCESSES = 16
CLUSTER_NODES = 8
FF_MAX_THREADS = 16 # 16 phisical core - 1 (used for the emitter).