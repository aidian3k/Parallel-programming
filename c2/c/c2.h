#ifndef C2_H
#define C2_H

typedef struct {
    double** matrix_data;
    int number_of_rows;
    int number_of_columns;
} matrix_t;

typedef struct {
  int starting_row;
  int ending_row;
} thread_work_t;

matrix_t* load_matrix_from_file(const char* file_name);
void free_alocated_matrix_memory(matrix_t* matrix);
double** allocate_memory_for_matrix(int number_of_columns, int number_of_rows);
void print_matrix(matrix_t* matrix);
void multiply_matrixes_parrarel(int number_of_threads);
void* single_multiply_task(void *args);

#endif