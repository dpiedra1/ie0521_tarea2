#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define Max_NUM 500

int main (int argc, char **argv)
{
  int rank, ierr, size;


  /*Create child proccesses, each of which has its own variables.
  * From this point on, every process has a diferent process ID,
  * ranging form 0 to num_procs minus 1, and COPIES of all
  * variables defined in the program. No variables are shared
  */
  ierr = MPI_Init(&argc, &argv);
  /*
  * Find out MY process ID, and how many process were started.
  */
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);
  /*
  * Segment the algoritm to define the single task that every process has to
  * solved. The mastar (rank = 0 ), send a number to the slaves. The slaves
  * evaluate in range (deteminated by the number) if the numbers are prime.
  */
  if( rank == 0){
      int Num = (Max_NUM/(size-1));
      for (int i = 1; i < size; i++) {
        MPI_Send( &Num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        Num= Num + Max_NUM/(size-1);
      }
  }
  /*
  * Find if the number evaluated is a multiple of other number. If not print it.
  */
  else{
    int Num_recv = -1;
    MPI_Recv(&Num_recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int prime;
    int init;
    if (Num_recv-Max_NUM/(size-1) == 0) {
      init = 2;
    }
    else{
      init = Num_recv-Max_NUM/(size-1);
    }

    for (int Num = init ; Num < Num_recv; Num++) {
        prime = 1;
        for (int j = 2; j < Num; j++) {
              if (Num%j == 0) {
                prime=0;
                j = Num;
            }
        }
        if (prime ==1) {
            printf("%d\n", Num );
        }
    }

  }


  ierr = MPI_Finalize();


  return 0;
}
