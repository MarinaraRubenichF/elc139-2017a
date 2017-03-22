#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>

long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

float matrix_chain_order_dyn(double *p, int n)
{
	int i, j, l, q, k;
	double **m = (double**) malloc(sizeof(double*) * n);

	if (m == NULL) {
      		printf("Error.\n");
      		exit(EXIT_FAILURE);      
   	}

	for(i=0;i<n;i++){
		
		m[i] = (double*) malloc(sizeof(double) * n);
		
		if (m[i] == NULL) {
      			printf("Error.\n");
      			exit(EXIT_FAILURE);      
   		}
	}

	for (i=1; i<n; i++)
	        m[i][i] = 0;
			
	for(l=2; l<n; l++){
		for(i=1; i<(n-l+1); i++){
			j = (i+l-1);
			m[i][j] = INT_MAX;
			for(k=i; k<=j-1; k++){
				q = m[i][k]+m[k+1][j]+p[i-1]*p[k]*p[j];
				if(q<m[i][j])
					m[i][j] = q;				
			}		
		}	
	}

	return m[1][n-1];
}

float matrix_chain_order_normal(double *p, int i, int j)
{
    if(i == j)
        return 0;
    int k;
    float min = INT_MAX;
    int count;
 
    for (k = i; k <j; k++)
    {
        count = matrix_chain_order_normal(p, i, k) +
                matrix_chain_order_normal(p, k+1, j) +
                p[i-1]*p[k]*p[j];
 
        if (count < min)
            min = count;
    }

    return min;
}

void vector_init(double* m, int size)
{	
	int i;

	srand((unsigned)time(NULL));

	for(i=0; i<size; i++)
		m[i] = (rand() % 50) + 1;

}


int main(int argc, char *argv[])
{
	
	int size;
	long start, finish;
	float dyn, normal;

	if ((argc != 2)){
		printf("Entry invalid. Try again.\n");
		return (-1);
	}
	
	size = atoi(argv[1]);

	double *vector = (double *) malloc(sizeof(double) * size);

   	if (vector == NULL) {
      		printf("Error.\n");
      		exit(EXIT_FAILURE);      
   	}
	
	vector_init(vector, size);
	
	//matrix chain order dynamic programming
	start = wtime();
	dyn = matrix_chain_order_dyn(vector, size);
	finish = wtime();

	printf("Menor numero de multiplicacoes possiveis (dinâmico): %f\n", dyn);
	printf("Tempo de Calculo: %ld usec\n", (long) (finish-start));


	//matrix chain order normal	
	start = wtime();
	normal = matrix_chain_order_normal(vector, 1, size-1);
	finish = wtime();

	printf("Menor numero de multiplicacoes possiveis (normal): %f\n", normal);
	printf("Tempo de Calculo: %ld usec\n", (long) (finish-start));
	
	return 0;
}
