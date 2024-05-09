#include <pthread.h>
#include <stdio.h>

long contador = 0; // recurso compartilhado
void *inc(void *threadid){ 
    for(int i = 0; i < 9000000; i++) contador++;
}

void *dec(void *threadid){   
    for(int i = 0; i < 9000000; i++) contador--;
}

int main (int argc, char *argv[]){   
    pthread_t thread1, thread2;   
    pthread_create(&thread1, NULL, inc, NULL); 
    pthread_create(&thread2, NULL, dec, NULL); 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL); 
    printf("Valor final do contador: %ld\n", contador);
    pthread_exit(NULL);
}