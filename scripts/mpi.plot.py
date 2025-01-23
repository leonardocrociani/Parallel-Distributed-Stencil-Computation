import matplotlib.pyplot as plt
from config import MPI_WEAK_SCALING_MATRIX_SIZE
import os

import matplotlib.pyplot as plt

def plot_results(strong_scalability_matrix_sizes=[1024, 2048, 3072, 4096, 5120, 6144, 7168, 8192]): 
    #########################################   Strong Scalability   ###################################

    with open('./data/results/sequential.csv', 'r') as f:
        sequential_data = f.read()

    seq_lines = sequential_data.strip().split('\n')
    sequential_times = {int(line.split(',')[0]): float(line.split(',')[1]) for line in seq_lines[1:]}  # {matrix_size: time}

    with open(f'./data/results/mpi.csv', 'r') as f:
        data = f.read()

    lines = data.strip().split('\n')
    rows = [line.split(',') for line in lines[1:]]
    parsed_data = [(int(float(row[0])), int(row[1]), int(float(row[2])), float(row[3])) for row in rows]  # (matrix_size, processes, nodes, time)

    node_styles = {
        1: {'color': 'C0', 'marker': 'o'},
        2: {'color': 'C1', 'marker': 's'},
        4: {'color': 'C2', 'marker': 'D'},
        8: {'color': 'C3', 'marker': 'x'}
    }
    
    for matrix_size in strong_scalability_matrix_sizes:
        plt.figure(figsize=(10, 10))

        for num_nodes in [1, 2, 4, 8]:
            local_parsed_data = [d for d in parsed_data if d[2] == num_nodes and d[0] == matrix_size]

            if not local_parsed_data:
                print(f"No data found for matrix_size={matrix_size}, num_nodes={num_nodes}.")
                continue

            processes = [d[1] for d in local_parsed_data]
            times = [d[3] for d in local_parsed_data]

            seq_time = sequential_times[matrix_size]
            speedup = [seq_time / t for t in times]
            efficiency = [s / w for s, w in zip(speedup, processes)]

            plt.plot(processes, speedup, marker=node_styles[num_nodes]['marker'], 
                     color=node_styles[num_nodes]['color'], linestyle='-', label=f'Speedup ({num_nodes} nodes)')

            plt.plot(processes, efficiency, marker=node_styles[num_nodes]['marker'], 
                     color=node_styles[num_nodes]['color'], linestyle='--', label=f'Efficiency ({num_nodes} nodes)')

        processes = [2, 4, 6, 8, 10, 12, 14, 16]
        plt.plot(processes, processes, color='green', linestyle='-', label='Ideal Speedup')

        plt.title(f'Strong Scalability - Speedup & Efficiency for Matrix Size {matrix_size} - Mpi')
        plt.xlabel(f'Number of Processes')
        plt.ylabel('Speedup / Efficiency')

        plt.legend()
        plt.grid()
        plt.tight_layout()
        
        # Save single plot per matrix size
        plt.savefig(f'./data/plots/mpi/strong_scalability_{matrix_size}.png')
        plt.close()
        

def plot_weak_scalability(MPI_WEAK_SCALING_MATRIX_SIZE):
    ##########################################   Weak Scalability   ####################################

    with open('./data/results/sequential.csv', 'r') as f:
        sequential_data = f.read()

    seq_lines = sequential_data.strip().split('\n')
    sequential_times = {int(line.split(',')[0]): float(line.split(',')[1]) for line in seq_lines[1:]}  # {matrix_size: time}

    with open(f'./data/results/mpi.csv', 'r') as f:
        data = f.read()

    lines = data.strip().split('\n')
    rows = [line.split(',') for line in lines[1:]]
    parsed_data = [(int(float(row[0])), int(row[1]), int(float(row[2])), float(row[3])) for row in rows]  # (matrix_size, processes, nodes, time)

    processes_set = sorted(set(d[1] for d in parsed_data))  
    matrix_sizes = [p * MPI_WEAK_SCALING_MATRIX_SIZE for p in processes_set]

    node_styles = {
        1: {'color': 'C0', 'marker': 'o'},
        2: {'color': 'C1', 'marker': 's'},
        4: {'color': 'C2', 'marker': 'D'},
        8: {'color': 'C3', 'marker': 'x'}
    }

    plt.figure(figsize=(10, 10))

    for num_nodes in [8,4,2,1]:
        local_parsed_data = [d for d in parsed_data if d[2] == num_nodes] 

        processes = []
        speedup = []
        efficiency = []
        
        labels_x = []

        for p, matrix_size in zip(processes_set, matrix_sizes):
            size_data = [d for d in local_parsed_data if d[0] == matrix_size and d[1] == p]

            if not size_data:
                print(f"No data found for processes={p}, matrix_size={matrix_size}, num_nodes={num_nodes}.")
                continue

            time = size_data[0][3]  # Single row, index 3 -> MPI execution time
            seq_time = sequential_times.get(matrix_size, None)

            if seq_time is None:
                print(f"No sequential time found for matrix_size={matrix_size}. Skipping...")
                continue

            speedup_val = seq_time / time
            eff_val = speedup_val / p  

            processes.append(p) 
            speedup.append(speedup_val)
            efficiency.append(eff_val)
            if not f'({p}, {matrix_size})' in labels_x:
                labels_x.append(f'({p}, {matrix_size})')  # Append to labels_x only when valid data exists

        # Ensure lists have data before plotting
        if processes:
            plt.plot(processes, speedup, marker=node_styles[num_nodes]['marker'], 
                     color=node_styles[num_nodes]['color'], linestyle='-', label=f'Speedup ({num_nodes} nodes)')

            plt.plot(processes, efficiency, marker=node_styles[num_nodes]['marker'], 
                     color=node_styles[num_nodes]['color'], linestyle='--', label=f'Efficiency ({num_nodes} nodes)')

    # Add ideal speedup line (only if data exists)
    if processes:
        plt.plot(processes, processes, color='green', linestyle='-', label='Ideal Speedup')
        print(labels_x, processes)
        # Ensure xticks match actual data points
        plt.xticks(ticks=processes, labels=labels_x, rotation=30, ha='right')

    # Titles and Labels
    plt.title(f'Weak Scalability - Speedup & Efficiency - Mpi')
    plt.xlabel(f'(Number of Processes, Matrix Size)')
    plt.ylabel('Speedup / Efficiency')

    plt.legend()
    plt.grid()
    plt.tight_layout()

    # Save single plot for weak scalability
    plt.savefig(f'./data/plots/mpi/weak_scalability.png')
    plt.close()

    print("Weak Scalability plot saved successfully.")

plot_results()
plot_weak_scalability(MPI_WEAK_SCALING_MATRIX_SIZE)