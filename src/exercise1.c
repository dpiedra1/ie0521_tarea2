#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  // In this test root process is one that has rank 0
  int root = 0;

  // World communication
  MPI_Comm comm = MPI_COMM_WORLD;

  int rank, total_processes;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

  // Create array only if root process
  // In this case array size is the same as number of columns
  int array_size;
  int num_row;

  // Array for slave process
  float *recv_array;
  int *indexes;

  // 1D Matrix
  float *matrix_1D;

  // Define a buffer to receive data
  int *recvbuf;
  int recvcount;

  // Result
  float *result;

  // We need two int arrays to specify number of elements to each process and
  // the offset
  int *sendcounts = (int *)malloc(total_processes*sizeof(int));
  int *displs = (int *)malloc(total_processes*sizeof(int));

  if(root == rank){

    // Define matrix and 1D array only in root process
    // ********************************************************************
    float array[5] = {1.0,1.0,1.0,1.0,1.0};
    float matrix[6][5] = {{1, 1, 1, 1, 1},
                          {2, 2, 2, 2, 2},
                          {3, 3, 3, 3, 3},
                          {4, 4, 4, 4, 4},
                          {5, 5, 5, 5, 5},
                          {6, 6, 6, 6, 6}};

    // Get data from both arrays
    array_size = sizeof(array)/sizeof(float);
    num_row = sizeof(matrix) / sizeof(matrix[0]);

    // Broadcast array size to other process
    MPI_Bcast(&array_size, 1, MPI_INT, root, comm);

    // Broadcast number of rows to other process
    MPI_Bcast(&num_row, 1, MPI_INT, root, comm);

    // Broadcast array to other processes
    MPI_Bcast(array, array_size, MPI_FLOAT, root, comm);

    // Convert matrix to 1D array
    matrix_1D = (float*)malloc(array_size*num_row*sizeof(float));
    for(int i=0;i<num_row;i++){
      for(int j=0;j<array_size;j++){
        matrix_1D[i*array_size + j] = matrix[i][j];
      }
    }

    // Broadcast matrix to all processes
    MPI_Bcast(matrix_1D, array_size*num_row, MPI_FLOAT, root, comm);

    int remainder = num_row % total_processes;

    int whole_part = (num_row - remainder) / total_processes;

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

    // Indexes of the matrix
    // **************************************************************************
    indexes = (int*)malloc(num_row*sizeof(int));
    for(int i=0;i<num_row;i++){
      indexes[i] =i;
    }
    MPI_Scatterv(indexes, sendcounts, displs, MPI_INT, recvbuf, recvcount,
                 MPI_INT, root, comm);

    // Make calcs
    // **************************************************************************
    result = (float*)malloc(recvcount*sizeof(float));
    for(int i = 0; i<recvcount; i++){
      float sum = 0.0;
      int index = recvbuf[i];
      for(int j=0; j<array_size; j++){
        sum = sum + matrix_1D[index*array_size + j]*array[j];
      }
      result[i] = sum;
    }

    // **************************************************************************

    // Return data to root process
    // **************************************************************************
    float *final_result;
    final_result = (float *)malloc(num_row*sizeof(float));

    MPI_Gatherv(result, recvcount, MPI_FLOAT, final_result, sendcounts, displs,
               MPI_FLOAT, root, comm);

    printf("Printing final result..\n" );
    for(int i=0; i<num_row; i++){
      printf("%f\n", final_result[i]);
    }

    // Free memory
    free(final_result);
    free(indexes);


  }
  // When non root process
  else{
    // Get array size from root process
    MPI_Bcast(&array_size, 1, MPI_FLOAT, root, comm);

    // Get number of rows of matrix from root process
    MPI_Bcast(&num_row, 1, MPI_INT, root, comm);

    // Receive array
    recv_array = (float*)malloc(array_size*sizeof(float));
    MPI_Bcast(recv_array, array_size, MPI_FLOAT, root, comm);

    // Broadcast matrix to all processes
    matrix_1D = (float*)malloc(array_size*num_row*sizeof(float));
    MPI_Bcast(matrix_1D, array_size*num_row, MPI_FLOAT, root, comm);

    int remainder = num_row % total_processes;

    int whole_part = (num_row - remainder) / total_processes;

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

    // Assing offset to other processes
    for(int i=1; i < total_processes; i++){
      displs[i] = displs[i-1] + sendcounts[i-1];
    }

    recvbuf = (int *)malloc(sendcounts[rank]*sizeof(int));
    recvcount = sendcounts[rank];

    // Distribute data
    // **************************************************************************
    MPI_Scatterv(NULL, sendcounts, displs, MPI_INT, recvbuf, recvcount,
                 MPI_INT, root, comm);

    // Make calcs
    // **************************************************************************
    result = (float*)malloc(recvcount*sizeof(float));
    for(int i = 0; i<recvcount; i++){
     float sum = 0.0;
     int index = recvbuf[i];
     for(int j=0; j<array_size; j++){
       sum = sum + matrix_1D[index*array_size + j]*recv_array[j];
     }
      result[i] = sum;
    }

    // **************************************************************************

    // Return data to root process
    // **************************************************************************

    MPI_Gatherv(result, recvcount, MPI_FLOAT, NULL, sendcounts, displs,
               MPI_FLOAT, root, comm);
  }

  // Free memory
  // **************************************************************************
  free(sendcounts);
  free(displs);
  free(matrix_1D);
  free(recvbuf);
  free(result);
  free(recv_array);

  MPI_Finalize();
  return 0;
}
