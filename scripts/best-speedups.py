import pandas as pd

def get_best_speedups(version, K=3):
    with open('./data/results/sequential.csv', 'r') as f:
        sequential_data = f.read()

    seq_lines = sequential_data.strip().split('\n')
    sequential_times = {int(line.split(',')[0]): float(line.split(',')[1]) for line in seq_lines[1:]}  # { matrix_size: time }

    with open(f'./data/results/{version}.csv', 'r') as f:
        data = f.read()

    lines = data.strip().split('\n')
    rows = [line.split(',') for line in lines[1:]]
    parsed_data = [(int(row[0]), int(row[1]), float(row[2])) for row in rows]  # (matrix_size, #pe, milliseconds)
    
    df = pd.DataFrame(parsed_data, columns=['matrix_size', 'pe', 'milliseconds'])

    df['sequential_time'] = df['matrix_size'].map(sequential_times)
    df['speedup'] = df['sequential_time'] / df['milliseconds']
    df['efficiency'] = df['speedup'] / df['pe']

    df = df.sort_values(by='speedup', ascending=False)
    top_k = df.drop_duplicates(subset=['matrix_size']).head(K)

    return top_k[['speedup', 'efficiency', 'matrix_size', 'pe', 'milliseconds', 'sequential_time']]

def print_speedup_latex_table(top_k, version, K):
    latex_rows = []
    for _, row in top_k.iterrows():
        matrix_size = row['matrix_size']
        processes = int(row['pe'])
        milliseconds = row['milliseconds']
        speedup = row['speedup']
        efficiency = row['efficiency']
        sequential_time = row['sequential_time']

        speedup_latex = f"$\\frac{{T(1)}}{{T(p)}} = \\frac{{{sequential_time:.2f}}}{{{milliseconds:.2f}}} = {speedup:.2f}$"
        efficiency_latex = f"$\\frac{{S(p)}}{{p}} = \\frac{{{speedup:.2f}}}{{{processes}}} = {efficiency:.2f}$"
        latex_rows.append(f"        {matrix_size} & {processes} & {milliseconds:.2f} & {speedup_latex} & {efficiency_latex} \\\\ \\hline")

    latex_table = f"""
\\begin{{table}}[H]
    \\centering
    \\def\\arraystretch{{2}}
    \\begin{{tabular}}{{|c|c|c|c|c|}}
        \\hline
        Matrix side Size & PEs & Time $T(p)$ & Speedup $S(p)$ & Efficiency $E(p)$ \\\\ \\hline
{chr(10).join(latex_rows)}
    \\end{{tabular}}
    \\caption{{Top {K} Configurations with the Best Speedup for {version}-based Version}}
\\end{{table}}
    """
    print(latex_table)


ff_top_speedups = get_best_speedups('fastflow', K=3)
mpi_top_speedups = get_best_speedups('mpi', K=3)

print('\n=== FF top speedups ===\n\n', ff_top_speedups, '\n\n')

print_speedup_latex_table(ff_top_speedups, 'fastflow', K=3)

print('=== MPI top speedups ===\n\n', mpi_top_speedups, '\n')

print_speedup_latex_table(mpi_top_speedups, 'mpi', K=3)