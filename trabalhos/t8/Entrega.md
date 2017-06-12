# T8: Quebra de senhas com OpenMPI
Programação Paralela - ELC139-2017a\
Professora: Andrea Schwertner Charão\
Nome: Ana Luisa V. Solórzano


## Sumário

  * [Códigos](#codigos)
    * [MDCrack sequencial](mdcrack_sequencial.c)
    * [Troca de Mensagens](msg_mpi.c)
    * MDCrack com MPI: [1](mdcrack_mpi.c) e [2](mdcrack_mpi2.c)
  * [Estratégias utilizadas](#estratégias-utilizadas)
  * [Análises de desempenho](#análises-de-desempenho)   
  * [Referências](#referências)
    
----

### Estratégias utilizadas:


### Análises de desempenho: 
 **Arquivos de entrada:** 
   * [crackme.txt](crackme.txt)
   * [exemplos.txt](exemplos.txt)

**Ambiente de execução:**
 * **Hardware:** Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz, 2 cores físicos e 4 virtuais, cache L1 32K, L2 256K e L3 3MB e 4GB RAM
 * **Software:** Ubuntu e versão do Linux 4.4.0-78-generic
 * **Compilador:** gcc version 5.4.0

**Execuções:**
Foram feitas 3 execuções para a versão sequencial em cada caso e pega a média de tempo. E feitas uma execução para cada programa com MPI para 2, 3, 4, 5 e 10 processos. Todos para cada um dos 2 arquivos de entrada.
   
**Resultados:**\
(Execuções testadas sem os prints sobre as trocas de mensagens.)

   * [MDCrack sequencial:](mdcrack_sequencial.c)
   
   Tempo de Execução - crackme.txt: 1474,869sec\
   Tempo de Execução - exemplos.txt: 40,350sec
   
   * [MDCrack com MPI versão 1:](mdcrack_mpi.c)
   
   | np | Tempo de Execução(sec) - crackme.txt| Speedup | Tempo de Execução(sec) - exemplos.txt| Speedup |
   | -- | ----------------------------------- | ------- | ------------------------------------ | ------- |
   | 2  | 1589.067 | 0.928 | 43.476 | 0.928 |
   | 3  | 986.366 | 1.495 | 43.465 | 0.928 |
   | 4  | 887.196 | 1.662 | 45.567 | 0.885 |
   | 5  | 964.384 | 1.529 | 47.191 | 0.855 |
   | 10 | 1451.442 | 1.016 | 92.859 | 0.434 |
  
   * [MDCrack com MPI versão 2:](mdcrack_mpi2.c)
   
   | np | Tempo de Execução(sec) - crackme.txt| Speedup | Tempo de Execução(sec) - exemplos.txt| Speedup |
   | -- | ----------------------------------- | ------- | ------------------------------------ | ------- |
   | 2  | 1589.444 | 0.928 | 43.487 | 0.927 |
   | 3  | 986.070 | 1.496 | 43.462 | 0.928 |
   | 4  | 886.849 | 1.663 | 45.120 | 0.894 |
   | 5  | 962.299 | 1.533 | 47.472 | 0.850 |
   | 10 | 1432.750 | 1.029 | 92.650 | 0.435 | 

Foram usadas duas versões do programa com MPI esperando um tempo de execução mais variável, pois eles tem implementações um pouco diferentes: um usa mais alocação dinâmica de vetor e outro estática e um lança as execuções linha a linha durante a leitura do arquivo, deixando o arquivo aberto. Provavelmente por ser um programa CPU-bound e não exigir muito de memória não tiveram tempos muito distintos de execução.

Os speedups dos programas paralelos foram calculados utilizando como base a implementação sequencial. Observou-se que a troca de mensagens trouxe um pequeno overhead usando *-np 2* em comparação com a versão sequencial.

Para uma execução rápida (usando *exemplos.txt*) o uso de MPI não trouxe desempenho, o gasto com a troca de mensagens não compensou a paralelização, sendo que o uso de mais processos do que mensagens (10 processos para 7 hashes no arquivo) demonstrou ter o pior desempenho pois teve o gasto a mais com a criação de processos que nem vão receber mensagem para executar algo.

Já para uma execução mais lenta (usando *crackme.txt*) com mais hashes mais difíceis de crackear o uso de MPI trouxe um bom ganho de desempenho com speedup 1.49 com 3 processos e 1.66 com 4 processos, este sendo o speedup máximo atingido pois ele faz total uso dos cores, 1 core fica dedicado ao mestre principalmente para a recepção de mensagens e os outros são para os trabalhadores executarem o seu trabalho. Com mais processos do que o número de cores disponíveis o desempenho caiu neste caso pois teve o gasto com a criação e sincronização deles, não compensados pelos tempos de execução para crackear as senhas. 

Com isso, para um conjunto de hashes "fáceis" de crackear vale a pena usar a versão sequencial, senão para hashes mais complicadas a versão paralela com 4 processos vale mais a pena nesse ambiente. (Mas não é muito intuitivo saber quais arquivos vão ser facilmente crackeados ou não...)

### Referências: 
 * http://mpitutorial.com/tutorials/dynamic-receiving-with-mpi-probe-and-mpi-status/
 * http://www.mcs.anl.gov/~itf/dbpp/text/node19.html#figLB2 
