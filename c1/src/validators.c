#include <stdio.h>
#include <stdlib.h>
#include "includes/validators.h"

void check_validity_of_input_arguments(int argc) {
  if(argc != 3) {
    fprintf(stderr, "There should be 3 elements in the program to start! [Number of processess] and [file_name]\n");
    exit(EXIT_FAILURE);
  }
}

void check_validity_of_number_of_processes(int n) {
  if(n < 0) {
    fprintf(stderr, "There is problem with n value. It should be positive!");
    exit(EXIT_FAILURE);
  }
}