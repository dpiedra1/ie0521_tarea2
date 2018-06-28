#include <stdio.h>
#include <stdlib.h>
#include  <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
      Function that calculates the weight in permutation
      graph_1d:      Pointer to the graph in 1 dimension
                     For example if graph = 1, 2, 3
                                            4, 5, 6
                                            7, 8, 9
                     Then graph_1d is a 1D array
                                    graph_1d = 1, 2, 3, 4, 5, 6, 7, 8, 9
                     The former example is what the function receives, NOT a
                     2 dimensional array.
      permutation:   It can be for example: [0, 5, 7, 6, 1, 2, 4, 3]
                     It means that the path is San Jose->Paraiso->San Isidro->
                     Puntarenas->Limon->San Francisco->Liberia->Alajuela->San Jose
                     Note that after the last city, in this case Alajuela (3),
                     it goes once again to San Jose(0). There is no need to
                     add the first city at the end of the permutation array.
      size:          Size of permutation, for example in the case above size is
                     8. Since the graph is a square matrix then size is also
                     the number of columns and rows of this matrix.
*/
// TODO: Finish this function
int calculate_weight(int *graph_1d, int *permutation, int size){
   int graph_pointer, Next_permutation;
   int weight = 0;

   for (int i = 0; i < size-1; i++) {
     Next_permutation = permutation[i+1];
     graph_pointer = permutation[i]*size + Next_permutation;
     weight = weight + graph_1d[graph_pointer];
   }

  //return to inicial city
  graph_pointer = Next_permutation*size+permutation[0];
  weight = weight + graph_1d[graph_pointer];

  //return weight;
  return weight;
}



