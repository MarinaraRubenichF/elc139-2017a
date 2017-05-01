# T6: Experiências com MPI
Programação Paralela - ELC139-2017a\
Professora: Andrea Schwertner Charão\
Nome: Ana Luisa V. Solórzano

## Sumário

  * [Parte 1](#parte-1)
    * Análise do programa
    * Análises de desempenho 
    * Screenshots
    * Observações
  * [Parte 2](#parte-2)
    * Anotações
    * Respostas
    * Screenshots
    * Observações
  * [Referências](#referências)
  
  ----
  ## Parte 1 
   ### Análise do programa:
  **Particionamento:** É definido na função principal  utilizando o valor de *ntasks*, que é o número de processos para realizar a operação total. Essa variável é utilizada para obter o tamanho do particionamento dos vetores (*part_size*), posteriormente utilizado na função para o cálculo do produto escalar como limitador do laço interno.
  
   ``` C
   part_size = n / ntasks;
   ...
   
   //Calcula produto escalar
   start_time = wtime();
   mydot = dot_product(a, b, part_size, repeat);
   end_time = wtime();
   ...
   
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
  ```
   
   **Comunicação:** o cálculo das somas parciais de produtos escalares ocorre na função principal quando o processo de id = 0 fica esperando os resultados parciais de cada um dos outros processos, independendo de uma ordem de chegada, para somar depois com *mydot*:
   ``` C
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
   ```
   **Aglomeração:** a geração das somas parciais calculadas por cada processo ocorre na função de cálculo do produto escalar dentro do laço mais interno.
   
   **Mapeamento:** o mapeamento entre processos é realizado na função principal quando cada tarefa começa a executar a partir do MPI_Init
   ``` C
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
   MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
   ```
   
   ### Análises de desempenho:
   Especificações da máquina utilizada **lsc4** :\
   Compilador gcc 4.9.2 e Sistema Operacional Debian versão do Linux 4.9.2-10)\
   Processador Intel(R) Xeon(R) E5620, com 4 cores físicos e 8 virtuais, memória cache L1 de 32KB, L2 de 256KB e L3 de 12MB e 12GB de RAM.
   
   | Caso | Tamanho do vetor | Repetições | Num. de Processos | Host | Tempo de Execução (sec) | Speedup |
   | ---- | ---------------- | ---------- | ----------------- | ---- | ----------------------- | ------- |
   | A    | 30000000 | 100 | 1 | lsc4| 15.504 | - |
   | B    | 30000000 | 100 | 2 | lsc4| 7.930 | 1.955 |
   | D    | 30000000 | 100 | 4 | lsc4| 5.850 | 2.650 |
   
   ## Parte 2
   
   + Implemente um programa MPI que transporte uma mensagem em um pipeline formado por processos de 0 a NP-1 (processo 0 envia para 1, processo 1 envia para 2, ..., processo NP-1 mostra o resultado). A mensagem é um número inteiro que é incrementado antes de ser passado adiante.

+ O programa [mpi_errado1.c](mpi_errado1.c) deveria realizar a troca de mensagens entre 2 processos, mas ele não funciona como esperado. Identifique o erro e corrija-o.

+ O programa [mpi_errado2.c](mpi_errado2.c) deveria realizar a troca de mensagens entre 2 processos, mas também não funciona como esperado. Identifique o erro e corrija-o.

 ## Referências
 * Intel. Intel® Xeon® Processor E5620. http://ark.intel.com/PT-BR/products/47925/Intel-Xeon-Processor-E5620-12M-Cache-2_40-GHz-5_86-GTs-Intel-QPI.
 * Open-mpi. Running MPI jobs. https://www.open-mpi.org/faq/?category=running.

