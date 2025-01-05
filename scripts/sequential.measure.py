from config import BENCH_MATRIX_SIZES
from utils import run_and_get_time
from tqdm import tqdm
import os

csv = 'matrix_size,time\n'

os.system('cd ../src && make sequential')

for matrix_size in tqdm(BENCH_MATRIX_SIZES, desc="Processing matrix sizes"):
    timing = run_and_get_time(f'../src/sequential {matrix_size}')
    csv += f'{matrix_size},{timing}\n'
    
with open('./data/results/sequential.csv', 'w') as f:
    f.write(csv)