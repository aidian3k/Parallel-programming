#ifndef VECTOR_HELPERS_H
#define VECTOR_HELPERS_H

double calculate_range_sum(double* vector, int staring_index, int ending_index);
double* read_vector_from_file(char* file_name, int* vector_size);
double calculate_final_sum_from_array(double* array, int n);
void fill_index_ranges_array(int* index_array, int n, int vector_size);

#endif