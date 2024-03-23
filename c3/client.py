from multiprocessing import Queue

from c3.helpers.matrix_helpers import read_matrix_of_file_path, read_vector_of_file_path, get_matrix_number_of_rows
from c3.helpers.queue_helpers import get_base_queue_manager, SingleTask, SingleResult


class MatrixMultiplicationClient:
    def __init__(self, matrix_file_path: str, vector_file_path: str, ip_address: str = "localhost", port: int = 8080, authorization_key: bytes = b"root", number_of_tasks: int = 4):
        self.__number_of_tasks = number_of_tasks
        self.__manager = get_base_queue_manager(ip_address, port, authorization_key)
        self.__manager.connect()
        self.__get_matrix_and_vector_data(matrix_file_path, vector_file_path)
        self.__partition_data_and_send_to_tasks_queue()
    
    def __get_matrix_and_vector_data(self, matrix_file_path: str, vector_file_path: str):
        self.__matrix = read_matrix_of_file_path(matrix_file_path)
        self.__vector = read_vector_of_file_path(vector_file_path)
        
    def __partition_data_and_send_to_tasks_queue(self) -> None:
        step_per_each_task: int = get_matrix_number_of_rows(self.__matrix) / self.__number_of_tasks
        tasks: Queue[SingleTask] = self.__manager.tasks_queue()
        
        for index in range(self.__number_of_tasks):
            starting_row: int = index * step_per_each_task
            ending_row: int = get_matrix_number_of_rows(self.__matrix) if index == self.__number_of_tasks - 1 else (index + 1) * step_per_each_task
            tasks.put(SingleTask(starting_row, ending_row, self.__matrix[starting_row:ending_row], self.__vector))
    
    def gather_results_from_workers(self) -> list[float]:
        results_queue_from_workers: Queue[SingleResult] = self.__manager.results_queue()
        result = [.0] * len(self.__vector)
        
        while not results_queue_from_workers.empty():
            current_single_result: SingleResult = results_queue_from_workers.get()
            result[current_single_result.starting_row:current_single_result.ending_row] = current_single_result.single_result
        
        return result
        
        
            
        