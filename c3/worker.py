from multiprocessing import Queue, cpu_count, Process

from helpers.queue_helpers import get_base_queue_manager, SingleResult, SingleTask, execute_single_work


class MatrixMultiplicationWorker:
    def __init__(self, ip_address: str = "localhost", port: int = 8080, authorization_key: bytes = b"root"):
        self.__manager = get_base_queue_manager(ip_address, port, authorization_key)
        self.__manager.connect()
        self.__register_queues()
        self.__initialize_processes()
        
    def start_working(self) -> None:
        for process_number in range(len(self.__processes)):
            self.__processes[process_number].start()
    
    def wait_until_processes_stop_working(self) -> None:
        for process_number in range(len(self.__processes)):
            self.__processes[process_number].join()
    
    def __register_queues(self) -> None:
        self.__results_queue: Queue[SingleResult] = self.__manager.results_queue()
        self.__tasks_queue: Queue[SingleTask] = self.__manager.tasks_queue()
    
    def __initialize_processes(self) -> None:
        self.__processes: list[Process] = []
        
        for process_index in range(cpu_count()):
            current_process: Process = Process(target=execute_single_work, args=(self.__tasks_queue, self.__tasks_queue))
            self.__processes.append(current_process)
        
    
