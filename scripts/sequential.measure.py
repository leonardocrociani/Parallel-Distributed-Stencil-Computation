from utils import run_and_get_time
from tqdm import tqdm
import os

csv = 'matrix_size,time\n'

os.system('cd ../src && make sequential')

MATRIX_SIZES = [1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192, 9216, 10240] # valid for comparison both with ff and mpi

for matrix_size in tqdm(MATRIX_SIZES, desc="Processing matrix sizes"):
    cmd = f'srun --time=02:00:00 --ntasks=1 --cpus-per-task=1 --nodes=1 ../src/sequential {matrix_size}'
    timing = run_and_get_time(cmd)
    csv += f'{matrix_size},{timing}\n'
    
with open('./data/results/sequential.csv', 'w') as f:
    f.write(csv)