from helpers.constants import QueueNames
from helpers.queue_helpers import QueueManager, SingleTask, SingleResult
from multiprocessing import Queue
import sys

from helpers.validation_helpers import ValidationHelpers


class MatrixMultiplicationServer:
    def __init__(self, ip_address: str = "127.0.0.1", port: int = 8080, authorization_key: bytes = b"root"):
        self.__server_manager = QueueManager(address=(ip_address, port), authkey=authorization_key)
        self.__create_queues()
        self.__register_queues()

    def __register_queues(self):
        self.__server_manager.register(QueueNames.RESULTS_QUEUE_NAME, callable=lambda: self.__results)
        self.__server_manager.register(QueueNames.TASKS_QUEUE_NAME, callable=lambda: self.__tasks)

    def __create_queues(self):
        self.__results: Queue[SingleTask] = Queue()
        self.__tasks: Queue[SingleResult] = Queue()

    def start_server(self):
        self.__server_manager.get_server().serve_forever()


if __name__ == "__main__":
    ValidationHelpers.check_server_input_arguments(sys.argv)

    ip_address: str = sys.argv[1]
    port: int = int(sys.argv[2])
    authorization_key: bytes = b"root" if len(sys.argv) < 4 else bytes(sys.argv[3].encode('utf-8'))

    matrix_multiplication_server: MatrixMultiplicationServer = MatrixMultiplicationServer(ip_address, port,
                                                                                          authorization_key)
    matrix_multiplication_server.start_server()
