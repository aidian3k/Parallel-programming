#include "includes/validators.h"
#include <stdlib.h>
#include <stdio.h>

void validate_input_arguments(int argc) {
  if (argc < 4) {
    fprintf(stderr,
            "To run the program, you should provide: [number_of_threads], "
            "[first_matrix_file], [second_matrix_file]");
    
    exit(EXIT_FAILURE);
  }
}

void validate_number_of_threads(int number_of_threads) {
  if (number_of_threads < 0) {
    fprintf(stderr, "The number of threads cannot be < 0");
    exit(EXIT_FAILURE);
  }
}