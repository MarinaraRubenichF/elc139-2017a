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
   
**Resultados:**\
Execuções testadas sem os printf.

   * [MDCrack sequencial:](mdcrack_sequencial.c)
   
   Tempo de Execução - crackme.txt: 1475,575sec \
   Tempo de Execução - exemplos.txt:
   
   * [MDCrack com MPI versão 1:](mdcrack_mpi.c)
   
   | np | Tempo de Execução(sec) - crackme.txt| Speedup | Tempo de Execução(sec) - exemplos.txt| Speedup |
   | -- | ----------------------------------- | ------- | ------------------------------------ | ------- |
   | 2    | 1589,067 | | 43,476 | - |
   | 3    | 986,366 | | 43,465 |   |
   | 4    | 887,196 | | 45,567 |   |
   | 5    | 964,384 | | 47,191 |   |
   | 10   | 1451,442129 | | 92,859 |   |
  
   * [MDCrack com MPI versão 2:](mdcrack_mpi2.c)
   
   | np | Tempo de Execução(sec) - crackme.txt| Speedup | Tempo de Execução(sec) - exemplos.txt| Speedup |
   | -- | ----------------------------------- | ------- | ------------------------------------ | ------- |
   | 2    | 1589,444 | | 43,487 | - |
   | 3    | 986,070 | | 43,462 |   |
   | 4    | 886,849 | | 45,120 |   |
   | 5    | 962,299 | | 47,472 |   |
   | 10   | 1432,750 | | 92,650 |   |

### Referências: 
 * http://mpitutorial.com/tutorials/dynamic-receiving-with-mpi-probe-and-mpi-status/
 * http://www.mcs.anl.gov/~itf/dbpp/text/node19.html#figLB2 
