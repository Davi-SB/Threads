#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 3


void* agendar(void*arg);
void* escalonador(void*arg);
void* rotina1(void*arg);
//void* rotina2(void*arg);
//void* rotina3(void*arg);

void* agendar(void* arg) {
    for (int i = 0; i < 100; i++){ // abastece o a lista_pronto, adiciona tarefas pendentes
        // trava mutex fila
        // push na nossa fila de rotinas (o que thread deve executar quando for criada)
        // pthread_cond_signal(&condEscalonador);
        // destrava mutex fila
    }
    pthread_exit(NULL);
}

void* escalonador(void* arg) {
    /*
    while(1) {
        // trava mutex fila
        // trava mutex nThreads
        while(nThreadsRodando < N && !fila.empty()) { // enquanto puder ter mais threads rodando E threads (rotinas) prontas  
            ponteiroPFuncaoVoid rotina = fila.pop();
            nThreadsRodando++;
            pthread_create(tread, NULL, rotina, (void*)arg);
        }
        pthread_cond_wait(&condEscalonador)
        // destrava mutex fila
        // destrava mutex nThreads
    }
    */
}

void* rotina1(void* arg) {
    // codigo da funcao aqui
    for (int i = 0; i < 9999; i++);

    // trava mutex nThreads            
    // nThreadsRodando--;
    // pthread_cond_signal(&condEscalonador);
    // destrava mutex nThreads
    pthread_exit(NULL);
}

int main() {


    pthread_exit(NULL);
}