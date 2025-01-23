from config import MPI_WEAK_SCALING_MATRIX_SIZE, FF_WEAK_SCALING_MATRIX_SIZE, CLUSTER_NODES, MAX_MPI_PROCESSES, FF_MAX_THREADS
from utils import run_and_get_time
from tqdm import tqdm
import os

csv = 'matrix_size,time\n'

os.system('cd ../src && srun make sequential')

POOL_MATRIX_SIZES = (
    [i * FF_WEAK_SCALING_MATRIX_SIZE for i in range(2, FF_MAX_THREADS + 1, 2)] +
    [i * MPI_WEAK_SCALING_MATRIX_SIZE for i in range(2, MAX_MPI_PROCESSES + 1, 2)] 
)

MATRIX_SIZES = [] # the unique ones.
for ms in POOL_MATRIX_SIZES:
    if not ms in MATRIX_SIZES:
        MATRIX_SIZES.append(ms) 

for matrix_size in tqdm(MATRIX_SIZES, desc="Processing matrix sizes"):
    cmd = f'srun --time=02:00:00 --ntasks=1 --cpus-per-task=1 --nodes=1 ../src/sequential {matrix_size}'
    timing = run_and_get_time(cmd)
    csv += f'{matrix_size},{timing}\n'
    
with open('./data/results/sequential.csv', 'w') as f:
    f.write(csv)