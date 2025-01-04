#!/bin/bash

current_dir=$(basename "$PWD")

if [ "$current_dir" != "scripts" ]; then
    echo "You must navigate to the 'scripts' directory using 'cd scripts'."
    exit 1
fi

python3 -m venv venv && source venv/bin/activate &&  pip install -r requirements.txt

clear

echo "Environment created. Running tests and collecting results..."

sleep 1

TARGETS=("sequential" "fastflow" "mpi")

for target in "${TARGETS[@]}"
do
    clear

    echo "Running tests for $target..."

    sleep 1

    python "$target".measure.py
    python "$target".plot.py

    echo "Tests for $target completed."
done

clear
echo "All tests completed. Exiting..."