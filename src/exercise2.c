//#include  <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

  // Free memory
  for(int i=0; i<num_permutations; i++){
    free(result[i]);
  }
  free(result);

}
