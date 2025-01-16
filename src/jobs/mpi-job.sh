#!/bin/bash
#SBATCH --job-name=mpi_job         
#SBATCH --output=jobs/output_mpi.log 
#SBATCH --nodes=2                           # 2 nodes
#SBATCH --ntasks=4                          # 4 processes
#SBATCH --ntasks-per-node=2                 # 1 process on each node
#SBATCH --cpus-per-task=1                   # 2 threads for each node => total of 4 processing entities
#SBATCH --time=01:00:00             

srun --mpi=pmix ./mpi 1024