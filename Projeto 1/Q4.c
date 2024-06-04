#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 27 // 9 validRow, 9 validColumn e 9 validSquare

int invalidCount = 0;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_data {
  int **board;
  int start;
} Thread_data;

void *validRow(void *arguments) {
  Thread_data *data = arguments;

  int mark[9] = {};
  int i0 = data->start;
  
  for (int j = 0; j < 9; j++) {
    mark[(data->board)[i0][j]-1]++;
    if (mark[(data->board)[i0][j]-1] == 1) continue;
    
    pthread_mutex_lock(&Mutex); // invalidCount eh um recurso comparilhado
    invalidCount++;
    pthread_mutex_unlock(&Mutex);
    pthread_exit(NULL);
  }
  pthread_exit(NULL);
}

void *validColumn(void *arguments) {
  Thread_data *data = arguments;

  int mark[9] = {};
  int j0 = data->start;
  
  for (int i = 0; i < 9; i++) {
    mark[(data->board)[i][j0]-1]++;
    if (mark[(data->board)[i][j0]-1] == 1) continue;

    pthread_mutex_lock(&Mutex); // invalidCount eh um recurso comparilhado
    invalidCount++;
    pthread_mutex_unlock(&Mutex);
    pthread_exit(NULL);    
  }
  pthread_exit(NULL);    
}

void *validSquare(void *arguments) {
  Thread_data *data = arguments;

  int mark[9] = {};
  int i0 = 3*((data->start)/3), j0 = 3*((data->start)%3);
 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mark[(data->board)[i0+i][j0+j]-1]++;
      if (mark[(data->board)[i0+i][j0+j]-1] == 1) continue;

      pthread_mutex_lock(&Mutex); // invalidCount eh um recurso comparilhado
      invalidCount++;
      pthread_mutex_unlock(&Mutex);
      pthread_exit(NULL);
    }
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t myThread[THREADS]; // threads
  int ret, **board;            // retorno do thread_create e tabuleiro de sudoku
  void *(*func)(void *);       // ponteiro pra funcao que valida cada thread
  
  board = (int **) malloc(9*sizeof(int *));
  
  for (int i = 0; i < 9; i++) {
    board[i] = (int *) malloc(9*sizeof(int));
    for (int j = 0; j < 9; j++) {
      scanf("%d", &board[i][j]);
    }
  }

  func = validRow;
  for (int k = 0; k < THREADS; k++) {
    // if (k == 0) func = validRow;
    if (k == 9)  func = validColumn;
    if (k == 18) func = validSquare;
    
    Thread_data data = { board, k%9 };
    ret = pthread_create(&myThread[k], NULL, func, &data);
    if(ret) { printf("error pthread_create: %d\n", ret); exit(-1); }
  }

  for (int i = 0; i < THREADS; i++) {
    pthread_join(myThread[i], NULL);
  }

  printf("Tabuleiro ");
  if (invalidCount) printf("in");
  printf("valido\n");

  for (int i = 0; i < 9; i++) {
    free(board[i]);
  } free(board);

  pthread_exit(NULL);
  return 0;
}