import os
import pandas as pd

SEQUENTIAL_TIMING = {}

def plot_sequential():
    folder = './data/plots/sequential'
    if not os.path.exists(folder):
        os.makedirs(folder)
    df = pd.DataFrame(SEQUENTIAL_TIMING.items(), columns=['matrix_size', 'time']).sort_values(by='matrix_size')
    ax = df.plot(x='matrix_size', y='time', kind='line', title='Sequential Timing', xlabel='Matrix Size', ylabel='Time (ms)')
    fig = ax.get_figure()
    fig.savefig(f'{folder}/sequential_timing.png')

def plot_data(df, version):
    folder = f'./data/plots/{version}'
    if not os.path.exists(folder):
        os.makedirs(folder)
    lowest_matrix_size = df['matrix_size'].min()
    
    # ================= Weak Scaling ================= #
    
    # speedup = sequential_time / parallel_time
    
    weak_scaling = df[df['matrix_size'] == lowest_matrix_size]
    for core in weak_scaling['cores'].unique():
        numerator = weak_scaling[weak_scaling['cores'] == core]['time']
        denominator = SEQUENTIAL_TIMING[lowest_matrix_size]
        speedup = denominator / numerator
        weak_scaling.loc[weak_scaling['cores'] == core, 'time'] = speedup
    ax = weak_scaling.plot(x='cores', y='time', kind='line', title=f'{version} Weak Scaling', xlabel='Cores', ylabel='Speedup')
    fig = ax.get_figure()
    fig.savefig(f'{folder}/weak_scaling_speedup.png')
    
    # efficiency = speedup / cores
    
    for core in weak_scaling['cores'].unique():
        speedup = weak_scaling[weak_scaling['cores'] == core]['time']
        efficiency = speedup / core
        weak_scaling.loc[weak_scaling['cores'] == core, 'time'] = efficiency
    ax = weak_scaling.plot(x='cores', y='time', kind='line', title=f'{version} Weak Scaling', xlabel='Cores', ylabel='Efficiency')
    fig = ax.get_figure()
    fig.savefig(f'{folder}/weak_scaling_efficiency.png')
    
    # 
        
    # ================ Strong Scaling ================ #
    
    # TODO
    pass

def measure():
    df = pd.DataFrame(columns=['cores', 'matrix_size', 'name', 'time'])
    for filename in os.listdir('./data/results'):
        if filename.endswith('.txt'):
            with open(f'./data/results/{filename}', 'r') as f:
                out = f.read().split('\n')[0].split('Elapsed milliseconds: ')[1:]
            if (len(out) == 0):
                print(filename)
            items = filename.split('_')
            cores = int(items[0])
            matrix_size = items[1]
            version = items[2].split('.txt')[0]
            time = float(out[0])
            df = df._append({'cores': cores, 'matrix_size': matrix_size, 'version': version, 'time': time}, ignore_index=True)
    
    mpi = df[df['version'] == 'mpi']
    fastflow = df[df['version'] == 'fastflow']
    sequential = df[df['version'] == 'sequential']

    for matrix_size in df['matrix_size'].unique():
        SEQUENTIAL_TIMING[matrix_size] = sequential[sequential['matrix_size'] == matrix_size]['time'].mean()
    
    plot_sequential()
    plot_data(mpi, 'mpi')
    plot_data(fastflow, 'fastflow')
    #plot_data(sequential, 'sequential')
            
if __name__ == '__main__':
    measure()