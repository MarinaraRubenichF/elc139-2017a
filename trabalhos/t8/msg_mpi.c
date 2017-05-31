/*
Programa em MPI que testa a recepção de mensagens de quaisquer processos e descobre a origem do processo.
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

static char *msg_master[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
int num_msg = 10;

int main(int argc, char **argv)
{
	int numproc, rank;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char *msg_worker = "ok";
	char *msg_rec;
	int i, count_master;

	if(rank != 0){
		MPI_Recv(&msg_rec, sizeof(msg_rec), MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		printf("%d - Recebi a mensagem \"%s\" vinda do rank: %d com a tag: %d\n", rank, msg_rec, status.MPI_SOURCE, status.MPI_TAG);				
		MPI_Send(&msg_worker, sizeof(msg_worker), MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);
		printf("%d - Usei a mensagem \"%s\" vou enviar pro mestre...\n", rank, msg_rec);        	
	}
	else{
		for (i=1; i<numproc; i++){
			MPI_Send(&msg_master[count_master], sizeof(msg_master[count_master]), MPI_CHAR, i, i, MPI_COMM_WORLD);
			count_master++;
			num_msg--;
		}

		for(i=1; i<numproc; i++){	
			MPI_Recv(&msg_rec, sizeof(msg_rec), MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
			printf("%d - Recebi a mensagem \"%s\" vinda do rank: %d com a tag: %d\n", rank, msg_rec, status.MPI_SOURCE, status.MPI_TAG);
		}
	}

	MPI_Finalize();
   	return(0);
}
