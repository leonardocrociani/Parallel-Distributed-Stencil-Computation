from config import BENCH_FF_WORKERS, BASE_MATRIX_SIZE
from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'matrix_size,workers,time\n'

os.system('cd ../src && make fastflow')        

BENCH_FF_MATRIX_SIZES = [i * BASE_MATRIX_SIZE for i in range(1, BENCH_FF_WORKERS[-1] + 1)]

for matrix_size in tqdm(BENCH_FF_MATRIX_SIZES, desc="Matrix Sizes", leave=False):
    for workers in tqdm(BENCH_FF_WORKERS, desc="Workers", leave=False):
        
        timing = run_and_get_time(f'../src/fastflow {matrix_size} {workers}')
        csv += f'{matrix_size},{workers},{timing}\n'
        
with open('./data/results/fastflow.csv', 'w') as f:
    f.write(csv)