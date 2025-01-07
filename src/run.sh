#!/bin/bash

make all
if [ $? -ne 0 ]; then
  echo "Build failed. Please fix the errors and try again."
  exit 1
fi

if [ $# -lt 2 ]; then
  echo "Usage: ./run <target: sequential|fastflow|mpi> <parameters>"
  echo "Parameters:"
  echo "  sequential: <matrix size>"
  echo "  fastflow: <matrix size> <num workers>"
  echo "  mpi: <matrix size> <num threads> <num nodes>"
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
    ./sequential "${PARAMS[0]}"
    ;;
  fastflow)
    if [ ${#PARAMS[@]} -ne 2 ]; then
      echo "Usage for fastflow: ./run fastflow <matrix size> <num workers>"
      exit 1
    fi
    ./fastflow "${PARAMS[0]}" "${PARAMS[1]}"
    ;;
  mpi)
    if [ ${#PARAMS[@]} -ne 3 ]; then
      echo "Usage for mpi: ./run mpi <matrix size> <num threads> <num nodes>"
      exit 1
    fi
    OMP_NUM_THREADS="${PARAMS[1]}" srun --mpi=pmix --nodes="${PARAMS[2]}" ./mpi "${PARAMS[0]}"
    ;;
  *)
    echo "Invalid target: $TARGET"
    echo "Valid targets are: sequential, fastflow, mpi"
    exit 1
    ;;
esac
