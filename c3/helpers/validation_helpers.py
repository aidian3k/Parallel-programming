import ipaddress

from helpers.constants import ExitConstatns


class ValidationHelpers:
    @staticmethod
    def check_client_input_arguments(arguments: list) -> None:
        if len(arguments) < 6:
            print("The input to the program should look like that: [matrix_file_path], [vector_file_path], "
                  "[ip_address], [port]")
            exit(ExitConstatns.EXIT_FAILURE)

    @staticmethod
    def check_server_input_arguments(arguments: list) -> None:
        if len(arguments) < 3:
            print("The input to the server program should look like that: [ip_adress], [port]")
            exit(ExitConstatns.EXIT_FAILURE)
