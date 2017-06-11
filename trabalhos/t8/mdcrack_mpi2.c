#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#define TRUE 1
#define INPUT "./mdcrack-1.2/bin/mdcrack -s abcdefghijklmnopqrstuvwxyz -M MD5 "
#define MAX_PASS_SIZE 20

/* Facilitador de leitura dos outputs*/
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

/* Tempo (wallclock) em microssegundos */ 
long wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec*1000000 + t.tv_usec;
}

/* Envio de mensagens pelo mestre */
void send_msg(int dest, int tag, char* cripto)
{	
	printf(RED "(Mestre) - Enviando para %d a mensagem: %s" RESET, dest, cripto);
	int size = strlen(cripto) + 1;
	MPI_Send(&size, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	MPI_Send((char*)cripto, size, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
}

void master(char* file)
{
	int num_proc, num_msg, count;
	char msgrec[MAX_PASS_SIZE];
	size_t tam = 0;
	size_t str;
	char *line = NULL;
	char **cracked = malloc(100*sizeof(char *));
	
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

	FILE* f = fopen(file, "r");
	if(f == NULL){
		perror("Error: ");
		exit(0);
	}

	num_msg = 0;
	count = 1;

	/* Primeira leva de mensagens */
	while((count < num_proc) && ((str = getline(&line, &tam, f)) != -1)){
		send_msg(count, num_msg, line);
		num_msg++;
		count++;
	}
	
	/* Recebe as mensagens e se ainda tiverem mensagens distribui mais trabalho */
	while((str = getline(&line, &tam, f)) != -1 || (--count > 0) ){

		MPI_Recv(msgrec, MAX_PASS_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf(GREEN "(Mestre) - Recebi de %d a mensagem (%d): %s" RESET, status.MPI_SOURCE,  status.MPI_TAG, msgrec);
		cracked[status.MPI_TAG] =  malloc(strlen(msgrec)*sizeof(char));
		strcpy(cracked[status.MPI_TAG], msgrec);

		if(str != -1){
			send_msg(status.MPI_SOURCE, num_msg, line);
			num_msg++;
		}

	}

	fclose(f);

	printf("Senhas quebradas:\n");
	for(count=0; count<num_msg; count++){
		printf("%s", cracked[count]);
		free(cracked[count]);
	}
	free(cracked);

	/* Termina a execução dos processos */
	int end = 0;
	for(count=1; count<num_proc; count++)
		MPI_Send(&end, 1, MPI_INT, count, count, MPI_COMM_WORLD);
}

void worker(int rank)
{
	MPI_Status status;

	int size;
	char *msgrec = NULL;
	char *msgsend = NULL;
	char path[1035];

	while(TRUE){
		
		MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(size == 0)
			break;
		
		msgrec = malloc(size);
		MPI_Recv(msgrec, size, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		memmove(msgrec, msgrec, size-2);
		msgrec[size-2] = 0;
		printf(CYAN "(%d) - Recebi a mensagem: %s de número: %d\n" RESET, rank, msgrec, status.MPI_TAG);
		
		char programa[180] = INPUT;
		strcat(programa, msgrec);
		
		/* Executa o programa mdcrack */
		FILE *f = popen(programa, "r");
		if(f == NULL){
			perror("Error: ");
			exit(-1);
		}	
	
		while(fgets(path, sizeof(path)-1, f) != NULL){
			if(strstr(path, "Collision found") != NULL){
				msgsend = strchr(path, '>');
				msgsend+=2;
				printf(BLUE "(%d) - Enviando a mensagem: %s" RESET, rank, msgsend);
				MPI_Send((char*)msgsend, strlen(msgsend) + 1, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);
				break;	
			} 
		}

		pclose(f);
		free(msgrec);
	}
}

int main(int argc, char **argv)
{
	int rank, num_proc;
	long t_init, t_fim;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

	if(argc != 2){
		if(rank == 0)	
			printf("Entrada: mpiexec -np <ntasks> %s <nome arquivo>\n", argv[0]);
		MPI_Finalize();
		return(-1);
	}

	if(num_proc < 2){
		if(rank == 0)	
			printf("Modelo mestre-trabalhador, -np pelo menos = 2.\n");
		MPI_Finalize();
		return(-1);
	}

	if(rank == 0){
		t_init = wtime();
		master(argv[1]);
		t_fim = wtime();
		printf("Tempo de execução: %ld usec\n",(long)(t_fim - t_init));
	}
	else
		worker(rank);

	MPI_Finalize();
   	return(0);
}
