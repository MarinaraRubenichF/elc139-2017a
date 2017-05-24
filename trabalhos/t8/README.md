[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2017a) > Trabalhos

# T8: Quebra de senhas com OpenMPI

## Introdução

A **quebra de senhas** é uma operação de decodificação que pode ser utilizada para detectar vulnerabilidades de segurança em sistemas. De fato, se uma senha pode ser facilmente quebrada, é preferível que isso seja detectado preventivamente pelos administradores do sistema, antes que invasores explorem essa vulnerabilidade.

Neste trabalho, será utilizada uma versão do programa [MDCrack](http://c3rb3r.openwall.net/mdcrack/), que emprega o método da força bruta para quebrar senhas baseadas em algoritmos de hash comuns (MD5, por exemplo). O objetivo deste trabalho é desenvolver um programa paralelo, usando MPI, que use MDCrack para quebrar o maior número de senhas contidas num arquivo fornecido. Ou seja, este trabalho não visa paralelizar o programa MDCrack para quebra de uma única senha em paralelo, mas sim usá-lo eficientemente para quebrar um conjunto de senhas.

## Preparação

1. Obtenha os fontes do programa MDCrack 1.2 em http://c3rb3r.openwall.net/mdcrack/download/mdcrack-1.2.tar.gz. Descompacte o arquivo:
   ```
   tar xzvf mdcrack-1.2.tar.gz
   ```

2. Obtenha o patch disponível em [mdcrack-1.2p.patch](mdcrack-1.2p.patch) e salve-o na pasta mdcrack-1.2. Este patch faz uma pequena alteração no código para ele ser compilado sem problemas. Para aplicar este patch, digite:
   ```
   cd mdcrack-1.2
   patch -p1 < mdcrack-1.2p.patch
   ```
   
3. Compile o programa seguindo as instruções fornecidas com o código-fonte:
   ```
   cd mdcrack-1.2
   make little
   ```
   
4. Teste o programa da seguinte maneira:
   ```
   cd mdcrack-1.2
   bin/mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz 5e394281dfac81c1e7dddcaf4d35d1f6
   ```
   Neste exemplo, o conjunto de caracteres possíveis na senha é `abcdefghijklmnopqrstuvwxyz` e o código MD5 a quebrar é `5e394281dfac81c1e7dddcaf4d35d1f6`. Este comando deve produzir uma saída indicando que a senha foi quebrada e é igual a `aabb`:
   ```
   <<System>> MDcrack v1.2 is starting.
   <<System>> Sorting custom charset ... done
   <<System>> Using custom charset : abcdefghijklmnopqrstuvwxyz 
   <<System>> Max pass size = 12 >> Entering MD5 Core 1.

   Password size: 1 
   Password size: 2 
   Password size: 3 
   Password size: 4 

   ----------------------------------------
   Collision found ! => aabb

   <<Warning>> Session stopped in less than 10 ms,
   <<Warning>> not enough for a statistical report.
   ```
    
5. Para testar o programa com outras chaves, pode-se usar o comando `md5sum`, conforme o exemplo abaixo:
   ```
   echo -n aabb | md5sum
   ```
   Esse exemplo produz o código `5e394281dfac81c1e7dddcaf4d35d1f6` usado no exemplo acima. 


## Desenvolvimento

1. Este trabalho pode ser feito **em duplas ou individualmente**.

2. O programa paralelo deverá receber como entrada um arquivo contendo uma lista de hashes MD5 a quebrar, sendo um código por linha. O arquivo [crackme.txt](crackme.txt) contém um exemplo disso. Outros arquivos serão disponibilizados até a data de entrega final do trabalho.

3. O programa deverá distribuir o trabalho de quebra das várias senhas entre os processos que fazem parte do `MPI_COMM_WORLD`. Cada processo deverá processar uma ou mais senhas. 

4. O programa deve ser implementado segundo um modelo mestre-trabalhador, em que o processo mestre distribui as tarefas **dinamicamente** entre os processos trabalhadores ([Foster, seção 2.5](http://www.mcs.anl.gov/~itf/dbpp/text/node19.html#figLB2)). Cada processo trabalhador deverá chamar o programa MDCrack para quebrar cada senha fornecida.

5. O resultado da quebra de senhas deverá ser apresentado na mesma ordem em que as senhas se encontram no arquivo de entrada.

6. O desempenho do programa paralelo deverá ser analisado levando em conta diferentes dados de entrada e diferentes números de processos.


## Entrega

A entrega deste trabalho será em 2 partes:

### 1. Entrega parcial

No dia 01/06 (quinta-feira), no horário de aula, apresentar à professora, no laboratório LSC (sala 376, anexo B):
- Programa sequencial que faz a quebra de um lote de senhas. Este programa será usado como referência para análise de desempenho da versão paralela.
- Programa em MPI que testa a recepção de mensagens de quaisquer processos e descobre a origem do processo.


### 2. Entrega final

Até dia 12/06, 23:55 (segunda-feira), colocar o trabalho no seu repositório da disciplina, na pasta `trabalhos/t8`. Deve constar um documento `Entrega.md`, contendo:

- Identificação completa da disciplina e do(s) aluno(s);
- Links para os códigos desenvolvidos;
- Explicação da estratégia de paralelização adotada;
- Descrição dos experimentos realizados e discussão dos resultados obtidos;
- Referências.

**Atenção**: Trabalhos feitos em dupla deverão constar nos repositórios de ambos os integrantes, do contrário serão avaliados como trabalhos individuais.

No dia 13/06, no horário de aula, cada grupo ou aluno deverá apresentar o trabalho à professora no laboratório (LSC, sala 376). Na apresentação, deverá ser feita uma demonstração de execução e deverão ser respondidas perguntas sobre o desenvolvimento do trabalho.


## Material de apoio


- [Tutorial MPI](https://computing.llnl.gov/tutorials/mpi/)  
  Tutorial do Lawrence Livermore National Laboratory (LLNL) sobre MPI.
  
- [MDCrack](http://c3rb3r.openwall.net/mdcrack)  
Programa usado neste trabalho, de autoria de Gregory Duchemin.

- [Brute force attacks on cryptographic keys](http://www.cl.cam.ac.uk/~rnc1/brute.html)  
Texto de Richard Clayton, University of Cambridge, 2001.

- [MD5 (Message Digest Algorithm 5)](http://en.wikipedia.org/wiki/MD5)  
MD5 na Wikipedia.

- [RFC 1321 The MD5 Message-Digest Algorithm](http://tools.ietf.org/html/rfc1321)  
Referência oficial do algoritmo MD5, padronizado pela IETF (Internet Engineering Task Force).

- [Message Passing Interface (MPI) - Lawrence Livermore National Laboratory](http://www.llnl.gov/computing/tutorials/mpi/)  
Tutorial sobre MPI com vários exemplos.
