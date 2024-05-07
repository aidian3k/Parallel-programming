#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

int pow_i(int base, int exp) {
    int result = 1;
    while (exp--) {
        result *= base;
    }
    return result;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int size_of_cluster, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    
    if (process_rank == 0 && argc - 1 != size_of_cluster) {
        fprintf(stderr, "Number of arguments must match number of procs\n");
        return EXIT_FAILURE;
    }
    
    int *arg_numbers = calloc(argc - 1, sizeof(int));
    
    for (int i = 0; i < argc - 1; ++i) {
        arg_numbers[i] = atoi(argv[i + 1]);
    }

    int total_number_of_steps = log2(size_of_cluster);
    int local_number = arg_numbers[process_rank];
    
    for (int i = 0; i < total_number_of_steps; ++i) {
        int offset = pow_i(2, i);
        int rank_of_sending_process = (process_rank + offset) % size_of_cluster;
        int receiving_rank_process = process_rank - offset;
        
        if (receiving_rank_process < 0) {
            receiving_rank_process += size_of_cluster;
        }
        int partner_computed_value;
        
        MPI_Sendrecv(&local_number, 1, MPI_INT, rank_of_sending_process, 0, &partner_computed_value, 1, MPI_INT,
                     receiving_rank_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        local_number = gcd(local_number, partner_computed_value);
    }
    
    if (process_rank == 0) {
        printf("Greatest common divisor=[%d]\n", local_number);
    }
    
    MPI_Finalize();
    
    return EXIT_SUCCESS;
}