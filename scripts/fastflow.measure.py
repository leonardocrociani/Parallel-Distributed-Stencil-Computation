from config import BASE_MATRIX_SIZE, FF_MAX_WORKERS
from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'matrix_size,threads,time\n'

os.system('cd ../src && make fastflow')

WORKERS = [i for i in range(1, FF_MAX_WORKERS + 1, 2)] # 1, 3, 5 ... 19 => only for workers (there is also the master process)
MATRIX_SIZES = [(pe+1) * BASE_MATRIX_SIZE for pe in WORKERS]

for matrix_size in tqdm(MATRIX_SIZES, desc="Matrix Sizes"):
    for workers in tqdm(WORKERS, desc=f"Workers (Matrix Size: {matrix_size})", leave=False):
        cmd = f'srun --time=01:00:00 --nodes=1 --ntasks=1 --cpus-per-task={workers + 1} ../src/fastflow {matrix_size} {workers}'  # workers + 1 (emitter)
        timing = run_and_get_time(cmd)
        csv += f'{matrix_size},{workers + 1},{timing}\n'

with open('./data/results/fastflow.csv', 'w') as f:
    f.write(csv)
