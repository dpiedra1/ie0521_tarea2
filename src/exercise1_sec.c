#include <stdlib.h>
#include <stdio.h>
// Multiple matrix by column vector with sequential code

int main(int argc, char *argv[]){

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
  // Since the vector is a column vector, then array_size is same as
  // the number of columns of the matrix
  int array_size = sizeof(array)/sizeof(float);
  int num_row = sizeof(matrix) / sizeof(matrix[0]);

  // Array for store the result
  float *result = (float*)malloc(num_row*sizeof(float));

  // Multiple matrix by vector
  for(int i=0; i<num_row; i++){
    int sum = 0;
    for(int j=0; j<array_size; j++){
      sum = sum + matrix[i][j]*array[j];
    }
    result[i] = sum;
  }

  // Print result
  printf("Result array is:\n");
  for(int i=0; i<num_row; i++){
    printf("%f ", result[i]);
  }
  printf("\n");

  // Free memory
  free(result);

}
