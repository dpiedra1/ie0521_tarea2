#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define Max_NUM 7368789

int main (int argc, char **argv)
{
  int rank, ierr, size, branch, count_primes;
  double start, end;
  FILE* file;
  count_primes = 0;


  /*Create child proccesses, each of which has its own variables.
  * From this point on, every process has a diferent process ID.
  */
  ierr = MPI_Init(&argc, &argv);
  start = MPI_Wtime();
  file = fopen("MPI_primes.txt", "a");

  /*
  * Find out MY process ID, and how many process were started.
  */
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

  /*
  * Segment the algoritm to define the numbers that every process has to
  * check.
  */

  int prime =1;
  int SQRT, end_check, Max_Counter;
  Max_Counter = ceil(5000000/size);
  int start_check = 2 + ceil(Max_NUM/size)*rank;
  if (start_check + ceil(Max_NUM/size) > Max_NUM) {
    end_check = Max_NUM;
  }
  else{
    end_check = start_check + ceil(Max_NUM/size);
  }



      for (int Num = start_check; Num < end_check ; Num++) {
        prime = 1;
        SQRT = (int)(sqrt((double)Num)+1); //Maxime number to check, based on Criba
        for (int i = 2; i < SQRT; i++) {
            if (Num%i == 0) {
              prime = 0;
              i = Num;
            }
        }
        if (prime == 1) {// If theres no divisers, the number is prime
          count_primes++;
          fprintf(file, "%d\n", Num);
        }

        if (count_primes > Max_Counter ){
          Num = Max_NUM;
        }
      }

  end = MPI_Wtime();
  ierr = MPI_Finalize();

  fclose(file);
  printf("Total time = %f\n", (end-start));

  return 0;
}
