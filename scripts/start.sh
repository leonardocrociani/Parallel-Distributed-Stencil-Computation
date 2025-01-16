#!/bin/bash

current_dir=$(basename "$PWD")

if [ "$current_dir" != "scripts" ]; then
    echo "You must navigate to the 'scripts' directory using 'cd scripts'."
    exit 1
fi

python3 -m venv venv && source venv/bin/activate && pip install -r requirements.txt

clear

echo "Environment created. Running tests and collecting results..."

sleep 1

TARGETS=("sequential" "mpi" "fastflow")

for target in "${TARGETS[@]}"
do
    clear

    echo "Running tests for $target..."

    if [[ $? -ne 0 ]]; then
        echo "Failed to allocate resources for $target. Skipping..."
        continue
    fi

    python "$target.measure.py"
    python "$target.plot.py"

    echo "Tests for $target completed."

    scancel -u "$USER" &
    sleep 5
done

clear
echo "All tests completed. Exiting..."
