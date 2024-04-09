#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double function(double x) {
    return x * x;
}

double integrate(double (*f)(double), double begin, double end, int number_of_points) {
    double h = (end - begin) / number_of_points;
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
    //printf("begin: %g, end: %g, points: %d, process: %d\n", begin_of_range_for_process, end_of_range_for_process, points_for_each_process, process_rank);

    MPI_Reduce(&single_process_integration_result, &global_integration_result, NUMBER_OF_ELEMENTS_IN_BUFFER, MPI_DOUBLE, MPI_SUM, RANK_OF_ROOT_PROCESS, MPI_COMM_WORLD);

    if (process_rank == RANK_OF_ROOT_PROCESS) {
        printf("Final result of integrating function from range [%g] to [%g] is: %g", begin_of_integration_range, end_of_integration_range, global_integration_result);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}