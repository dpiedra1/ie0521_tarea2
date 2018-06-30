all:
	# Parallel code
	mpicc src/exercise1.c -o exercise1
	mpicc src/exercise2.c -o exercise2
	mpicc src/exercise3.c -lm -o exercise3

	# Sequential code
	gcc -Wall src/exercise1_sec.c -o exercise1_sec

run:
	mpiexec -n 4 ./exercise1
	mpiexec -n 4 ./exercise2
	mpiexec -n 4 ./exercise3
	./exercise1_sec

clean:
	rm exercise1
	rm exercise2
	rm exercise3
	rm exercise1_sec
	rm MPI_primes.txt
