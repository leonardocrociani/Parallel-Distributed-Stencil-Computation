#!/bin/bash

set -e

current_dir=$(basename "$PWD")

if [ "$current_dir" != "src" ]; then
    echo "You must navigate to the 'src' directory using 'cd src'."
    exit 1
fi

clear 

echo "Setting up environment..."

sleep 1

echo "  (1/2) Cloning fastflow..."

if [ -d "lib/fastflow" ]; then

    echo "    Fastflow already cloned."

else

    pushd lib > /dev/null && git clone https://github.com/fastflow/fastflow.git && cd fastflow/ff && ./mapping_string.sh || { echo "Error during fastflow cloning..."; exit 1; }

    popd > /dev/null

fi

echo "  (2/2) Building source..."

make || { echo "Error during source building..."; exit 1; }

echo "Environment setup completed."

read -p "Would you like to execute the tests? (y/n): " execute_tests

if [ "$execute_tests" == "y" ]; then
    clear

    echo "Executing tests... MATRIX_SIZE=1024, FF_WORKERS=4, MPI_PROCESSES=2"

    sleep 1

    printf "\n\nRunning tests for sequential...\n"

    ./sequential 1024 || { echo "Error while executing the sequential version"; exit 1; }

    printf "\n\nRunning tests for fastflow...\n"

    ./fastflow 1024 4 || { echo "Error while executing the fastflow version"; exit 1; }

    printf "\n\nRunning tests for mpi...\n"

    srun --mpi=pmix --nodes 2 ./mpi 1024 || { echo "Error while executing the mpi version"; exit 1; }

    printf "\nTests completed.\n"

else
    echo "\nSkipping tests."
    
fi
