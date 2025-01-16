import subprocess
import matplotlib.pyplot as plt
from config import MUST_EXISTS, FOLDER_TO_CREATE, BASE_MATRIX_SIZE
import os

def run_and_get_time(cmd):
    output_lines = subprocess.check_output(cmd, shell=True, text=True).strip().split('\n')
    output = None # intentionally. Will generate an error if there is not the expeceted output
    for line in output_lines:
        if 'Elapsed' in line:
            output = line
            break
    ms = output.split('Elapsed milliseconds: ')[1].strip()
    return float(ms)

def plot_results(version, strong_scalability_matrix_sizes=[1024, 2048, 3072, 4096, 5120, 8192]): # version can be 'mpi' or 'fastflow'

    #########################################   Strong Scalability   ###################################

    with open('./data/results/sequential.csv', 'r') as f:
        sequential_data = f.read()

    seq_lines = sequential_data.strip().split('\n')
    sequential_times = {int(line.split(',')[0]): float(line.split(',')[1]) for line in seq_lines[1:]}  # {matrix_size: time}

    with open(f'./data/results/{version}.csv', 'r') as f:
        data = f.read()

    lines = data.strip().split('\n')
    rows = [line.split(',') for line in lines[1:]]
    parsed_data = [(int(row[0]), int(row[1]), float(row[2])) for row in rows]  # (matrix_size, processes/workers, time)

    for matrix_size in strong_scalability_matrix_sizes:
        size_data = [d for d in parsed_data if d[0] == matrix_size]
        
        if not size_data:
            print(f"Nessun dato trovato per matrix_size={matrix_size}.")
            continue
        
        workers = [d[1] for d in size_data]
        times = [d[2] for d in size_data]
        
        seq_time = sequential_times[matrix_size]
        speedup = [seq_time / t for t in times]
        
        efficiency = [s / w for s, w in zip(speedup, workers)]
        
        
        plt.figure(figsize=(10, 6))
        
        plt.plot(workers, speedup, marker='o', label=f'Speedup')
        
        plt.plot(workers, efficiency, marker='x', label=f'Efficiency', linestyle='--')
        
        plt.title(f'Strong Scalability - Speedup & Efficiency for Matrix Size {matrix_size} - {version.title()}')
        plt.xlabel(f'Number of {"Threads" if version == "fastflow" else "Processes"}')
        plt.ylabel('Speedup / Efficiency')
        
        plt.legend()
        plt.grid()
        
        plt.tight_layout()
        plt.savefig(f'./data/plots/{version}/strong_scalability_{matrix_size}.png')
        plt.close()

    ##########################################   Weak Scalability   ####################################

    workers_set = sorted(set(d[1] for d in parsed_data))  
    matrix_sizes = [w * BASE_MATRIX_SIZE for w in workers_set]  

    workers = []
    speedup = []
    efficiency = []
    labels_x = []

    for w, matrix_size in zip(workers_set, matrix_sizes):
        size_data = [d for d in parsed_data if d[0] == matrix_size and d[1] == w]

        if not size_data:
            print(f"Nessun dato trovato per workers={w} e matrix_size={matrix_size}.")
            continue
        
        time = size_data[0][2]
        
        seq_time = sequential_times[matrix_size]
        speedup_val = seq_time / time
        eff_val = speedup_val / w  
        
        workers.append(w)
        labels_x.append(f'({w}, {matrix_size})')
        speedup.append(speedup_val)
        efficiency.append(eff_val)
        
    plt.figure(figsize=(10, 6))

    plt.plot(workers, speedup, marker='o', label='Speedup')
    plt.xticks(workers, labels_x)


    plt.xticks(ticks=workers, labels=labels_x, rotation=30, horizontalalignment='right')

    plt.plot(workers, efficiency, marker='x', label='Efficiency', linestyle='--')
    
    plt.title(f'Weak Scalability - Speedup & Efficiency - {version.title()}')
    plt.xlabel(f'(Number of {"Threads" if version == "fastflow" else "Processes"}, Matrix Size)')
    plt.ylabel('Speedup / Efficiency')

    plt.legend()
    plt.grid()


    plt.tight_layout()
    plt.savefig(f'./data/plots/{version}/weak_scalability.png')
    plt.close()

    print("Plot saved successfully.")

def init_env():
    for folder in FOLDER_TO_CREATE:
        if not os.path.exists(folder):
            os.makedirs(folder)
    
    for file in MUST_EXISTS:
        if not os.path.exists(file):
            raise FileNotFoundError(f"File {file} not found.")
        
init_env()