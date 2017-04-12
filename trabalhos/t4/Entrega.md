# T4: Bug em OpenMP
Programação Paralela - ELC139-2017a\
Professora: Andrea Schwertner Charão\
Nome: Ana Luisa V. Solórzano

## Questões
1. No tutorial sobre OpenMP do LLNL, há uma seção de exercícios que mostra códigos com bugs: https://computing.llnl.gov/tutorials/openMP/exercise.html. Qual o bug do programa omp_bug2.c?

  O bug é não ter colocado as variáveis *tid* e *total* como private, pois várias threads iriam acessar elas paralelamente, o que pode resultar em inconsistência nos resultados se elas não forem protegidas.

2. Qual seria uma solução para o bug? Explique e forneça o código corrigido.

  A solução seria incluir no primeiro pragma omp: *private (tid, total)*, fazendo com que cada thread tenha sua própria instância de cada variável. 
  
  [Código corrigido](omp_bug2.c)
  
  
## Referências

Intel. omp barrier. https://software.intel.com/en-us/node/524510

Intel. OpenMP\* Loop Scheduling. https://software.intel.com/en-us/articles/openmp-loop-scheduling

Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre OpenMP. OpenMP Exercise. https://computing.llnl.gov/tutorials/openMP/exercise.html


  
