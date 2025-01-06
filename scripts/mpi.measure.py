from config import BENCH_MPI_PROCESSES, BASE_MATRIX_SIZE
from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'matrix_size,processes,time\n'

os.system('cd ../src && make mpi')

BENCH_MPI_MATRIX_SIZES = [i * BASE_MATRIX_SIZE for i in range(1, BENCH_MPI_PROCESSES[-1] + 1)]

for matrix_size in tqdm(BENCH_MPI_MATRIX_SIZES, desc="Matrix Sizes", leave=False):
    for processes in tqdm(BENCH_MPI_PROCESSES, desc="Processes", leave=False):
        timing = run_and_get_time(f'srun --mpi=pmix --nodes {processes} ../src/mpi {matrix_size}')
        csv += f'{matrix_size},{processes},{timing}\n'
        
with open('./data/results/mpi.csv', 'w') as f:
    f.write(csv)
