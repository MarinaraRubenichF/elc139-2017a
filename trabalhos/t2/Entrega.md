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
      Sim. Quanto maior o tamanho do vetor maior será o tempo gasto na função de inicialização dos vetores, e quanto mais repetições de execução do produto escalar maior o tempo gasto na função de cálculo. Assim, nos vetores de tamanho grande (30 e 60 milhões) ao executar 1 repetição a maioria do tempo foi utilizado nas suas inicializações, sendo que a proporção entre o tempo gasto na chamada de 'init_vectors' e o tempo gasto na chamada de 'dot_product' gira em torno de 1,5 a 2.\
   (b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?
   ### Screenshots:
      ![swap](dotprod_seq/screenshots/swap.png?raw=true "Execução com vetor de tamanho 300M fazendo swap.")
      
      ![30M](dotprod_seq/screenshots/30M.png?raw=true "Execução do programa com vetor de tamanho 30M, o qual faz bom uso da CPU sem precisar de swap.")
      
      ![30Mgprof](dotprod_seq/screenshots/30M1exec.png?raw=true "Execução com vetor de tamanho 30M e 1 repetição.")
   ### Observações:

----

## Parte 2
   ### Anotações:
   ### Respostas:
   ### Screenshots:
   ### Observações:

----

## Referências
- The Geek Stuff. GPROF Tutorial – How to use Linux GNU GCC Profiling Tool. http://www.thegeekstuff.com/2012/08/gprof-tutorial/.
