#ifndef SIGNAL_HELPERS_H
#define SIGNAL_HELPERS_H

void send_signal_to_child_processes(int* created_processes_ids, int number_of_processes, int signal_number);
void wait_for_child_processes_to_stop(int number_of_created_processes);
int create_memory_segment(key_t key, int size);
void detach_results_indexes_from_process(int* index_array, double* results);
void delete_memory_segment(int memory_id);

#endif