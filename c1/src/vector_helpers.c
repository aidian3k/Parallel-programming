#include "includes/vector_helpers.h"
#include <stdio.h>
#include <stdlib.h>

double calculate_range_sum(double* vector, int staring_index, int ending_index) {
  double sum = 0;

  for(int i = staring_index; i < ending_index; ++i) {
    sum += vector[i];
  }

  return sum;
}

double* read_vector_from_file(char* file_name, int* vector_size) {
  FILE* file = fopen(file_name, "r");
  
  fscanf(file, "%d", vector_size);
  double* vector = malloc(sizeof(double) * (*vector_size));
  int vector_iterator = 0;

  while(fscanf(file, "%lg", &vector[vector_iterator++]) != EOF && vector_iterator < *vector_size) {}
  
  fclose(file);
  
  return vector;
}

double calculate_final_sum_from_array(double* array, int n) {
  double final_sum = 0;
  
  for(int i = 0; i < n; ++i) {
    final_sum += array[i];
  }
  
  return final_sum;
}

void check_validity_of_read_file(FILE* file) {
  if(file == NULL) {
    fprintf(stderr, "There is a problem with reading the file!");
    exit(EXIT_FAILURE);
  }
}

void fill_index_ranges_array(int* index_array, int n, int vector_size) {
  int step_size = vector_size / n;
  int index;

  for (index = 0; index < n + 1; ++index) {
    int starting_point = index == n ? vector_size : index * step_size;
    index_array[index] = starting_point;
  }
}