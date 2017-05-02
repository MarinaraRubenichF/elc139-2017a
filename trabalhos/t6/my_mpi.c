/* Implemente um programa MPI que transporte uma mensagem em um pipeline formado por processos de 0 a NP-1 (processo 0 envia para 1, processo 1 envia para 2, ..., processo NP-1 mostra o resultado). A mensagem é um número inteiro que é incrementado antes de ser passado adiante.*/
// mpirun -np <num processos> <executável>

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	int msg = 1; //mensagem inicial definida
	int msgrec;
	int i;
	int ntasks, rank;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank == 0){
		msg++;
		MPI_Send(&msg, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
		//printf("Enviei a mensagem %d\n", msg);
	}else if (rank == ntasks-1){
		MPI_Recv(&msgrec, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("Mensagem final: %d\n", msgrec);
	}else{
		MPI_Recv(&msgrec, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);		
		msg=msgrec+1;
		//printf("Recebi a mensagem %d\n", msgrec);
		MPI_Send(&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
		//printf("Enviei a mensagem %d\n", msg);
	}

	MPI_Finalize();
	return 0;
}
