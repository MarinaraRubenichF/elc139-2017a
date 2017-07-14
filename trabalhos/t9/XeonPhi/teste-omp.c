#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv){

	int myid, nthreads;
	int a[5] = {0,1,2,3,4};
	int b[5] = {0,1,2,3,4};
	int i, j, k, sum;

	#pragma omp parallel private(myid) shared(nthreads) 
	{
		myid = omp_get_thread_num();
		printf("Thread: %i\n", myid);
		if(myid == 0)
			nthreads = omp_get_num_threads();
	}
	printf("Tinham %d threads na região paralela.\n", nthreads);

	sum = 0;
	#pragma omp parallel for reduction (+:sum)
	for(i=0; i<5; i++)
		sum+=a[i] + b[i];
	printf("Soma omp: %d\n", sum);

	sum = 0;
	#pragma simd reduction (+:sum)
	for(i=0; i<5; i++)
		sum+=a[i] + b[i];
	printf("Soma simd: %d\n", sum);

	sum = 0;
	#pragma vector aligned
	#pragma simd reduction (+:sum)
	for(i=0; i<5; i++)
		sum+=a[i] * b[i];
	printf("Multiplicacao alinhada: %d\n", sum);


	// ---- multiplicação de matrizes com paralelismo a nível de thread, vetorização e alinhamento de memória

	int mat1[4] = {2,2,2,2};
	int mat2[4] = {2,2,2,2};
	int resultado[4];

	for(i=0; i<2;i++){
		for(j=0; j<2; j++){
			resultado[i*2+j] = 0;
		}
	}

	#pragma omp parallel for private(i, j, k)
	for(i=0; i<2;i++){
		for(k=0;k<2;k++){
			#pragma vector aligned
			#pragma simd
			for(j=0; j<2; j++){
				resultado[i*2+j] += mat1[i*2+k] * mat2[k*2+j];
			}	
		}
	}

	for(i=0; i<2;i++){
		for(j=0; j<2; j++){
			printf("%d ", resultado[i*2+j]);
		}
	}
	
	return 0;
}
