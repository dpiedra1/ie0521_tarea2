all:
	mpicc src/ejercicio1.c -o ejercicio1

run:
	mpiexec -n 4 ./ejercicio1

clean:
	rm ejercicio1
