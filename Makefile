all:
	mpicc src/exercise1.c -o exercise1
	mpicc src/exercise2.c -o exercise2
	mpicc src/exercise3.c -lm -o exercise3

run:
	mpiexec -n 4 ./exercise1
	mpiexec -n 4 ./exercise2
	mpiexec -n 4 ./exercise3

clean:
	rm exercise1
	rm exercise2
	rm exercise3
