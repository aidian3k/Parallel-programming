#include "includes/c2.h"
#include "includes/matrix.h"
#include "includes/validators.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

matrix_t *first_matrix;
matrix_t *second_matrix;
matrix_t *multiply_result_matrix;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double multiply_matrix_squared_sum = 0;

int main(int argc, char **argv) {
  validate_input_arguments(argc);
  int number_of_threads = atoi(argv[1]);

  first_matrix = load_matrix_from_file(argv[2]);
  second_matrix = load_matrix_from_file(argv[3]);

  printf("Printing first matrix: A\n");
  print_matrix(first_matrix);
  printf("\n");

  printf("Printing second matrix: B\n");
  print_matrix(second_matrix);
  printf("\n");

  multiply_matrixes_parrarel(number_of_threads);

  printf("Printing result multiply matrix: B\n");
  print_matrix(multiply_result_matrix);
  printf("\n");

  printf("Frobenius sum of the multiplied array is: %g\n",
         sqrt(multiply_matrix_squared_sum));

  return EXIT_SUCCESS;
}

void *single_multiply_task(void *args) {
  thread_work_t *thread_work = (thread_work_t *)args;
  double frobenius_thread_result = 0;
  int output_matrix_columns = multiply_result_matrix->number_of_columns;

  for (int cell = thread_work->starting_cell; cell < thread_work->ending_cell;
       ++cell) {
    int row = cell / output_matrix_columns;
    int column = cell % output_matrix_columns;

    for (int k = 0; k < second_matrix->number_of_rows; ++k) {
      multiply_result_matrix->matrix_data[row][column] +=
          first_matrix->matrix_data[row][k] *
          second_matrix->matrix_data[k][column];
    }

    frobenius_thread_result +=
        pow(multiply_result_matrix->matrix_data[row][column], 2);
  }

  pthread_mutex_lock(&mutex);
  multiply_matrix_squared_sum += frobenius_thread_result;
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void multiply_matrixes_parrarel(int number_of_threads) {
  validate_if_multiplication_is_valid(first_matrix->number_of_columns,
                                      second_matrix->number_of_rows);

  multiply_result_matrix = malloc(sizeof(matrix_t));
  multiply_result_matrix->number_of_rows = first_matrix->number_of_rows;
  multiply_result_matrix->number_of_columns = second_matrix->number_of_columns;

  double **result_data =
      allocate_memory_for_matrix(multiply_result_matrix->number_of_rows,
                                 multiply_result_matrix->number_of_columns);
  multiply_result_matrix->matrix_data = result_data;

  pthread_t *thread_pool = calloc(number_of_threads, sizeof(pthread_t));
  int cells_of_output_matrix = multiply_result_matrix->number_of_rows *
                               multiply_result_matrix->number_of_columns;
  int cells_per_thread = (cells_of_output_matrix) / number_of_threads;
  thread_work_t **thread_works =
      calloc(number_of_threads, sizeof(thread_work_t *));

  int starting_cell_work = 0;
  int ending_cell_work;

  for (int i = 0; i < number_of_threads; ++i) {
    thread_works[i] = malloc(sizeof(thread_work_t));
    thread_works[i]->starting_cell = i * cells_per_thread;
    thread_works[i]->ending_cell = i == number_of_threads - 1
                                       ? cells_of_output_matrix
                                       : (i + 1) * cells_per_thread;

    pthread_create(&thread_pool[i], NULL, single_multiply_task,
                   (void *)thread_works[i]);
  }

  for (int i = 0; i < number_of_threads; ++i) {
    pthread_join(thread_pool[i], NULL);
  }

  for (int i = 0; i < number_of_threads; ++i) {
    free(thread_works[i]);
  }

  free(thread_pool);
  free(thread_works);
}

void validate_if_multiplication_is_valid(int first_matrix_column_number,
                                         int second_matrix_row_number) {
  if (first_matrix_column_number != second_matrix_row_number) {
    fprintf(stderr,
            "There is an error in matrix_data. You cannot mulitply matrixes "
            "when columns of first != number of rows of second");
    free_alocated_matrix_memory(first_matrix);
    free_alocated_matrix_memory(second_matrix);

    exit(EXIT_FAILURE);
  }
}