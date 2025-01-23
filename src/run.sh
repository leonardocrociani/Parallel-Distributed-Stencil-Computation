#!/bin/bash

srun make || { echo "Error during source building..."; exit 1; }
if [ $? -ne 0 ]; then
  echo "Build failed. Please fix the errors and try again."
  exit 1
fi

if [ $# -lt 2 ]; then
  echo "Usage: ./run <target: sequential|fastflow|mpi> <parameters>"
  echo "Parameters:"
  echo "  sequential: <matrix size>"
  echo "  fastflow: <matrix size> <num workers>"
  echo "  mpi: <matrix size> <num processes> <num nodes>"
  exit 1
fi

TARGET=$1
shift
PARAMS=("$@")

case $TARGET in
  sequential)
    if [ ${#PARAMS[@]} -ne 1 ]; then
      echo "Usage for sequential: ./run sequential <matrix size>"
      exit 1
    fi
    srun --nodes=1 --ntasks=1 --cpus-per-task=1 ./sequential "${PARAMS[0]}"
    ;;
  fastflow)
    if [ ${#PARAMS[@]} -ne 2 ]; then
      echo "Usage for fastflow: ./run fastflow <matrix size> <num workers>"
      exit 1
    fi
    srun --nodes=1 --ntasks=1 --cpus-per-task="$(echo "${PARAMS[1]} + 1" | bc)" ./fastflow "${PARAMS[0]}" "${PARAMS[1]}"
    ;;
  mpi)
    if [ ${#PARAMS[@]} -ne 3 ]; then
      echo "Usage for mpi: ./run mpi <matrix size> <num processes> <num nodes>"
      exit 1
    fi
    srun --mpi=pmix --nodes="${PARAMS[2]}" --ntasks="${PARAMS[1]}" ./mpi "${PARAMS[0]}"
    ;;
  *)
    echo "Invalid target: $TARGET"
    echo "Valid targets are: sequential, fastflow, mpi"
    exit 1
    ;;
esac
