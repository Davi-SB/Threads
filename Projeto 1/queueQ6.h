#ifndef QUEUEQ6 
#define QUEUEQ6
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	void *(*func)(void *);
	Node *next;
}Node;


typedef struct queue{
	Node *front;
	Node *rear;
	int size;
} Queue;

typedef struct thread_data {
  int idConta, idCliente; // Diferentes clientes A e B podem compartilhar uma conta j
  Queue *q;
} Thread_data;

Node *createNode(void *(*func0)(void *)) {
	Node *n = (Node *) malloc(sizeof(Node));
	n->func = func0;
    n->next = NULL;
	return n;
}

Queue *createQueue() {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->front = q->rear = createNode(NULL); // header Node
    q->size = 0;
    return q;
}

void enqueue(Queue *q, void *(*func0)(void *)) { // "append"
    q->rear->next = createNode(func0);
    q->rear = q->rear->next;
    q->size++;
}

Escolha dequeue(Queue *q) { // remove o primeiro, q->front->Pnext
    if(q->size == 0) { printf("Cuidado!!! fila vazia\n"); exit(-1); }
    Escolha it = (q->front)->next->escolha;
    
    Node *temp = (q->front)->next; // (q->front) eh o header Node
    (q->front)->next = (q->front)->next->next;
    free(temp);
    
    if((q->front)->next == NULL) q->rear = q->front; // empty queue
    q->size--;
    return it;
}

int queueEmpty(Queue *q) {
  return (q->size == 0);
}

void deleteQueue(Queue* q) { // libera memoria alocada para a lista e os Nodes
    while (q->size > 0) {
        Node *temp = q->front; // temp recece a front inicial/atual
        q->front = q->front->next; // front passa a apontar para o proximo
        free(temp);
    }
    free(q); // q eh um ponteiro que foi alocado com malloc
}

#endif