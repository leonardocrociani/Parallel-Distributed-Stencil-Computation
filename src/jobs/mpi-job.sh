#!/bin/bash
#SBATCH --job-name=mpi_job         
#SBATCH --output=jobs/output_mpi.log 
#SBATCH --nodes=4         
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=20  
#SBATCH --time=01:00:00             

export OMP_NUM_THREADS=20

srun --mpi=pmix ./mpi 1024