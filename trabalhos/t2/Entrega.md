# T2: Experiências com profilers
Programação Paralela - ELC139-2017a\
Professora: Andrea Schwertner Charão\
Nome: Ana Luisa V. Solórzano

## Sumário

  * [Parte 1](#parte-1)
    * Anotações
    * Respostas
    * Screenshots
    * Observações
  * [Parte 2](#parte-2)
    * Anotações
    * Respostas
    * Screenshots
    * Observações
  * [Referências](#referências)

## Parte 1
   ### Anotações:
   Observou-se que com vetores de tamanho 300.000.000 toda a memória RAM é utilizada, assim se passa a fazer uso da memória swap, que deixa a execução mais lenta. Uma configuração boa encontrada para a análise foi quanto a variação da execução entre vetores de tamanhos {3000, 30000, 300000, 3000000, 30000000, 60000000} com repetições de {1, 10, 100, 1000} cada. 
   ### Respostas:
   (a) O perfil é afetado pelas opções de configuração?\
      .\
   (b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
   ### Screenshots:
      Execução com vetor de 300M fazendo swap.\
      Execução do programa com vetor de tamanho 30M, o qual faz bom uso da CPU sem precisar de swap.
   ### Observações:

----

## Parte 2
   ### Anotações:
   ### Respostas:
   ### Screenshots:
   ### Observações:

----

## Referências
