#include "includes/matrix.h"
#include <stdio.h>
#include <stdlib.h>

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

double **allocate_memory_for_matrix(int number_of_rows, int number_of_columns) {
  double** matrix_data = calloc(number_of_rows, sizeof(double*));

  for (int i = 0; i < number_of_rows; ++i) {
    matrix_data[i] = calloc(number_of_columns, sizeof(double));
  }

  return matrix_data;
}