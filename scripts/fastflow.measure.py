from config import FF_WEAK_SCALING_MATRIX_SIZE, FF_MAX_THREADS
from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'matrix_size,threads,time\n'

os.system('cd ../src && srun make fastflow')

THREADS = [i for i in range(2, FF_MAX_THREADS + 1, 2)] # 2, 4, ... 16
MATRIX_SIZES = [t * FF_WEAK_SCALING_MATRIX_SIZE for t in THREADS]

for matrix_size in tqdm(MATRIX_SIZES, desc="Matrix Sizes"):
    for threads  in tqdm(THREADS, desc=f"Threads (Matrix Size: {matrix_size})", leave=False):
        cmd = f'srun --time=01:00:00 --nodes=1 --ntasks=1 --cpus-per-task={threads} ../src/fastflow {matrix_size} {threads - 1}'  # workers + 1 (emitter)
        timing = run_and_get_time(cmd)
        csv += f'{matrix_size},{threads },{timing}\n'

with open('./data/results/fastflow.csv', 'w') as f:
    f.write(csv)