/*
      Nothing important here, just calculates factorial of n parameter.
*/
int factorial(int n)
{
    if (n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

/*
      This function swaps to values in an array
                          ********Input parameters********
      i:             Index in array to swap with j
      j:             Index in array to swap with i
                          ********Input/output parameters********
      array:         array to swap values in indexes in i and j
*/
void swap(int *array, const int i, const int j){

  int temp;
  temp = array[i];
  array[i] = array[j];
  array[j] = temp;

}

/*
      This function recursively calculates all permutations of an int array
                          ********Input parameters********
      array:         Array to calculate all its possible permutations.
      size:          Array size.
      firtst_index:  Fisrt index to calculate permutation, ussually 0.
      last_index:    Last index to calculate permutation, ussually size - 1.
      result_index:  Pointer to index in result array to save each permutation.
                     Before calling this function this parameter should point
                     to int variable that holds a zero value. In fact you can
                     do whatever you want, this is just an advice.
                          ********Output parameters********
      result:        Array of pointers that holds pointers to each permutation.
                     It is your responsibility to allocate memory outside of
                     this function (do not forget to deallocate too).
*/
void permute(int *array, int size, int **result, int *result_index, int first_index, int last_index){

  int i;
  if(first_index == last_index){
    // New permutation
    int *permutation;
    permutation = (int *)malloc(size*sizeof(int));

    // Since memcpy do not care about what type of data it copies, it is needed
    // to specify size in bytes
    size_t byte_size = size*sizeof(int);

    // Copy data
    memcpy(permutation, array, byte_size);

    // Save data in result
    result[*result_index] = permutation;

    // Increment index to save iteration
    *result_index = *result_index + 1;
  }
  else{
    for(i = first_index; i<=last_index; i++){
      // Swap values
      swap(array, first_index, i);

      // Call permute recursively
      permute(array, size, result, result_index, first_index + 1, last_index);

      // Return to original array
      swap(array, first_index, i);
    }
  }

}

int main(int argc, char *argv[]){

  int rank, total_processes;
  MPI_Init(&argc, &argv);

  int root = 0;

  // World communication
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

  // We need two int arrays to specify number of elements to each process and
  // the offset
  int *sendcounts = (int *)malloc(total_processes*sizeof(int));
  int *displs = (int *)malloc(total_processes*sizeof(int));
  int *recvbuf;
  int recvcount;
  int *weight;

  if(rank == root){

    // DEFINE WEIGHTED GRAPH HERE
    int graph[8][8] = {{ 0,   115, 8,   17,  167, 26,  83,  75  },
                       { 115, 0,   120, 129, 272, 92,  197, 100 },
                       { 8,   120, 0,   9,   160, 34,  78,  83  },
                       { 17,  129, 9,   0,   151, 43,  69,  91  },
                       { 167, 272, 160, 151, 0,   193, 98,  236 },
                       { 26,  92,  34,  43,  193, 0,   108, 55  },
                       { 83,  197, 78,  69,  98,  108, 0,   141 },
                       { 75,  100, 83,  91,  236, 55,  141, 0   }
                      };

    // Generate list of indexes
    int v_t[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(v_t)/sizeof(int);

    // Calculate total number of possible permutations
    int num_permutations = factorial(size);

    // Create and array of pointers to each permutation
    int **result;
    result = (int **)malloc(num_permutations*sizeof(int *));

    // Permute needs a variable to save index in result, starting in 0
    int result_index = 0;
    int *ptr_result_index = &result_index;

    // Calculate all possible permutations
    permute(v_t, size, result, ptr_result_index, 0, size-1);

    // Print all permutations (just for debugging)
    // Uncomment this code to print all permutations
    /*for(int i=0; i < num_permutations; i++){
      printf("Permutation: %d \n", i);
      for(int j=0; j<size; j++){
        printf("%d, ", result[i][j]);
      }
      printf("\n");
    }*/

    // Broadcast size and number of permutations
    MPI_Bcast(&size, 1, MPI_INT, root, comm);
    MPI_Bcast(&num_permutations, 1, MPI_INT, root, comm);

    // It is easier to pass a 1D array through MPI.
    // Convert result in 1D array
    int *permutations;
    permutations = (int *)malloc(num_permutations*size*sizeof(int));
    for(int i=0; i<num_permutations; i++){
      for(int j=0; j<size;j++){
        permutations[i*size + j] = result[i][j];
      }
    }

    // Broadcast all permutations to all processes (easier than pass each
    // permutation every time)
    // Then it is only needed to pass permutation index
    MPI_Bcast(permutations, num_permutations*size, MPI_INT, root, comm);

    // Convert graph in 1D array
    int *graph_1d;
    graph_1d = (int *)malloc(size*size*sizeof(int));
    for(int i=0; i<size; i++){
      for(int j=0; j<size;j++){
        graph_1d[i*size + j] = graph[i][j];
      }
    }

    // Broadcast weighted graph
    MPI_Bcast(graph_1d, size*size, MPI_INT, root, comm);

    // Create an array of permutation indexes
    int *permute_indexes;
    permute_indexes = (int *)malloc(num_permutations*sizeof(int));
    for(int i=0; i<num_permutations; i++){
      permute_indexes[i] = i;
    }

    // Distribute indexes to all processes
    int remainder = num_permutations % total_processes;

    int whole_part = (num_permutations - remainder) / total_processes;

    // Distribute whole part
    for(int i=0; i < total_processes; i++){
      sendcounts[i] = whole_part;
    }
    // Distribute remainder
    for(int i=1; i < total_processes; i++){
      if(remainder == 0){
        break;
      }
      else{
        sendcounts[i]++;
        remainder--;
      }
    }

    // Root process has zero offset
    displs[0] = 0;

    // Assign offset to other processes
    for(int i=1; i < total_processes; i++){
      displs[i] = displs[i-1] + sendcounts[i-1];
    }

    recvbuf = (int *)malloc(sendcounts[rank]*sizeof(int));
    recvcount = sendcounts[rank];

    // Distribute data
    // **************************************************************************
    MPI_Scatterv(permute_indexes, sendcounts, displs, MPI_INT, recvbuf, recvcount,
                 MPI_INT, root, comm);

    // Allocate memory for result
    weight = (int *)malloc(recvcount*sizeof(int));
    for(int i=0; i<recvcount; i++){
      // FIXME:
      weight[i] = calculate_weight(graph_1d,result,num_permutations);
    }

    // Allocate memory to receive data
    int *final_result = (int *)malloc(num_permutations*sizeof(int));

    // Receive result
    MPI_Gatherv(weight, recvcount, MPI_INT, final_result, sendcounts, displs,
                MPI_INT, root, comm);

    // Free memory
    free(graph_1d);
    free(permutations);
    for(int i=0; i<num_permutations; i++){
      free(result[i]);
    }
    free(result);

  }
  // For other processes
  else{
    // Receive number of permutations and size of each array
    int size, num_permutations;
    MPI_Bcast(&size, 1, MPI_INT, root, comm);
    MPI_Bcast(&num_permutations, 1, MPI_INT, root, comm);

    // Allocate memory for permutations
    int *permutations;
    permutations = (int *)malloc(num_permutations*size*sizeof(int));

    // Receive permutations
    MPI_Bcast(permutations, num_permutations*size, MPI_INT, root, comm);

    int *graph_1d;
    // Allocate memory for weighted graph
    graph_1d = (int *)malloc(size*size*sizeof(int));

    // Receive weighted graph
    MPI_Bcast(graph_1d, size*size, MPI_INT, root, comm);


    int remainder = num_permutations % total_processes;

    int whole_part = (num_permutations - remainder) / total_processes;

    // Distribute whole part
    for(int i=0; i < total_processes; i++){
      sendcounts[i] = whole_part;
    }
    // Distribute remainder
    for(int i=1; i < total_processes; i++){
      if(remainder == 0){
        break;
      }
      else{
        sendcounts[i]++;
        remainder--;
      }
    }

    // Root process has zero offset
    displs[0] = 0;

    // Assign offset to other processes
    for(int i=1; i < total_processes; i++){
      displs[i] = displs[i-1] + sendcounts[i-1];
    }

    recvbuf = (int *)malloc(sendcounts[rank]*sizeof(int));
    recvcount = sendcounts[rank];

    // Distribute data
    // **************************************************************************
    MPI_Scatterv(NULL, sendcounts, displs, MPI_FLOAT, recvbuf, recvcount,
                 MPI_FLOAT, root, comm);

    // Allocate memory for result
    weight = (int *)malloc(recvcount*sizeof(int));

    // Calculate weight
    for(int i=0; i<recvcount; i++){
     // FIXME
     weight[i] = calculate_weight(NULL,NULL,0);
    }

    if(rank == 1){
      for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
          printf("%d,", graph_1d[i*size + j]);
        }
        printf("\n");
      }
    }

    MPI_Gatherv(weight, recvcount, MPI_INT, NULL, sendcounts, displs,
                MPI_INT, root, comm);

    // Free memory
    free(graph_1d);
    free(permutations);
  }

  // Free memory common for all processes
  // **************************************************************************
  free(weight);
  free(sendcounts);
  free(displs);
  free(recvbuf);

  MPI_Finalize();
  return 0;

}
