#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
	
#define MAX 10

typedef double myfloat;

static myfloat *vector;

void matrix_mult(myfloat *first, myfloat *second, myfloat *multiply, int first_rows, int first_cols, int second_cols)
{

	#pragma offload target(mic) in(first:length(first_rows*first_cols)) in(second:length(first_cols*second_cols)) out(multiply:length(first_rows*second_cols))
	{
		int i, j, k;
		for (i=0; i<first_rows; i++) {
			for (j=0; j<second_cols; j++)
				multiply[i*second_cols+j] = 0.0;
		}
		#pragma omp parallel for private(i, j, k)
		for (i=0; i<first_rows; i++) {
			for (k=0; k<first_cols; k++) {
				#pragma vector aligned
				#pragma simd
				for (j=0; j<second_cols; j++) {
					multiply[i*second_cols+j] += first[i*first_cols+k]*second[k*second_cols+j];
				}
			}
		}
	}
}

double sum(double *v, int n){
	int i;
	double sum = 0.0;

	#pragma offload target(mic) in(v:length(n))
	{
		#pragma vector aligned
		#pragma simd reduction(+:sum)
		for (i=0; i<n; i++)
			sum += v[i];
	}
	return sum;
}

void vector_create(int n, double* vector){
	
	int i;
	for(i=0; i<n; i++)
		vector[i] = rand()%MAX;
}

void main (int argc, char** argv){

	if(argc != 2)
		exit(0);
	
	int size = atoi(argv[1]);
	double* vector = malloc(sizeof(double)*size);
	int i;

	srand(time(NULL));
	printf("Tamanho:  %d\n", size);

	
	vector_create(size, vector);
	

	printf("Vetor 1: ");
	for(i=0; i<size; i++)
		printf("%.2lf ", vector[i]);

	printf("\nSoma dos elementos do vetor: %.2lf\n", sum(vector, size));
	
}
