from benchmark import benchmark
from measure import measure
from config import MUST_EXISTS, FOLDER_TO_CREATE
import os

def warm_up():
    print("Warming up...")
    
    for file in MUST_EXISTS:
        if not os.path.exists(file):
            raise FileNotFoundError(f"File {file} not found")
        
    for folder in FOLDER_TO_CREATE:
        if not os.path.exists(folder):
            os.makedirs(folder)

    if 0 != os.system('cd ../src && make clean && make all'):
        raise Exception("Error compiling code")
    
    os.system('clear')
    print("✅ Warm up finished")
    
if __name__ == "__main__":
    warm_up()
    print("Running benchmark...")
    benchmark()
    print("✅ Benchmark finished")
    print("Measuring results...")
    measure()
    print("✅ Measuring finished")