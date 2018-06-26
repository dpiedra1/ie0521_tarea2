all:
	mpicc src/ejercicio1.c -o ejercicio1
	mpicc src/exercise2.c -o exercise2

run:
	# mpiexec -n 4 ./ejercicio1
	mpiexec -n 4 ./exercise2

clean:
	rm ejercicio1
	rm exercise2
