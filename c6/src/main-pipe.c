#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_LENGTH 1000000
#define CHUNK_SIZE 1000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size_of_cluster, rank_of_process;
    
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_of_process);

    MPI_Comm comm;
    int dims[] = {size_of_cluster};
    int periods[] = {false};
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, true, &comm);
    
    int comm_rank;
    MPI_Comm_rank(comm, &comm_rank);

    int previous_process_rank, next_process_rank;
    MPI_Cart_shift(comm, 0, 1, &previous_process_rank, &next_process_rank);

    int *vector;
    
    if (comm_rank == 0) {
        vector = calloc(VECTOR_LENGTH, sizeof(int));
    }

    long final_sum = 0;
    
    for (int i = 0; i < VECTOR_LENGTH / CHUNK_SIZE; ++i) {
        int part[CHUNK_SIZE];
        
        if (comm_rank == 0) {
            int offset = i * CHUNK_SIZE;
            memcpy(part, vector + offset, CHUNK_SIZE * sizeof(int));
        }

        MPI_Recv(part, CHUNK_SIZE, MPI_INT, previous_process_rank, 0, comm, MPI_STATUS_IGNORE);

        if (comm_rank != 0 && comm_rank != size_of_cluster - 1) {
            for (int i = 0; i < CHUNK_SIZE; ++i) {
                part[i] = part[i] + 1;
            }
        }

        if (comm_rank == size_of_cluster - 1) {
            for (int i = 0; i < CHUNK_SIZE; ++i) {
                final_sum += part[i];
            }
        }

        MPI_Send(part, CHUNK_SIZE, MPI_INT, next_process_rank, 0, comm);
    }

    if (comm_rank == size_of_cluster - 1) {
        printf("Final sum of the chunked vector is=[%ld]\n", final_sum);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}