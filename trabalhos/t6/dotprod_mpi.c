/*
 *  Calcula produto escalar (dot product) de 2 vetores.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

#define MASTER 0

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Inicializa vetores
 */ 
void init_vectors(double* a, double* b, int n)
{
   int i;
   for (i = 0; i < n; i++)
   {
      a[i] = 0.5;
      b[i] = 1.0; 
   }
}

/*
 * Calcula o produto escalar (varias vezes)
 */ 
double dot_product(double* a, double* b, int n, int repeat)
{
   double dot;
   int i, k;
   for (k = 0; k < repeat; k++) {
      dot = 0.0;
      for (i = 0; i < n; i++) {
         dot += a[i] * b[i];
      }
   }
   return dot;
} 

int main(int argc, char **argv)
{
   int i;
   int taskid, ntasks;
   int part_size;
   int msg_tag = 0;
   MPI_Status status;
   int n;
   int repeat;
   long start_time, end_time;
   double mydot = 0.0;
   double dot;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
   MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

   if ((argc == 3)) {
      n = atoi(argv[1]);       // tamanho dos vetores
      repeat = atoi(argv[2]);  // numero de repeticoes (para variar carga)
   } else {
      if (taskid == MASTER)
         printf("Uso: mpiexec -np <ntasks> %s <tamanho dos vetores> <repeticoes>\n", argv[0]);
      MPI_Finalize();
      return EXIT_FAILURE;
   }
   if ((n % ntasks) != 0) {
      if (taskid == MASTER)
         printf("Tamanho dos vetores deve ser multiplo do numero de processos\n");
      MPI_Finalize();
      return EXIT_FAILURE;
   }

   part_size = n / ntasks;
   printf("Proc %d, n %d, part_size %d\n", taskid, n, part_size);

   // Aloca memoria para os vetores
   double *a = (double *) malloc(sizeof(double) * part_size);
   double *b = (double *) malloc(sizeof(double) * part_size);
   
   if (a == NULL || b == NULL) {
      printf("Erro de alocacao de memoria\n");
      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      exit(EXIT_FAILURE);      
   }
   
   // Inicializa vetores
   init_vectors(a, b, part_size);

   // Calcula produto escalar
   start_time = wtime();
   mydot = dot_product(a, b, part_size, repeat);
   end_time = wtime();

   if (taskid == MASTER)
   {
      for (i = 0; i < ntasks-1; i++)
      {
         MPI_Recv(&dot, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         printf("Valor parcial recebido do processo %d = %f\n", status.MPI_SOURCE, dot);
         mydot += dot;
      }
      printf("Produto escalar = %f\n", mydot);
      printf("Tempo de calculo = %ld usec\n", (long) (end_time - start_time));      
   }
   else
   {
      MPI_Send(&mydot, 1, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD);
   }

   free((void *) a);
   free((void *) b);

   MPI_Finalize();
   return EXIT_SUCCESS;
}
