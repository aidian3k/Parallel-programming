#include "c2.h"
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
  if (argc < 4) {
    fprintf(stderr,
            "To run the program, you should provide: [number_of_threads], "
            "[first_matrix_file], [second_matrix_file]");
    return EXIT_FAILURE;
  }

  int number_of_threads = atoi(argv[1]);

  if (number_of_threads < 0) {
    fprintf(stderr, "The number of threads cannot be < 0");
    return EXIT_FAILURE;
  }

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
  
  printf("Frobenius sum of the multiplied array is: %g\n", sqrt(multiply_matrix_squared_sum));

  return EXIT_SUCCESS;
}

double **allocate_memory_for_matrix(int number_of_rows, int number_of_columns) {
  double** matrix_data = calloc(number_of_rows, sizeof(double*));
  
  for (int i = 0; i < number_of_rows; ++i) {
    matrix_data[i] = calloc(number_of_columns, sizeof(double));
  }

  return matrix_data;
}

matrix_t* load_matrix_from_file(const char *file_name) {
  FILE *file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "There was an issue with opening file with matrix!");
    exit(EXIT_FAILURE);
  }
 
  int number_of_rows;
  int number_of_columns;
  fscanf(file, "%d", &number_of_rows);
  fscanf(file, "%d", &number_of_columns);
  printf("%d %d\n", number_of_rows, number_of_columns);

  double **matrix_data =
      allocate_memory_for_matrix(number_of_rows, number_of_columns);

  for (int i = 0; i < number_of_rows; ++i) {
    for (int j = 0; j < number_of_columns; ++j) {
      fscanf(file, "%lf", &matrix_data[i][j]);
    }
  }

  fclose(file);

  matrix_t *matrix = malloc(sizeof(matrix_t));
  matrix->number_of_rows = number_of_rows;
  matrix->number_of_columns = number_of_columns;
  matrix->matrix_data = matrix_data;

  return matrix;
}

void print_matrix(matrix_t *matrix) {
  for (int i = 0; i < matrix->number_of_rows; ++i) {
    printf("[");

    for (int j = 0; j < matrix->number_of_columns; ++j) {
      printf("%6.4lf ", matrix->matrix_data[i][j]);
    }

    printf("\b]\n");
  }
}

void free_alocated_matrix_memory(matrix_t *matrix) {
  for (int i = 0; i < matrix->number_of_columns; ++i) {
    free(matrix->matrix_data[i]);
  }

  free(matrix->matrix_data);
  free(matrix);
}

void* single_multiply_task(void *args) {
  thread_work_t* thread_work = (thread_work_t *)args;

  for (int row = thread_work->starting_row; row < thread_work->ending_row;
       ++row) {
    for (int j = 0; j < second_matrix->number_of_columns; ++j) {
      for (int k = 0; k < second_matrix->number_of_rows; ++k) {
        multiply_result_matrix->matrix_data[row][j] +=
            first_matrix->matrix_data[row][k] *
            second_matrix->matrix_data[k][j];
      }
      
      pthread_mutex_lock(&mutex);
      multiply_matrix_squared_sum += pow(multiply_result_matrix->matrix_data[row][j], 2);
      pthread_mutex_unlock(&mutex);
    }
  }

  pthread_exit(NULL);
}

void multiply_matrixes_parrarel(int number_of_threads) {
  if (first_matrix->number_of_columns != second_matrix->number_of_rows) {
    fprintf(stderr,
            "There is an error in matrix_data. You cannot mulitply matrixes "
            "when columns of first != number of rows of second");
    free_alocated_matrix_memory(first_matrix);
    free_alocated_matrix_memory(second_matrix);

    exit(EXIT_FAILURE);
  }

  multiply_result_matrix = malloc(sizeof(matrix_t));
  multiply_result_matrix->number_of_rows = first_matrix->number_of_rows;
  multiply_result_matrix->number_of_columns = second_matrix->number_of_columns;

  double **result_data =
      allocate_memory_for_matrix(multiply_result_matrix->number_of_rows,
                                 multiply_result_matrix->number_of_columns);
  multiply_result_matrix->matrix_data = result_data;

  pthread_t *thread_pool = calloc(number_of_threads, sizeof(pthread_t));
  thread_work_t **thread_works =
      calloc(number_of_threads, sizeof(thread_work_t *));
  int single_thread_work_size =
      first_matrix->number_of_rows / number_of_threads;

  int starting_row;
  int ending_row;

  for (int i = 0; i < number_of_threads; ++i) {
    starting_row = i * single_thread_work_size;
    ending_row = i != number_of_threads - 1 ? (i + 1) * single_thread_work_size
                                            : first_matrix->number_of_rows;

    thread_works[i] = malloc(sizeof(thread_work_t));
    thread_works[i]->starting_row = starting_row;
    thread_works[i]->ending_row = ending_row;
  }

  for (int i = 0; i < number_of_threads; ++i) {
    pthread_create(&thread_pool[i], NULL, single_multiply_task,
                   (void *)thread_works[i]);
  }

  for (int i = 0; i < number_of_threads; ++i) {
    pthread_join(thread_pool[i], NULL);
  }

  for(int i = 0; i < number_of_threads; ++i) {
    free(thread_works[i]);
  }
  
  free(thread_pool);
  free(thread_works);
}