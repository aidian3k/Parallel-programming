def read_vector_of_file_path(vector_file_path: str) -> list[float]:
    with open(vector_file_path, "r") as vector_file:
        number_of_columns: int = int(vector_file.read())
        int(vector_file.read())

        return [float(vector_file.read()) for _ in range(number_of_columns)]


def read_matrix_of_file_path(matrix_file_path: str) -> list[list[float]]:
    with open(matrix_file_path, "r") as matrix_file:
        number_of_rows: int = int(matrix_file.readline())
        number_of_columns: int = int(matrix_file.readline())
        read_matrix = []

        for row in range(number_of_rows):
            single_row = []

            for column in range(number_of_columns):
                single_row.append(float(matrix_file.read()))

            read_matrix.append(single_row)

        return read_matrix


def get_matrix_number_of_columns(matrix: list[list[float]]):
    if len(matrix) < 1:
        raise ValueError("Cannot execute command while number of elements is 0")

    return len(matrix[0])


def get_matrix_number_of_rows(matrix: list[list[float]]):
    return len(matrix)
