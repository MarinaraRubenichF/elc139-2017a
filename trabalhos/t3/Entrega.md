# T3: Programação Paralela Multithread
Programação Paralela - ELC139-2017a\
Professora: Andrea Schwertner Charão\
Nome: Ana Luisa V. Solórzano

## Questões Pthreads:

1. Explique como se encontram implementadas as 4 etapas de projeto: 
  
  **Particionamento:** Este programa realiza um paralelismo de dados, no qual os dados são particionados e cada um dos subconjuntos
  de dados é associado a uma das threads, que executa os mesmos cálculos sobre ele. O particionamento é fundamentado na criação dos
  vetores, pois cada vetor tem tamanho total relativo ao nº de threads de entrada:  
   ``` C
   dotdata.a = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.a, wsize*nthreads, 0.01);
   dotdata.b = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.b, wsize*nthreads, 1.0);
   ```
  a partir disso, o particionamento é determinado na função *'dotprod_worker'*, onde é declarado a quantidade de trabalho
  de cada thread, garantindo que cada processo sempre executará cálculos sobre a mesma quantidade de dados: 
  ``` C
   int wsize = dotdata.wsize;
   int start = offset*wsize;
   int end = start + wsize;
  ``` 
  por fim esse particionamento é executado no laço:
  ``` C
  for (i = start; i < end ; i++)  {
         mysum += (a[i] * b[i]);
  }
  ```
  
  **Comunicação:** A comunicação é feita na etapa onde ocorre dependência entre os cálculos do produto escalar, ou seja,
  durante as somas dos produtos parciais calculadas em cada thread, no trecho de código:
 
 ``` C
  pthread_mutex_lock (&mutexsum);
  dotdata.c += mysum;
  pthread_mutex_unlock (&mutexsum);
 ```   
 
  **Aglomeração:** É dada no agrupamento das várias multiplicações e somas dos dados realizados por cada processo, gerando
  somas parciais para obtenção do resultado final. Ele ocorre na função *'dotprod_worker'*:
  
  ``` C
  for (k = 0; k < dotdata.repeat; k++) {
      mysum = 0.0;
      for (i = start; i < end ; i++)  {
         mysum += (a[i] * b[i]);
      }
   }
  ```  
  
  **Mapeamento:** É realizada uma distribuição estática de tarefas aos processadores, cada um calcula o mesmo número de
  operações sobre a mesma quantidade de dados.
  
  ``` C
  for (i = 0; i < nthreads; i++) {
      pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
   }
   ```
   
2. Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?

  A aceleração foi de aproximadamente 1.82 (foram feitas 10 execuções de cada caso e pegos os menores tempos).

3. A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.

Tabelas com os tempos de execução (usec) para cada caso testado (10 rodadas cada):

Tamanho de vetor: 1000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | 3479549 | 1904348 | 1601889 |
2000  | 6961125 | 3816475 | 3228619 |
3000  | 10427300 | 6172786 | 5419455 |

Tamanho de vetor: 10000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | 34765807 | 19159307 | 16037858 |
2000  | 76330325 | 39538070 | 35712179 |
3000  | 104316050 | 60251731 | 53370544 |

Tamanho de vetor: 20000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | 69502162 | 38045032 | 32393138 |
2000  | 139019381 | 77096666 | 71441022 |
3000  | 208542112 | 115446728 | 107186503 |

4. Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.

Speedups para vetores de tamanho 1000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | ---- | ---- |----- |
2  | 1.827 | 1.823 | 1.689 |
4  | 2.172 | 2.156 | 1.924 |

Speedups para vetores de tamanho 10000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | --- | ---- |----- |
2  | 1.814 | 1.930 | 1.731 |
4  | 2.167 | 2.137 | 1.954 |

Speedups para vetores de tamanho 2000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | --- | ---- |----- |
2  | 1.826 | 1.803 | 1.806 |
4  | 2.145 | 1.945 | 1.945 |



5. Explique as diferenças entre pthreads_dotprod.c e pthreads_dotprod2.c. Com as linhas removidas, o programa está correto?
  O pthreads_dotprod2.c não realiza exclusão mútua na função *'dotprod_worker'* durante o acesso a variável que receberá a soma total (l. 46). Ele não está correto, pois esse fato pode acarretar em inconsistencia no resultado, pois pode ocorrer de duas threads quererem incrementar a mesma variável ao mesmo tempo, mesmo assim algumas vezes o programa pode gerar saídas consistentes.
  
## Implementação OpenMP 

1. Implemente um programa equivalente a pthreads_dotprod.c usando OpenMP.

2. Avalie o desempenho do programa em OpenMP, usando os mesmos dados/argumentos do programa com threads POSIX.

Tabelas com os tempos de execução (usec) para cada caso testado (10 rodadas cada):

Tamanho de vetor: 1000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | 4186366 | 4841716 | 2758551 |
2000  | 8357674 | 9682921 | 5584204 |
3000  | 12578018 | 14560894 | 9266145 |

Tamanho de vetor: 10000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | | | |
2000  | | | |
3000  | | | |

Tamanho de vetor: 20000000

Repetições | 1 thread | 2threads | 4threads |
---------- | ------- | -------- | -------- |
1000  | | | |
2000  | | | |
3000  | | | |

Speedups para vetores de tamanho 1000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | ---- | ---- |----- |
2  |   |   |   |
4  |  |   |   |

Speedups para vetores de tamanho 10000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | --- | ---- |----- |
2  |   |   |   |
4  |  |   |   |

Speedups para vetores de tamanho 2000000 em cada caso (nº de threads pelo nº repetições):

Threads | 1000 | 2000 | 3000 |
------- | --- | ---- |----- |
2  |  |   |   |
4  |  |  |   |
  
## Referências: 
- Ricardo Rocha DCC-FCUP. Programação Paralela e Distribuída. https://www.dcc.fc.up.pt/~ricroc/aulas/0708/ppd/apontamentos/fundamentos.pdf.
- Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre OpenMP. https://computing.llnl.gov/tutorials/openMP/.
- Microsoft. Parallel Programming - OpenMP - Directives. https://msdn.microsoft.com/en-us/library/68ah4xc7.aspx.
