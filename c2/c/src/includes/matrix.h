#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  double** matrix_data;
  int number_of_rows;
  int number_of_columns;
} matrix_t;

matrix_t* load_matrix_from_file(const char* file_name);
void free_alocated_matrix_memory(matrix_t* matrix);
void print_matrix(matrix_t* matrix);
double **allocate_memory_for_matrix(int number_of_rows, int number_of_columns);

#endif