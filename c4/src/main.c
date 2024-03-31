#include "/opt/homebrew/Cellar/libomp/18.1.2/include/omp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    bool* erastotanes_sieve_result;
    int number_of_primes;
} Erastotanes_result;

typedef struct {
    int* values;
    int size;
} array;

Erastotanes_result* find_divison_prime_numbers(int maximum_found_number) {
    int maximum_prime_number_to_find = (int) sqrt(maximum_found_number);
    bool* erastotanes_sieve = calloc(maximum_prime_number_to_find + 1, sizeof(bool));
    
    for(int i = 2; i <= maximum_prime_number_to_find; ++i) {
        erastotanes_sieve[i] = true;
    }
    
    for(int i = 2; i * i <= maximum_prime_number_to_find; ++i) {
        if(!erastotanes_sieve[i]) {
            continue;
        }
        
        for(int j = i * i; j <= maximum_prime_number_to_find; j += i) {
            erastotanes_sieve[j] = false;
        }
    }
    
    int number_of_primes = 0;
    
    //#pragma omp for reduction()
    for(int i = 2; i <= maximum_prime_number_to_find; ++i) {
        if(erastotanes_sieve[i]) {
            number_of_primes++;
        }
    }
    
    Erastotanes_result* result = malloc(sizeof(Erastotanes_result));
    result->number_of_primes = number_of_primes;
    result->erastotanes_sieve_result = erastotanes_sieve;
    
    return result;
}

array* find_prime_numbers_of_erastotanes_result(Erastotanes_result* erastotanes_result, int maximum_found_number) {
    int* prime_numbers = malloc(erastotanes_result->number_of_primes * sizeof(int));
    int number_of_elements_in_sieve = (int) sqrt(maximum_found_number);
    int prime_number_iterator = 0;
    
    for(int i = 0; i <= number_of_elements_in_sieve; ++i) {
        if(erastotanes_result->erastotanes_sieve_result[i]) {
            prime_numbers[prime_number_iterator++] = i;
        }
    }
    
    array* prime_result_array = malloc(sizeof(array));
    prime_result_array->size = prime_number_iterator;
    prime_result_array->values=prime_numbers;
    
    return prime_result_array;
}

void write_final_results_to_file(array* final_found_prime_numbers) {
    FILE* out = fopen("prime_output.dat", "w");
    
    for(int i = 0; i < final_found_prime_numbers->size; ++i) {
        fprintf(out, "%d ", final_found_prime_numbers->values[i]);
    }
}

array* find_final_prime_numbers_of_given_range(int maximum_found_number, array* prime_numbers_of_dividers_range) {
    int sieve_size = (int) sqrt(maximum_found_number);
    int* final_prime_numbers = calloc(maximum_found_number, sizeof(int));
    int prime_counter = 0;
    
    for(int i = 0; i < prime_numbers_of_dividers_range->size; ++i) {
        final_prime_numbers[prime_counter++] = prime_numbers_of_dividers_range->values[i];
    }
    
    for(int i = sieve_size + 1; i <= maximum_found_number; ++i) {
        bool is_not_divided_by_any_of_dividers = true;
        int current_number = i;
        
        for(int j = 0; j < prime_numbers_of_dividers_range->size; ++j) {
            if(current_number % prime_numbers_of_dividers_range->values[j] == 0) {
                is_not_divided_by_any_of_dividers = false;
                break;
            }
        }
        
        if(is_not_divided_by_any_of_dividers) {
            final_prime_numbers[prime_counter++] = current_number;
        }
    }
    
    final_prime_numbers = realloc(final_prime_numbers, prime_counter * sizeof(int));
    array* final_prime_array = malloc(sizeof(array));
    final_prime_array->size = prime_counter;
    final_prime_array->values = final_prime_numbers;
    
    return final_prime_array;
}

int main(int argc, char** argv) {
   if(argc < 3) {
       fprintf(stderr, "There should be two arguments passed to the program: [maximum_found_number]: int [number_of_threads]: int");
       exit(EXIT_FAILURE);
   }
   
   int maximum_found_number = atoi(argv[1]);
   int number_of_threads = atoi(argv[2]);
   
   if(maximum_found_number <= 0 || number_of_threads <= 0 ) {
       fprintf(stderr, "There is wrong arguments passed to the program! Maximum found number should be >=0 and number_of_threads should be >= 0");
       exit(EXIT_FAILURE);
   }
   
   // omp_set_num_threads(number_of_threads); not working for arm architecture
   Erastotanes_result* erastotanes_result = find_divison_prime_numbers(maximum_found_number);
   array* prime_numbers_of_dividers_range = find_prime_numbers_of_erastotanes_result(erastotanes_result, maximum_found_number);
   array* final_found_prime_numbers = find_final_prime_numbers_of_given_range(maximum_found_number, prime_numbers_of_dividers_range);
   
   write_final_results_to_file(final_found_prime_numbers);
}