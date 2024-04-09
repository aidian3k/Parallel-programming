import sys

from client import MatrixMultiplicationClient
from helpers.validation_helpers import ValidationHelpers
from worker import MatrixMultiplicationWorker
import time

if __name__ == "__main__":
    ValidationHelpers.check_client_input_arguments(sys.argv)
    
    matrix_file_path: str = sys.argv[1]
    vector_file_path: str = sys.argv[2]
    ip_address: str = sys.argv[3]
    port: int = int(sys.argv[4])
    number_of_tasks: int = int(sys.argv[5])

    start_time = time.time()
    
    client: MatrixMultiplicationClient = MatrixMultiplicationClient(matrix_file_path, vector_file_path, ip_address, port, number_of_tasks=number_of_tasks)
    worker: MatrixMultiplicationWorker = MatrixMultiplicationWorker(ip_address, port)
    
    worker.start_working()
    worker.wait_until_processes_stop_working()
    
    end_time = time.time()

    elapsed_time = end_time - start_time
    print(f"Matrix multiplication completed in {elapsed_time:.2f} seconds.")

    with open("result_vector.dat", "w") as result_file:
        result_file.write(str(client.gather_results_from_workers()))
     