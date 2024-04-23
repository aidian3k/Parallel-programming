#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double function(double x) {
    return x * x;
}

double integrate(double (*f)(double), double begin, double ending, int number_of_points) {
    double h = (ending - begin) / number_of_points;
    double sum = 0;
    for (int i = 1; i <= number_of_points; ++i) {
        sum += f(begin + i * h);
    }

    return sum * h;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "There was a problem with the input arguments: [%s]\n", argv[0]);
        fprintf(stderr, "Program should get: [begin_of_integration_range]: double, [end_of_integration_range]: double, [number_of_points]: int");
        exit(EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);

    int process_rank, size_of_cluster;
    const int RANK_OF_ROOT_PROCESS = 0;
    const int NUMBER_OF_ELEMENTS_IN_BUFFER = 1;

    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    double begin_of_integration_range = atof(argv[1]);
    double end_of_integration_range = atof(argv[2]);
    int number_of_points_to_integrate = atoi(argv[3]);
    double global_dx = (end_of_integration_range - begin_of_integration_range) / number_of_points_to_integrate;

    double global_integration_result = 0.0;

    int points_for_each_process = number_of_points_to_integrate / size_of_cluster;
    double begin_of_range_for_process = begin_of_integration_range + process_rank * points_for_each_process * global_dx;
    double end_of_range_for_process = begin_of_range_for_process + points_for_each_process * global_dx;
    double single_process_integration_result = integrate(function, begin_of_range_for_process, end_of_range_for_process, points_for_each_process);
    //printf("Proccess with rank: %d from %g to %g, result=%g\n", process_rank, begin_of_range_for_process, end_of_range_for_process, single_process_integration_result);
    MPI_Request request;
    double single_result;
    MPI_Status status;
    
    if (process_rank != RANK_OF_ROOT_PROCESS) {
        MPI_Isend(&single_process_integration_result, NUMBER_OF_ELEMENTS_IN_BUFFER, MPI_DOUBLE, RANK_OF_ROOT_PROCESS, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    } else {
        double final_integration_result = single_process_integration_result;

        for (int i = 1; i < size_of_cluster; i++) {
            MPI_Irecv(&single_result, NUMBER_OF_ELEMENTS_IN_BUFFER, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            final_integration_result += single_result;
        }
        
        printf("Final result of integrating function from range [%g] to [%g] is: %g\n", begin_of_integration_range, end_of_integration_range, final_integration_result);
    }
    
    MPI_Finalize();

    return EXIT_SUCCESS;
}