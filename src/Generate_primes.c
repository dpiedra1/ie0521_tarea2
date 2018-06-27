#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define Max_Counter 500000

/*
* This program finds the primes number in a range. Once the number is
* determinated to be prime, is safe in file .txt
*/

int main(int argc, char const *argv[]) {
  clock_t begin = clock();
  FILE *file = fopen("primes.txt", "w");

  int prime;
  int count = 0;
  int Num = 2;
  int SQRT;
  while(count < Max_Counter) {
    prime = 1;
    SQRT = (int)(sqrt((double)Num)+1);
    for (int i = 2; i < SQRT; i++) {
      if (Num%i == 0) {
        prime = 0;
        i = Num;
      }
    }
    if (prime == 1) {
      fprintf(file,"%d\n", Num );
      count++;
    }
    Num++;
  }

  fclose(file);
  clock_t end = clock();
  double runtime = (double)(end - begin)/ CLOCKS_PER_SEC;
  printf("Lineal time: %f\n", runtime);
  
  return 0;
}
