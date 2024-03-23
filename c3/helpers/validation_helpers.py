import ipaddress

from c3.helpers.constants import ExitConstatns


class ValidationHelpers:
    @staticmethod
    def check_client_input_arguments(arguments: list) -> None:
        if len(arguments) < 6:
            print("The input to the program should look like that: [matrix_file_path], [vector_file_path], "
                  "[ip_address], [port]")
            exit(ExitConstatns.EXIT_FAILURE)
        
        ip_address_index = 3
        ValidationHelpers.__check_if_address_ip_is_valid(arguments[ip_address_index])

    @staticmethod
    def check_server_input_arguments(arguments: list) -> None:
        if len(arguments) < 3:
            print("The input to the server program should look like that: [ip_adress], [port]")

    @staticmethod
    def __check_if_address_ip_is_valid(ip_address: str) -> None:
        try:
            ipaddress.ip_address(ip_address)
        except ValueError:
            print("You should pass valid ip_address to start the program!")
            exit(ExitConstatns.EXIT_FAILURE)
