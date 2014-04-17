seq: seq.c
	g++ seq.c -o seq -Wall -lrt
par: par.c
	g++ -fopenmp par.c -o par -Wall -lrt
