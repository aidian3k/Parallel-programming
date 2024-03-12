#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "includes/signal_helpers.h"
#include <sys/types.h>

void send_signal_to_child_processes(int* created_processes_ids, int number_of_processes, int signal_number) {
  for(int i = 0; i < number_of_processes; ++i) {
    kill(created_processes_ids[i], signal_number);
  }
}

void wait_for_child_processes_to_stop(int number_of_created_processes) {
  for(int i = 0; i < number_of_created_processes; ++i) {
    if(wait(NULL) < 0) {
      fprintf(stderr, "There was a problem with wait function!");
    }
  }
}

int create_memory_segment(key_t key, int size) {
  int memory_id;
  
  if ((memory_id = shmget(key, size, 0666 | IPC_CREAT)) == -1) {
    fprintf(stderr, "shmget failed");
    exit(EXIT_FAILURE);
  }
  
  return memory_id;
}

void detach_results_indexes_from_process(int* index_array, double* results) {
  if (shmdt(results) < 0){
    fprintf(stderr, "shmdt failed");
    exit(EXIT_FAILURE);
  }

  if (shmdt(index_array) < 0){
    fprintf(stderr, "shmdt failed");
    exit(EXIT_FAILURE);
  }
}

void delete_memory_segment(int memory_id) {
  if(shmctl(memory_id,IPC_RMID,NULL) < 0) {
    fprintf(stderr, "smctl failed");
    exit(EXIT_FAILURE);
  }
}