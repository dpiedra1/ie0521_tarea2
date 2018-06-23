#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Max_NUM 500

/*
* This program finds the primes number in a range. Once the number is
* determinated to be prime, is safe in file .txt 
*/

int main(int argc, char const *argv[]) {
  clock_t begin = clock();
  FILE *file = fopen("primes.txt", "w");

  int prime;
  for (int Num = 2; Num < Max_NUM; Num++) {
    prime = 1;
    for (int i = 2; i < Num; i++) {
      if (Num%i == 0) {
        prime = 0;
        i = Num;
      }
    }
    if (prime == 1) {
      fprintf(file,"%d\n", Num );
    }

  }
  fclose(file);
  clock_t end = clock();
  double runtime = (double)(end - begin)/ CLOCKS_PER_SEC;
  printf("%f\n", runtime);
  return 0;
}
