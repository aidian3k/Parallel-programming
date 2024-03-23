from multiprocessing import Queue
from multiprocessing.managers import BaseManager


class QueueManager(BaseManager):
    pass


class SingleTask:
    def __init__(self, starting_row: int, ending_row: int, matrix_rows: list[list[float]], vector: list[float]):
        self.matrix_rows = matrix_rows
        self.vector = vector
        self.starting_row = starting_row
        self.ending_row = ending_row


class SingleResult:
    def __init__(self, starting_row: int, ending_row: int, single_result: list[list[float]]):
        self.starting_row = starting_row
        self.ending_row = ending_row
        self.single_result = single_result


def get_base_queue_manager(ip_address: str = "127.0.0.1", port: int = 8080,
                           authoriztion_key: bytes = b"root") -> QueueManager:
    QueueManager.register("results_queue")
    QueueManager.register("tasks_queue")

    return QueueManager(address=(ip_address, port), authkey=authoriztion_key)


def execute_single_work(tasks_queue: Queue[SingleTask], results_queue: Queue[SingleResult]) -> None:
    while not tasks_queue.empty():
        current_task: SingleTask = tasks_queue.get()
        current_submatrix_result: list[list[float]] = []
        
        for row in range(len(current_task.matrix_rows)):
            current_row = current_task.matrix_rows[row]
            single_row_result: list[float] = [current_row[i] * current_task.vector[i] for i in range(len(current_task.vector))]
            current_submatrix_result.append(single_row_result)
            
        results_queue.put(SingleResult(current_task.starting_row, current_task.ending_row, current_submatrix_result))
