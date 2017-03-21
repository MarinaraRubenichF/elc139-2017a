#!/bin/bash

for i in 3000 30000 300000 3000000 30000000 60000000
do
	for j in 1 10 100 1000
	do
		./dotprod_seq $i $j
		gprof ./dotprod_seq gmon.out > outputs/output$i$j.txt
	done
done




