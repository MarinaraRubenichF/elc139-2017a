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
   
2. Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?\\

3. A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? 
Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema 
(tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). 
Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. 
Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, 
tempos muito pequenos, etc.

4. Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.

5 .Explique as diferenças entre pthreads_dotprod.c e pthreads_dotprod2.c. Com as linhas removidas, o programa está correto?

## Referências: 
