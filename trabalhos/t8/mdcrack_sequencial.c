#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#define INPUT "./mdcrack-1.2/bin/mdcrack -s abcdefghijklmnopqrstuvwxyz -M MD5 "

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
 * Execução do programa mdcrack para cada hash do arquivo de entrada
 */
void executa_mdcrack(char *hash)
{
	char path[1035];
	char programa[180] = INPUT;
	strcat(programa, hash);

	FILE *f = popen(programa, "r");
	if(f==NULL){
		perror("Error: ");
		exit(-1);
	}	
	
	while(fgets(path, sizeof(path)-1, f) != NULL){
		if(strstr(path, "Collision found") != NULL)
			printf("%s", path);
	}

	pclose(f);
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("O programa deve receber o nome do arquivo de hashes.\n");
		return(-1);
	}

	FILE *f;
	char *arquivo;
	char *linha = NULL;  
	size_t tam = 0;
	size_t str;
	long t_ini, t_fim;

	arquivo = argv[1];
	f = fopen(arquivo, "r");
	if(f == NULL){
		perror("Error: ");
		return(-1);
	}	

	t_ini = wtime();
	while((str = getline(&linha, &tam, f)) != -1){
		memmove(linha, linha, str-1);
		linha[str-1]=0;
		executa_mdcrack(linha);
	}
	t_fim = wtime();	

	free(linha);
	fclose(f);

	printf("Tempo de execução: %ld usec\n",(long)(t_fim - t_ini));

	return(0);

}
