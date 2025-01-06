from utils import run_and_get_time
import os
from tqdm import tqdm

csv = 'schedule,time\n'

os.system('cd ../src && make mpi')

for schedule in tqdm(['static', 'static,1', 'static,2', 'static,4'], desc="Schedule", leave=False):
    timing = run_and_get_time(f'OMP_SCHEDULE="{schedule}" srun --mpi=pmix --nodes 4 ../src/mpi 2048')
    csv +=  f'{schedule},{timing}\n'
            
print(csv)
