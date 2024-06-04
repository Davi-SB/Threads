#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CLIENTES 3
#define NUM_OPS 5 //Número de operações por cliente

typedef struct escolhas{
    int idConta, idCliente;
    int op;
    int valorOP;
} Escolha;

typedef struct link {
    Escolha escolha;
    struct link *next;
} Link;

typedef struct queue{
    Link *front;
    Link *rear;
    int size;
} Queue;

typedef struct thread_data {
  int idConta, idCliente; // Diferentes clientes A e B podem compartilhar uma conta j
  Queue *q;
} Thread_data;

Link* createHeader() {
    Link *n = (Link *) malloc(sizeof(Link));
    n->next = NULL;
    return n;
}

Link* createLink(Escolha escolha0) {
    Link *n = (Link *) malloc(sizeof(Link));
    n->escolha.idCliente = escolha0.idCliente;
    n->escolha.idConta   = escolha0.idConta;
    n->escolha.op        = escolha0.op;
    n->escolha.valorOP   = escolha0.valorOP;
    n->next = NULL;
    return n;
}

Queue *createQueue() {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->front = q->rear = createHeader(); // header Link
    q->size = 0;
    return q;
}

void enqueue(Queue *q, Escolha escolha0) { // "append"
    q->rear->next = createLink(escolha0);
    q->rear = q->rear->next;
    q->size++;
}

Escolha dequeue(Queue *q) { // remove o primeiro, q->front->Pnext
    if(q->size == 0) { printf("Cuidado!!! fila vazia\n"); exit(-1); }
    Escolha it = (q->front)->next->escolha;
    
    Link *temp = (q->front)->next; // (q->front) eh o header Link
    (q->front)->next = (q->front)->next->next;
    free(temp);
    
    if((q->front)->next == NULL) q->rear = q->front; // empty queue
    q->size--;
    return it;
}

int queueEmpty(Queue *q) {
  return (q->size == 0);
}

void deleteQueue(Queue* q) { // libera memoria alocada para a lista e os Links
    while (q->size > 0) {
        Link *temp = q->front; // temp recece a front inicial/atual
        q->front = q->front->next; // front passa a apontar para o proximo
        free(temp);
    }
    free(q); // q eh um ponteiro que foi alocado com malloc
}

int saldoCliente[NUM_CLIENTES] = {};

// mutex para realizar que o uso da fila (recurso compartilhado) seja usado de maneira segura 
// e correta por diferentes threads
pthread_mutex_t mutex_queue = PTHREAD_MUTEX_INITIALIZER; 

// variavel de condicao para caso de fila vazia
pthread_cond_t  empty = PTHREAD_COND_INITIALIZER;  

void *bancoRotina(void* arg) {
    Queue *q = (Queue *) arg;  
    for (int i = 0; i < (NUM_CLIENTES*NUM_OPS); i++) {
        
        // Trava o mutex visto que a fila eh um recurso compártilhado
        pthread_mutex_lock(&mutex_queue);
        
        // em caso de fila vazia, o banco nao pode realzar operacoes entao dorme na variavel empty
        while (queueEmpty(q)) pthread_cond_wait(&empty, &mutex_queue); 
        
        Escolha pedido = dequeue(q);
        pthread_mutex_unlock(&mutex_queue);

        switch (pedido.op) {
            case 0: // consulta de saldo
            printf("Cliente %d - Consulta de Saldo: %d\n", pedido.idCliente, saldoCliente[pedido.idConta]);
            break;

            case 1: // deposito
            saldoCliente[pedido.idConta] += pedido.valorOP;
            printf("Cliente %d - Deposito de: %d\n", pedido.idCliente, pedido.valorOP);
            break;

            case 2: // saque
            saldoCliente[pedido.idConta] -= pedido.valorOP;
            printf("Cliente %d - Saque de: %d\n", pedido.idCliente, pedido.valorOP);
            break;
        }
    }
    pthread_exit(NULL);
}

void *clienteRotina(void *arg) {
    Thread_data data = *((Thread_data*)arg);
    Escolha escolha;
    escolha.idCliente = data.idCliente;
    escolha.idConta   = data.idConta;

    for (int i = 0; i < NUM_OPS; i++) {
        escolha.op = rand()%3;
        if(escolha.op) escolha.valorOP = rand()%1000; // (escolha == deposito || escolha == saque)

        pthread_mutex_lock(&mutex_queue);
        enqueue(data.q, escolha);
        
        // em caso da thread banco estar dormindo, envia um sinal para que ela possa voltar ao estado pronto
        pthread_cond_signal(&empty); 
        pthread_mutex_unlock(&mutex_queue);
    }
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));
    pthread_t threadBanco, cliente[NUM_CLIENTES];
    Queue *filaPedidos = createQueue();
    Thread_data data[NUM_CLIENTES];

    int rc = pthread_create(&threadBanco, NULL, bancoRotina, (void *)filaPedidos);
    if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }

    for (int i = 0; i < NUM_CLIENTES; i++) {
        data[i].idCliente = i;
        
        // faz com que diferentes clientes possam ter a mesma conta, uma conta em conjunto. Dessa forma, 
        // o cliente i pode ter ser dono da conta i, i-1, ... , 1 OU 0
        data[i].idConta   = (rand()%(i+1));
        data[i].q         = filaPedidos;

        rc = pthread_create(&cliente[i], NULL, clienteRotina, (void *)&data[i]);
        if(rc) { printf("error pthread_create: %d\n", rc); exit(-1); }
        printf("\t\t\t\t\t\tCliente %d usa conta %d\n", data[i].idCliente, data[i].idConta); 
    }

    // espera todos os clientes terminarem de realizar suas operacoes para prosseguir com a execucao 
    // da thread main
    for(int i = 0; i < NUM_CLIENTES; i++) pthread_join(cliente[i], NULL);

    // espera o banco termnar de processar as operacoes para prosseguir com a execucao da thread main
    pthread_join(threadBanco, NULL); 
    deleteQueue(filaPedidos);
    pthread_exit(NULL);
}