#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BUFFOR_SIZE 80

double sum(double* vector, int n) {
	int i;
	double sum = 0.0f;
	for(i=0; i<n; i++) {
		sum += vector[i];
	}
	return sum;
}

int main(int argc, char **argv) { 
	struct timespec start, end;
	
	FILE* f = fopen("vector.dat", "r");
    char buffor[BUFFOR_SIZE+1];
	double* vector;
	int n;
	int i;
	fgets(buffor, BUFFOR_SIZE, f);
 	n = atoi(buffor);
	vector = malloc(sizeof(double) * n);
	printf("Vector has %d elements\n", n);
	for(i=0; i<n; i++) {
		fgets(buffor, BUFFOR_SIZE, f);
		vector[i] = atof(buffor);
	}
	fclose(f);

	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	double final_sum; 
	for(i=0; i<1000000; i++) {
		final_sum = sum(vector, n);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

	printf("The final results of the sum is: %g\n", final_sum);
    printf("Time:  %f\n", execution_time);

	return 0;
}