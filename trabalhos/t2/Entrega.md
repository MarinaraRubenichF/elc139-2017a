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

----

## Parte 1 
   ### Anotações:
   O programa foi analisado quanto a variação da execução entre vetores de tamanhos {3000, 30000, 300000, 3000000, 30000000, 60000000} com repetições de {1, 10, 100, 1000} cada. 
   ### Respostas:
   (a) O perfil é afetado pelas opções de configuração?\
      Sim. Na análise do 'Flat profile', quanto maior o tamanho do vetor maior foi o tempo gasto na função de inicialização dos vetores, e quanto mais repetições de execução do produto escalar, maior foi o tempo gasto na função de cálculo. Na análise do 'Call graph' também notou-se mudanças nas informações, onde para vetores de tamanho pequeno os tempos gastos nas funções de inicialização e de cálculo foram tão baixos que não foram captados, enquanto vetores de tamanho grande tiveram detalhes sobre a árvore de chamada, mostrando o tempo gasto em cada função a partir da função chamadora.\
   (b) Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?\
   Sim. A função 'dot_product', por ser a que toma a maior parte do tempo de processamento na maioria dos casos. Por ser um programa CPU bound, se essa função fosse paralelizada poderia fazer melhor uso dos núcleos, o que refletiria no tempo de execução. 
   ### Screenshots:
      ![swap](dotprod_seq/screenshots/swap.png?raw=true "swap")
      
      Execução com vetor de tamanho 300M fazendo swap.
      
      ![30M](dotprod_seq/screenshots/30M.png?raw=true "30M vector size")
      
      Execução do programa com vetor de tamanho 30M e alternando o nº de repetições, o qual faz bom uso da CPU sem precisar de swap.
      
      ![30Mgprof](dotprod_seq/screenshots/30M1exec.png?raw=true "30M vector size - gprofile screenshot")
      
      Execução com vetor de tamanho 30M e 1 repetição.
   ### Observações:
   Observou-se que com vetores de tamanho  300.000.000 toda a memória RAM é utilizada, assim se passa a fazer uso da memória swap, que deixa a execução mais lenta. 
   
   Nos vetores de tamanho grande (30 e 60 milhões) ao executar 1 repetição a maioria do tempo foi utilizado nas suas inicializações, sendo que a proporção entre o tempo gasto na chamada de 'init_vectors' e o tempo gasto na chamada de 'dot_product' gira em torno de 1,5 a 2. Já nas execuções com 10 ou mais repetições a maior parte do tempo foi utilizada na função 'dot_product', independente do tamanho de vetor dentro dos escolhidos.
   
----

## Parte 2
   ### Anotações:
   ### Respostas:
   ### Screenshots:
   ### Observações:

----

## Referências
- The Geek Stuff. GPROF Tutorial – How to use Linux GNU GCC Profiling Tool. http://www.thegeekstuff.com/2012/08/gprof-tutorial/.
- GNU gprof. Interpreting gprof's Output. https://ftp.gnu.org/old-gnu/Manuals/gprof-2.9.1/html_chapter/gprof_5.html.
