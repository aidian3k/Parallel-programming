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


int main(int argc, char** argv) {
    if(argc < 4) {
        fprintf(stderr, "There was a problem with the input arguments: [%s]\n", argv[0]);
        fprintf(stderr, "Program should get: [begin_of_integration_range]: double, [end_of_integration_range]: double, [number_of_points]: int");
        exit(EXIT_FAILURE);
    }

    MPI_Init(&argc, &argv);

    int process_rank, size_of_cluster;
    const int RANK_OF_ROOT_PROCESS = 0;
    const int NUMBER_OF_ELEMENTS_IN_BUFFER = 1;
}