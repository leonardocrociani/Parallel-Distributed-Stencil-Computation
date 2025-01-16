#!/bin/bash
#SBATCH --job-name=fastflow_job         
#SBATCH --output=jobs/output_fastflow.log 
#SBATCH --nodes=1         
#SBATCH --ntasks=1                            # 1 process
#SBATCH --cpus-per-task=5                     # 1 emitter, 4 workers, main will be stalled.
#SBATCH --time=01:00:00             

srun ./fastflow 1024 4