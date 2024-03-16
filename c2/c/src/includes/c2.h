#ifndef C2_H
#define C2_H

typedef struct {
  int starting_cell;
  int ending_cell;
} thread_work_t;

void validate_if_multiplication_is_valid(int first_matrix_column_number, int second_matrix_row_number);
void multiply_matrixes_parrarel(int number_of_threads);
void* single_multiply_task(void *args);

#endif