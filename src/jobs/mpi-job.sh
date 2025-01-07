#!/bin/bash
#SBATCH --job-name=mpi_job         
#SBATCH --output=jobs/output_mpi.log 
#SBATCH --nodes=4         
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=20  
#SBATCH --time=01:00:00             

# Load necessary modules (if any)
# module load mpi

# Set the number of threads
export OMP_NUM_THREADS=20

# Run the application
srun --mpi=pmix ./mpi 1024