all:
	# Parallel code
	mpicc src/exercise1.c -o exercise1
	mpicc src/exercise2.c -o exercise2
	mpicc src/exercise3.c -lm -o exercise3

	# Sequential code
	gcc -Wall src/exercise1_sec.c -o exercise1_sec
	gcc -Wall src/TPS.c -o exercise2_sec
	gcc -Wall src/Generate_primes.c -o exercise3_sec

run:
	mpiexec -n 4 ./exercise1
	mpiexec -n 4 ./exercise2
	mpiexec -n 4 ./exercise3
	./exercise1_sec

sec:
	gcc -Wall src/exercise1_sec.c -o exercise1_sec
	gcc -Wall src/TPS.c -o exercise2_sec
	gcc -Wall src/Generate_primes.c -o exercise3_sec


clean:
	rm exercise1
	rm exercise2
	rm exercise3
	rm exercise1_sec
	rm exercise2_sec
	rm exercise3_sec
	rm MPI_primes.txt
