#!/bin/bash

set -e

current_dir=$(basename "$PWD")

if [ "$current_dir" != "src" ]; then
    echo "You must navigate to the 'src' directory using 'cd src'."
    exit 1
fi

clear 

printf "Setting up environment...\n\n"

sleep 1

printf "(1/2) Cloning fastflow...\n\n"

if [ -d "lib/fastflow" ]; then

    printf "    Fastflow already cloned.\n\n"

else

    pushd lib > /dev/null && git clone https://github.com/fastflow/fastflow.git && cd fastflow/ff && srun -N 1 ./mapping_string.sh || { echo "Error during fastflow cloning..."; exit 1; }

    popd > /dev/null

fi

printf "(2/2) Building source...\n\n"

make || { echo "Error during source building..."; exit 1; }

printf "Environment setup completed.\n\n"

read -p "Would you like to execute the tests? (y/n): " execute_tests

if [ "$execute_tests" == "y" ]; then
    clear

    echo "Executing tests... MATRIX_SIZE=1024, FF_WORKERS=4, MPI_PROCESSES=4 (2 nodes)"

    sleep 1

    printf "\n\nRunning tests for sequential...\n"

    (sbatch -W jobs/sequential-job.sh && cat jobs/output_sequential.log && rm jobs/output_sequential.log) || { echo "Error while executing the sequential version"; exit 1; }

    printf "\n\nRunning tests for fastflow...\n"

    (sbatch -W jobs/fastflow-job.sh && cat jobs/output_fastflow.log && rm jobs/output_fastflow.log) || { echo "Error while executing the fastflow version"; exit 1; }

    printf "\n\nRunning tests for mpi...\n"

    (sbatch -W jobs/mpi-job.sh && cat jobs/output_mpi.log && rm jobs/output_mpi.log) || { echo "Error while executing the mpi version"; exit 1; }

    printf "\nTests completed.\n"

else
    echo "\nSkipping tests."
    
fi
