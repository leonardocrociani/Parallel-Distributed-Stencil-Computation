from config import MPI_WEAK_SCALING_MATRIX_SIZE, MAX_MPI_PROCESSES, CLUSTER_NODES
from utils import run_and_get_time
import os
from tqdm import tqdm

os.system('cd ../src && make clean && srun make mpi')

PROCESSES = [1] + [i for i in range(2, MAX_MPI_PROCESSES + 1, 2)] # 2, 4, 6 ... 16
MATRIX_SIZES = [p * MPI_WEAK_SCALING_MATRIX_SIZE for p in PROCESSES]
NODES = [1, 2, 4, 8]

csv = 'matrix_size,processes,nodes,time\n'
for nodes in NODES:
    for matrix_size in tqdm(MATRIX_SIZES, desc="Matrix Sizes"):
        for processes in tqdm(PROCESSES, desc=f"Processes (Matrix Size: {matrix_size})", leave=False):
            if processes < nodes:
                continue
            cmd = f'srun --time=01:00:00 --mpi=pmix --nodes={nodes} --ntasks={processes} ../src/mpi {matrix_size}'
            timing = run_and_get_time(cmd)
            csv += f'{matrix_size},{processes},{nodes},{timing}\n'

with open(f'./data/results/mpi.csv', 'w') as f:
    f.write(csv)