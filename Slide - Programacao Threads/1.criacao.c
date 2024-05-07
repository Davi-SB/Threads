#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *PrintHello(void *threadid){     
	printf("Ola, mundo!\n"); 
	pthread_exit(NULL); // boa pratica
}

int main(){   
	pthread_t MyThread;   
	int rc;   
	rc = pthread_create(&MyThread, NULL, PrintHello, NULL);      
	if(rc) { printf("ERRO; código de retorno é %d\n", rc); exit(-1); } // o retorno de create ser diferente de zero indica um erro

	// pthread_join(MyThread, NULL); // essa linha faria primeiro a MyThread terminar, ou seja, printar e depois sim a main poderia continuar sua execucao  
	printf("Main\n");
	pthread_exit(NULL); // aguarda todas as outras threads terminarem
	/*
	Se não fosse pela chamada de pthread_exit na função main, o programa poderia encerrar antes da execução completa da thread criada por pthread_create. Isso porque, em alguns sistemas, a thread principal pode terminar antes das outras threads, resultando na finalização de todo o programa e na possível interrupção das threads em execução.

	Portanto, pthread_exit é importante para garantir que todas as threads tenham a chance de completar sua execução antes do encerramento do programa. Ele permite que a thread principal (ou qualquer outra thread) saia de forma controlada, garantindo a execução completa das outras threads, se necessário.
	*/
} // gcc –pthread prog.c -o prog