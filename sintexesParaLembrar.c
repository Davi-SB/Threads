// create
pthread_thread_t myThread;
int rc = pthread_create(&myThread, NULL, rotina, (void *) &argumento);
if(rc) exit(1);

// join
pthread_join(myThread, (void **) &retorno);

// mutex
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&myMutex);
pthread_mutex_unlock(&myMutex);

// condicional
pthread_cond_t varCond = PTHREAD_COND_INITIALIZER;
pthread_cond_wait(&varCond, &myMutex);
pthread_cond_signal(&varCond);
pthread_cond_broadcast(&varCond);

// barreira
pthread_barrier_t myBarrier; // inicializada apenas dinamicamente (main)
/* na main: */ pthread_barrier_init(&myBarrier, NULL, (int) count);
pthread_barrier_wait(&myBarrier);
/* na main: */ pthread_barrier_destroy(&myBarrier);


