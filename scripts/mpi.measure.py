from config import CLUSTER_NODES, BASE_MATRIX_SIZE, PROCESSES_PER_NODE
from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'matrix_size,processes,time\n'

os.system('cd ../src && make mpi')

PROCESSES = [i for i in range(2, (CLUSTER_NODES * PROCESSES_PER_NODE) + 1, 2)] # 2, 4, 6 ... 16
MATRIX_SIZES = [p * BASE_MATRIX_SIZE for p in PROCESSES]

for matrix_size in tqdm(MATRIX_SIZES, desc="Matrix Sizes"):
    for processes in tqdm(PROCESSES, desc=f"Processes (Matrix Size: {matrix_size})", leave=False):
        nodes = int(processes / PROCESSES_PER_NODE)
        cmd = f'srun --time=01:00:00 --mpi=pmix --nodes={nodes} --ntasks={processes} --ntasks-per-node={PROCESSES_PER_NODE} ../src/mpi {matrix_size}'
        timing = run_and_get_time(cmd)
        csv += f'{matrix_size},{processes},{timing}\n'

with open('./data/results/mpi.csv', 'w') as f:
    f.write(csv)
