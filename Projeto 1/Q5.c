#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int main() {
  int n, numThreads;
  int **A;
  printf("Por favor digite o numero de variaveis e o numero de threads: ");
  scanf("%d %d", &n, &numThreads);

  A = (int **) malloc(n*sizeof(int *));
  for (int i = 0; i < n; i++) {
    A[i] = (int *) malloc(n*sizeof(int));
    for (int j = 0; j < n; j++) {
      scanf("%d", &A[i][j]);
    }
  }



  pthread_exit(NULL);
  return 0;
}