#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#define TRUE 1
#define INPUT "./mdcrack-1.2/bin/mdcrack -s abcdefghijklmnopqrstuvwxyz -M MD5 "
#define SIZE 10
#define MAX_PASS_SIZE 20

/* Facilitador de leitura dos outputs */
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
	int num_proc, num_line, num_msg, i, end;
	int vsize = SIZE;
	char msgrec[MAX_PASS_SIZE];

	size_t tam = 0;
	size_t str;
	char *line = NULL;
	char **cripto = malloc(vsize * sizeof(char*));

	/* Lê o arquivo com os hashes e os salva em um vetor */
	FILE* f = fopen(file, "r");
	if(f == NULL){
		perror("Error: ");
		exit(0);
	}

	num_line = 0;
	while((str = getline(&line, &tam, f)) != -1){

		cripto[num_line] = malloc(str*sizeof(char));
		strcpy(cripto[num_line], line);
		num_line++;

		if(num_line == vsize){
			vsize += SIZE;
			cripto = realloc(cripto, (vsize)*sizeof(char*));
		}
	}
	cripto[num_line] = '\0';
	fclose(f);

	char **cracked = malloc(num_line*sizeof(char *));

	/* Início da troca de mensagens */
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	
	/* Envia uma mensagem para cada processo */
	num_msg = 0;
	for(i=1; (i<num_proc && i<=num_line) ; i++){
		send_msg(i, num_msg, cripto[num_msg]);
		num_msg++;	
	}
	
	/* Recepção das mensagens dos trabalhadores e, se tiver, envio de mais mensagens */
	while((num_msg < num_line) || (--i > 0)){

		MPI_Recv(msgrec, MAX_PASS_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf(GREEN "(Mestre) - Recebi de %d a mensagem (%d): %s" RESET, status.MPI_SOURCE,  status.MPI_TAG, msgrec);
		
		cracked[status.MPI_TAG] =  malloc(strlen(msgrec)*sizeof(char));
		strcpy(cracked[status.MPI_TAG], msgrec);

		/* Se ainda tem mensagens para enviar */
		if(num_msg < num_line){
			send_msg(status.MPI_SOURCE, num_msg, cripto[num_msg]);
			num_msg++;
		}
	}

	/* Termina a execução dos processos */
	end = 0;
	for(i=1; i<num_proc; i++)
		MPI_Send(&end, 1, MPI_INT, i, i, MPI_COMM_WORLD);

	/* Libera os vetores e imprime as senhas quebradas */
	for(i=0; i<num_line; i++){
		printf("%s", cracked[i]);
		free(cripto[i]);
		free(cracked[i]);
	}

	free(cripto);
	free(cracked);
}

void worker(int rank)
{
	MPI_Status status;
	int size;
	char *msgrec = NULL;
	char* aux;
	char msgsend[MAX_PASS_SIZE];
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
				aux = strchr(path, '>');
				aux+=2;
				strcpy(msgsend, aux);
				printf(BLUE "(%d) - Enviando a mensagem: %s" RESET, rank, msgsend);
				MPI_Send(msgsend, strlen(msgsend) + 1, MPI_CHAR, 0, status.MPI_TAG, MPI_COMM_WORLD);
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
	char *file;
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
			printf("Modelo mestre-trabalhador, o programa precisa de pelo menos 2 processos em execução.\n");
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
