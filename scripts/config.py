import subprocess

MUST_EXISTS = ['../src/fastflow.cpp', '../src/mpi.cpp', '../src/sequential.cpp']
FOLDER_TO_CREATE = ['./data', './data/results', './data/plots', './data/plots/sequential', './data/plots/mpi', './data/plots/fastflow', './tmp']
CODE_VERSIONS = ['mpi', 'fastflow', 'sequential']

REPEAT_BENCHMARKS = False
USE_ALL_CORES = True

BASE_CORES =  int(subprocess.check_output('lstopo-no-graphics | grep Core | wc -l', shell=True, text=True).strip())
BASE_MATRIX_SIZE = 512
CLUSTER_NODES = 8
PROCESSES_PER_NODE = 2
FF_MAX_WORKERS = 19 # 1 is used for the emitter.