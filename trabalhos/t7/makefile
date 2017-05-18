EXECS=dijkstra
MPICC?=mpicc

all: ${EXECS}

dijkstra: Dijkstra.c
	${MPICC} -o dijkstra Dijkstra.c

clean:
	rm -f ${EXECS}
	
