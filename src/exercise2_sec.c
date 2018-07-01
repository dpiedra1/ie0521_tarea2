#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
      Function that calculates the weight in permutation
      graph_1d:      Pointer to the graph in 1 dimension
                     For example if graph = 1, 2, 3
                                            4, 5, 6
                                            7, 8, 9
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

int calculate_weight(int *graph_1d, int *permutation, int size){
   int graph_pointer, Actual_permutation, Next_permutation;
   int weight = 0;

   for (int i = 0; i < size-1; i++) {
     Next_permutation = permutation[i+1];
     Actual_permutation = permutation[i];
     graph_pointer = permutation[i]*size + Next_permutation;
     weight = weight + graph_1d[graph_pointer];
   }


  //return to inicial city
  graph_pointer = Next_permutation*size+permutation[0];
  weight = weight + graph_1d[graph_pointer];

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

        // Convert graph in 1D array
        int *graph_1d;
        graph_1d = (int *)malloc(size*size*sizeof(int));
        for(int i=0; i<size; i++){
          for(int j=0; j<size;j++){
            graph_1d[i*size + j] = graph[i][j];
          }
        }


        // Calculate weights
        int weight, smallest_path;
        int smallest_weight = 1000000;

          for (int j = 0; j < num_permutations; j++) {
            weight = calculate_weight(graph_1d, result[j], size);
            if (weight < smallest_weight) {
              smallest_weight = weight;
              smallest_path = j;
            }
          }

        for (int i = 0; i < size; i++) {
          int position = result[smallest_path][i];
          switch (position) {
            case 0: {printf("San José-->");
                    break;}
            case 1:{ printf("Limón -->");
                    break;}
            case 2: {printf("San Francisco-->");
                    break;}
            case 3: {printf("Alajuela-->");
                    break;}
            case 4: {printf("Liberia-->");
                    break;}
            case 5: {printf("Paraíso-->");
                    break;}
            case 6: {printf("Puntarenas-->");
                    break;}
            case 7: {printf("San Isidro-->");
                    break;}
          }

        }
        printf("%d\n", result[smallest_path][0]);
        printf("Smallest weight: %d\n", smallest_weight);

  return 0;

}
