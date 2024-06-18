#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 27

int invalidCount = 0;
pthread_mutex_t Mutex = PTHREAD_MUTEX_INITIALIZER;
int board[9][9] = {{0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0,0},};

void *validRow(void *arg) {
  // linha a ser checada
  int i0 = *((int *) arg);

  // array que conta a quantidade de aparicoes de cada numero
  int mark[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (int j = 0; j < 9; j++) {
    mark[board[i0][j] - 1]++;

    // enquanto for a primeira aparicao do numero, continue procurando
    if (mark[board[i0][j] - 1] == 1) continue;

    // se nao foi a primeira aparicao do numero, ou seja, houve
    // uma repeticao, o loop alcanca essa parte do codigo, acessa
    // a regiao critica e sinaliza que a linha checado pela thread
    // nao eh valida 
    pthread_mutex_lock(&Mutex);
    invalidCount++;
    pthread_mutex_unlock(&Mutex);
    
    // sai da thread ja que nao eh mais necessario checar
    pthread_exit(NULL);
  }
  pthread_exit(NULL);
}

void *validCollumn(void *arg) {
  // coluna a ser checada
  int j0 = *((int *) arg);

  // array que conta a quantidade de aparicoes de cada numero
  int mark[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 9; i++) {
    mark[board[i][j0] - 1]++;

    // enquanto for a primeira aparicao do numero, continue procurando
    if (mark[board[i][j0] - 1] == 1) continue;

    // se nao foi a primeira aparicao do numero, ou seja, houve
    // uma repeticao, o loop alcanca essa parte do codigo, acessa
    // a regiao critica e sinaliza que a coluna checado pela thread
    // nao eh valida 
    pthread_mutex_lock(&Mutex);
    invalidCount++;
    pthread_mutex_unlock(&Mutex);

    // sai da thread ja que nao eh mais necessario checar
    pthread_exit(NULL);
  }
  pthread_exit(NULL);
}

void *validSquare(void *arg) {
  // quadrado a ser checado
  int square = *((int *) arg);

  // coordenadas do canto do quadrado
  int i0 = 3 * (square / 3);
  int j0 = 3 * (square % 3);
  
  // array que conta a quantidade de aparicoes de cada numero
  int mark[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      mark[board[i0 + i][j0 + j] - 1]++;
      
      // enquanto for a primeira aparicao do numero, continue procurando
      if (mark[board[i0 + i][j0 + j] - 1] == 1) continue;

      // se nao foi a primeira aparicao do numero, ou seja, houve
      // uma repeticao, o loop alcanca essa parte do codigo, acessa
      // a regiao critica e sinaliza que o quadrado 3x3 checado pela 
      // thread nao eh valido 
      pthread_mutex_lock(&Mutex);
      invalidCount++;
      pthread_mutex_unlock(&Mutex);

      // sai da thread ja que nao eh mais necessario checar
      pthread_exit(NULL);
    }
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t myThread[THREADS]; // array de threads
  int ret;                     // retorno do thread_create e tabuleiro de sudoku
  void *(*func)(void *);       // ponteiro pra função que valida cada thread

  // le o tabuleiro de sudoku
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      scanf("%d", &board[i][j]);
    }
  }

  // func eh a rotina da thread                    
  for (int k = 0; k < THREADS; k++) {
    if (k == 0)  func = validRow;     // threads 0 a 8 fazem a checagem da linha
    if (k == 9)  func = validCollumn; // threads 9 a 17 fazem a checagem da coluna
    if (k == 18) func = validSquare;  // threads 18 a 26 fazem a checagem do quadrado 3x3

    int start = (k % 9);
    ret = pthread_create(&myThread[k], NULL, func, &start);
    if (ret) { printf("error pthread_create: %d\n", ret); exit(-1); }
  }

  // espera todas as threads realizarem a checagem do tabuleiro
  for (int i = 0; i < THREADS; i++) {
    pthread_join(myThread[i], NULL);
  }

  // se nenhuma thread identificou numeros repetidos numa
  // mesma linha, coluna ou quadrado 3x3, o tabuleiro eh valido
  if (invalidCount == 0) {
    printf("Tabuleiro valido\n");
  } else {
    printf("Tabuleiro invalido\n");
  }

  pthread_exit(NULL);
  return 0;
}