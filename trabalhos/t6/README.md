[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2017a) > Trabalhos

# T6: Experiências com MPI

Se desejar usar MPI localmente em seu notebook/desktop, instale os pacotes openmpi-bin e libopenmpi-dev.

Para usar MPI nos servidores lsc4 e lsc5, use credenciais enviadas pela professora. O programa executável deverá ser estar disponível em uma mesma pasta em ambos os servidores (você pode fazer um script para fazer a cópia usando scp).

## Aquecimento

Esta parte do trabalho não precisa ser entregue. Seu objetivo é apenas exercitar a compilação e execução de programas MPI.

- Baixe o arquivo [hello_mpi.c](hello_mpi.c).

- Compile o programa:
  ```
  mpicc -o hello_mpi hello_mpi.c
  ```

- Execute o programa com 5 processos e observe onde são executados:
  ```
  mpiexec -np 5 hello_mpi
  ```

- Execute o programa com 10 processos distribuídos em 2 servidores:
  ```
  mpirun -np 10 -H lsc4,lsc5 hello_mpi
  ```


## Parte 1


+ Baixe o programa [dotprod_mpi.c](dotprod_mpi.c). Estude seu código-fonte, que usa MPI para calcular o produto escalar de 2 vetores.

+ Considerando a metodoogia vista na aula sobre o projeto de programas paralelos, explique como se dá o particionamento, a comunicação, a aglomeração e o mapeamento neste programa.

+ Compile o programa e execute-o nas configurações abaixo, cuidando para que não haja interferência significativa nas medições. Registre o tempo de execução e calcule o speedup obtido nos casos B, C, D e E (para simplificar, considere o caso A como uma execução sequencial).

  | Caso | Tamanho do vetor | Repetições | Num. de Processos | Host |
  | ---- | ---------------- | ---------- | ----------------- | ---- |
  | A    | 30000000 | 100 | 1 | lsc4 OU lsc5 |
  | B    | 30000000 | 100 | 2 | lsc4 OU lsc5 |
  | C    | 30000000 | 100 | 2 | lsc4 E lsc5 |
  | D    | 30000000 | 100 | 4 | lsc4 OU lsc5 |
  | E    | 30000000 | 100 | 4 | lsc4 E lsc5 |


## Parte 2

+ Implemente um programa MPI que transporte uma mensagem em um pipeline formado por processos de 0 a NP-1 (processo 0 envia para 1, processo 1 envia para 2, ..., processo NP-1 mostra o resultado). A mensagem é um número inteiro que é incrementado antes de ser passado adiante.

+ O programa [mpi_errado1.c](mpi_errado1.c) deveria realizar a troca de mensagens entre 2 processos, mas ele não funciona como esperado. Identifique o erro e corrija-o.

+ O programa [mpi_errado2.c](mpi_errado2.c) deveria realizar a troca de mensagens entre 2 processos, mas também não funciona como esperado. Identifique o erro e corrija-o.



## Entrega

No seu repositório da disciplina, na pasta `trabalhos/t6`, crie um documento `Entrega.md`, contendo:
 - Identificação completa da disciplina e do(a) aluno(a);
 - Respostas para as partes 1 e 2, incluindo links para os códigos desenvolvidos;
 - Referências.



## Material de apoio


- [Tutorial MPI](https://computing.llnl.gov/tutorials/mpi/)  
  Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre MPI.
