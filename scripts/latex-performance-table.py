import pandas as pd

def generate_latex_table(version):
    with open(f'./data/results/{version}.csv', 'r') as f:
        data = f.read()

    lines = data.strip().split('\n')
    rows = [line.split(',') for line in lines[1:]]
    parsed_data = [(int(row[0]), int(row[1]), int(float(row[2]))) for row in rows]  # (matrix_size, processes, milliseconds)

    df = pd.DataFrame(parsed_data, columns=['matrix_size', 'processes', 'milliseconds'])

    pivot_table = df.pivot(index='processes', columns='matrix_size', values='milliseconds')

    pivot_table = pivot_table.fillna('').sort_index(axis=1)

    latex_code = "\\begin{table}[H]\n"
    latex_code += "    \\centering\n"
    latex_code += "    \\begin{tabular}{|" + "c|" * (pivot_table.shape[1] + 1) + "}\n"
    latex_code += "    \\hline\n"

    latex_code += "    \\textbf{PEs}$\\backslash$\\textbf{M} & " + " & ".join(
        f"\\textbf{{{col}}}" for col in pivot_table.columns
    ) + " \\\\ \\hline\n"

    for process, row in pivot_table.iterrows():
        latex_code += f"    \\textbf{{{process}}} & " + " & ".join(
            str(int(val)) if val != '' else "" for val in row
        ) + " \\\\ \\hline\n"

    latex_code += "    \\end{tabular}\n"
    latex_code += "    \\caption{Execution times (in milliseconds) for various matrix side sizes and PEs counts.}\n"
    latex_code += "\\end{table}"

    return latex_code


ff_table = generate_latex_table('fastflow')
mpi_table = generate_latex_table('mpi')

print('\n=== FF perf. table ===\n\n', ff_table, '\n\n')
print('=== MPI perf. table ===\n\n', mpi_table, '\n')
