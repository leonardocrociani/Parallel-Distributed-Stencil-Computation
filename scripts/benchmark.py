import os
import subprocess
from tqdm import tqdm
from config import REPEAT_BENCHMARKS, CLUSTER_ENVIRONMENT, CODE_VERSIONS
from config import BENCH_MATRIX_SIZES, BENCH_CORES, BENCH_NODES

def run_code(matrix_size, cores, name, nodes):

    use_nodes = CLUSTER_ENVIRONMENT and name == 'mpi'
    output_file = f'./data/results/{cores}_{matrix_size}_{name}{("_"+nodes) if use_nodes else ""}.txt'
    
    if os.path.exists(output_file) and not REPEAT_BENCHMARKS:
        return
    
    else:
        if name == 'mpi':
            if cores < 2 or cores < nodes:
                return
            host_option  = f'--host {','.join([('node' + str(i+1)) for i in range(nodes)]) }' if use_nodes else ''
            if os.system(f"mpirun -np {cores} {host_option} ../src/{name}.o {matrix_size} > {output_file}") != 0:
                raise Exception(f"Error running {name} code")
            
        elif name == 'sequential':
            if cores != 1 or nodes > 1:
                return
            if os.system(f"../src/{name}.o {matrix_size} > {output_file}") != 0:
                raise Exception(f"Error running {name} code")
            
        else:
            if nodes > 1:
                return
            if os.system(f"../src/{name}.o {matrix_size} {cores} > {output_file}") != 0:
                raise Exception(f"Error running {name} code")

def run_all_version(matrix_size, cores, nodes):
    for name in CODE_VERSIONS:
        run_code(matrix_size, cores, name, nodes)

def benchmark_aux():
    
    print('Matrix Sizes:', BENCH_MATRIX_SIZES)
    print('Cores:', BENCH_CORES)
    print('Nodes:', BENCH_NODES)
    
    for matrix_size in tqdm(BENCH_MATRIX_SIZES, desc="Matrix Sizes"):
        for cores in tqdm(BENCH_CORES, desc="Cores", leave=False):
            for nodes in tqdm(BENCH_NODES, desc="Nodes", leave=False):
                run_all_version(matrix_size, cores, nodes)

def benchmark():
    benchmark_aux()

if __name__ == '__main__':
    benchmark()