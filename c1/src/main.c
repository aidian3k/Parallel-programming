#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "includes/vector_helpers.h"
#include "includes/signal_helpers.h"
#include "includes/validators.h"

int n;
double* vector;
int processes_ready_counter = 0;
const char* file_name = "c1";

void handle_usr1_signal(int signal_number) {
    int current_process_id = getpid() % n;

    key_t index_memory_key = ftok(file_name, 66);
    int indexes_memory_id = create_memory_segment(index_memory_key, sizeof(int) * (n + 1));

    int* index_array;
    if((index_array = (int*) shmat(indexes_memory_id,(void*)0, 0)) < 0) {
      fprintf(stderr, "shmat failed");
      exit(EXIT_FAILURE);
    }

    key_t results_memory_key = ftok(file_name, 65);
    int results_memory_id = create_memory_segment(results_memory_key, sizeof(double) * n);
    double* results;

    if((results = (double*) shmat(results_memory_id,(void*)0, 0)) < 0) {
      fprintf(stderr, "shmat failed");
      exit(EXIT_FAILURE);
    }

    results[current_process_id] = calculate_range_sum(vector, index_array[current_process_id], index_array[current_process_id + 1]);
    detach_results_indexes_from_process(index_array, results);
}

void handle_usr2_signal(int signal_number) {
  processes_ready_counter++;
}

int main(int argc, char **argv) {
    check_validity_of_input_arguments(argc);
    n = atoi(argv[1]);
    check_validity_of_number_of_processes(n);
    
    int vector_size = 0;
    vector = read_vector_from_file(argv[2], &vector_size);

    pid_t current_process_id;
    pid_t* created_process_ids = calloc(n, sizeof(pid_t));
    int created_processes_counter = 0;

    struct sigaction process_ready_action;
    sigset_t block_mask;
    sigemptyset (&block_mask);
    process_ready_action.sa_mask = block_mask;
    process_ready_action.sa_flags = 0;
    process_ready_action.sa_handler = handle_usr2_signal;
    sigaction (SIGUSR2, &process_ready_action, 0);

    for(int i = 0; i < n; ++i) {
        current_process_id = fork();

        if(current_process_id < 0) {
            fprintf(stderr, "There was a problem with creating the process in fork!");
            
            return EXIT_FAILURE;
        } else if(current_process_id == 0) {
            struct sigaction setup_action;
            sigset_t block_mask;
            sigemptyset (&block_mask);
            setup_action.sa_mask = block_mask;
            setup_action.sa_flags = 0;
            setup_action.sa_handler = handle_usr1_signal;
            sigaction (SIGUSR1, &setup_action, 0);

            kill(getppid(), SIGUSR2);
            pause();

            return EXIT_SUCCESS;
        } else {
            created_process_ids[created_processes_counter++] = current_process_id;
        }
    }
    
    
    key_t index_memory_key = ftok(file_name, 66);
    int indexes_memory_id = create_memory_segment(index_memory_key, sizeof(int) * (n + 1));

    int* index_array;
    if((index_array = (int*) shmat(indexes_memory_id,(void*)0, 0)) < 0) {
        fprintf(stderr, "shmat failed");
        exit(EXIT_FAILURE);
    }

    fill_index_ranges_array(index_array, n, vector_size);
    
    key_t results_memory_key = ftok(file_name, 65);
    int results_memory_id = create_memory_segment(results_memory_key, sizeof(double) * n);
    
    double* results;
    if((results = (double*) shmat(results_memory_id,(void*)0, 0)) < 0) {
        fprintf(stderr, "shmat failed");
        exit(EXIT_FAILURE);
    }

    while(processes_ready_counter != n) {}

    send_signal_to_child_processes(created_process_ids, n, SIGUSR1);
    wait_for_child_processes_to_stop(n);
    
    double final_sum = calculate_final_sum_from_array(results, n);

    printf("The final results of the sum is: %g", final_sum);
    
    detach_results_indexes_from_process(index_array, results);
    delete_memory_segment(results_memory_id);
    delete_memory_segment(indexes_memory_id);

    return EXIT_SUCCESS;
}