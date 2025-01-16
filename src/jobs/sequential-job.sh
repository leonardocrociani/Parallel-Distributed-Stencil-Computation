#!/bin/bash
#SBATCH --job-name=sequential_job         
#SBATCH --output=jobs/output_sequential.log 
#SBATCH --nodes=1         
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=01:00:00             

srun ./sequential 1024