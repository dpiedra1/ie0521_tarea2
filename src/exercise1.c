#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_ROWS 6
#define NUM_COLS 5


int main(int argc, char *argv[]){

  int rank, total_processes;
  MPI_Init(&argc, &argv);

  int root = 0;

  // World communication
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_processes);

  if(rank == 0){
    // Define matrix and 1D vector only in root process
    // ********************************************************************
    float result[NUM_ROWS];

    float vector[NUM_COLS] = {1.0,1.0,1.0,1.0,1.0};
    float matrix[NUM_ROWS][NUM_COLS] = {{1, 1, 1, 1, 1},
                                        {2, 2, 2, 2, 2},
                                        {3, 3, 3, 3, 3},
                                        {4, 4, 4, 4, 4},
                                        {5, 5, 5, 5, 5},
                                        {6, 6, 6, 6, 6}};

    // Get matrix and vector dimensions
    // ********************************************************************
    // Vector and matrix could be of different dimensions. We do not want
    // to hardcode this values (in fact they are, but suppose they are not)
    // so if we use NUM_COLS and NUM_ROWS then we are cheating. That is why we
    // get this values again.
    // In this case vector size is the same as column size, since we suppose
    // that this is a multiplication of matrix by column vector
    int num_cols = sizeof(vector)/sizeof(float);
    int num_rows = sizeof(matrix) / sizeof(matrix[0]);

    // Broadcast vector and matrix size
    MPI_Bcast(&num_cols, 1, MPI_INT, root, comm);
    MPI_Bcast(&num_rows, 1, MPI_INT, root, comm);

    // Broadcast vector
    MPI_Bcast(vector, num_cols, MPI_FLOAT, root, comm);

    int answer;
    float result_value;
    int message = 1;
    MPI_Status status;

    // Distribute work among slave processes
    // ----------------------------------------------------------------------
    // Since root process is sending several messages to same slave process,
    // this can cause that messages are not sended or received in expected
    // order (this is a huge problem since we expect them in order).
    // That is why we use tags. Sometimes is sayed that tags are optional, but
    // in this case they are not. If we use the same tag (for example 0) then
    // most of the times nothing works.
    for(int row_number=0; row_number<num_rows;row_number++){
      MPI_Recv(&answer,1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);

      // Send a message with row number of the matrix (value different from -1)
      message = row_number;
      MPI_Send(&message, 1, MPI_INT, status.MPI_SOURCE, 1, comm);

      // Send row
      MPI_Send(matrix[row_number], num_cols, MPI_FLOAT, status.MPI_SOURCE, 2, comm);

      // Receive result
      MPI_Recv(&result_value,1, MPI_FLOAT, MPI_ANY_SOURCE, 3, comm, &status);
      result[row_number] = result_value;
    }

    message = -1;
    for(int process=1; process<total_processes; process++){
      MPI_Send(&message, 1, MPI_INT, process, 1, comm);
    }

    // Print result
    printf("Printing result...\n");
    for(int i =0; i<num_rows; i++){
      if(i != num_rows-1){
        printf("%f, ", result[i]);
      }
      else{
        printf("%f", result[i]);
      }
    }
    printf("\n");

  }else{
    int num_cols, num_rows;
    // Receive vector and matrix size
    MPI_Bcast(&num_cols, 1, MPI_INT, root, comm);
    MPI_Bcast(&num_rows, 1, MPI_INT, root, comm);

    // Receive vector
    float *vector;
    vector = (float *)malloc(num_cols*sizeof(float));
    MPI_Bcast(vector, num_cols, MPI_FLOAT, root, comm);

    // Allocate memory for specific row
    float *row;
    row = (float *)malloc(num_cols*sizeof(float));

    int answer;
    int message = 1;

    // Receive work from master process
    // ----------------------------------------------------------------------
    while(1){
      MPI_Send(&message, 1, MPI_INT, 0, 0, comm);
      MPI_Recv(&answer,1, MPI_INT, 0, 1, comm, MPI_STATUS_IGNORE);
      if(answer == -1){
        break;
      }else{
        // Receive row
        MPI_Recv(row, num_cols, MPI_FLOAT, 0, 2, comm, MPI_STATUS_IGNORE);

        // Calculate result
        float result_entry = 0;
        for(int i=0;i<num_cols; i++){
          result_entry = result_entry + row[i]*vector[i];
        }
        // Send result
        MPI_Send(&result_entry, 1, MPI_FLOAT, 0, 3, comm);

      }
    }

    // Free memory
    free(row);
    free(vector);

  }

  MPI_Finalize();
  return 0;

}
