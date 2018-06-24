all:
	mpicc src/ejercicio1.c -o ejercicio1
	gcc src/exercise2.c -o exercise2

run:
	mpiexec -n 4 ./ejercicio1

clean:
	rm ejercicio1
	rm exercise2
