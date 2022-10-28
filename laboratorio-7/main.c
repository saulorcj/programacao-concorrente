#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS_PROD 4
#define NTHREADS_CONS 2
#define TAM_BASE 100
#define TAM_BUFFER 10

sem_t sem_prod;
sem_t sem_cons;
sem_t sem_buffer1;
sem_t sem_buffer2;
sem_t sem_espera;

int base_dados[TAM_BASE];
int buffer[TAM_BUFFER];

int i_buffer = 0;

void Retira(int id){
    // espera buffer ficar cheio
    sem_wait(&sem_buffer1);
    // elemento a ser retirado
    int elemento;

    // percorre o buffer
    for(int i = id; i < TAM_BUFFER; i+=NTHREADS_CONS){
        sem_wait(&sem_cons);
        elemento = buffer[i];
        i_buffer--;
        printf("Consumidor[%d] retira %d\n", id, elemento);
        sem_post(&sem_cons);
    }
    // se buffer tiver vazio
    if(i_buffer == -1){
        i_buffer = 0;
        // permite que o produtor continue a operar
        printf("Buffer Vazio!\n");
        sem_post(&sem_buffer2);
    }
    // barreira
    sem_wait(&sem_espera);
}

void Insere(int id, int i_base){
    sem_wait(&sem_prod);
    // retira elemento da base e bota no buffer
    buffer[i_buffer] = base_dados[i_base];
    i_buffer++;
    printf("Produtor[%d] insere %d\n", id, i_base);

    // se buffer tiver cheio
    if(i_buffer == TAM_BUFFER){
        i_buffer = TAM_BUFFER - 1;
        printf("Buffer Cheio!\n");

        // permite que as threads consumidoras comecem a operar
        for(int i = 0; i < NTHREADS_CONS; i++)
            sem_post(&sem_buffer1);
        // espera as threads consumidoras esvaziarem o buffer
        sem_wait(&sem_buffer2);
        // permite que as threads consumidores avancem a barreira
        for(int i = 0; i < NTHREADS_CONS; i++)
            sem_post(&sem_espera);
    }
    sem_post(&sem_prod);
}

void *Produtor(void *t){
    int *id = (int*) t;
    printf("Produtor[%d] começando\n", *id);

    for(int i = *id; i < TAM_BASE; i+=NTHREADS_PROD){
        Insere(*id, i);
    }
    pthread_exit(NULL);
}

void *Consumidor(void *t){
    int *id = (int*) t;
    *id -= NTHREADS_PROD;
    printf("Consumidor[%d] começando\n", *id);
    int elemento;
    // quantidade de iterações necessárias para esvaziar todos os buffers
    int quant_iter = TAM_BASE / TAM_BUFFER;

    for(int k = 0; k < quant_iter; k++){
        Retira(*id);
    }
    pthread_exit(NULL);
}

int main(){
    // total de threads
    int nthreads_total = NTHREADS_PROD + NTHREADS_CONS;
    // threads
    pthread_t tid[nthreads_total];
    // id das threads
    int *id[nthreads_total];

    // alocando espaço
    for (int t = 0; t < nthreads_total; t++) {
        if ((id[t] = malloc(sizeof(int))) == NULL) {
           pthread_exit(NULL);
           return 1;
        }
        *id[t] = t;
    }

    // iniciando os semáforos
    sem_init(&sem_prod, 0, 1); // semáforo para produtores
    sem_init(&sem_cons, 0, 1); // semáforo para consumidores
    sem_init(&sem_buffer1, 0, 0); // semáforo para bloquear consumidores
    sem_init(&sem_buffer2, 0, 0); // semáforo para bloquar produtores
    sem_init(&sem_espera, 0, 0); // semáforos para método de barreira nos consumidores

    // preenchendo a base de dados
    for(int i = 0; i < TAM_BASE; i++)
        base_dados[i] = i;
    
    // criando as threads produtoras
    for(int i = 0; i < NTHREADS_PROD; i++){
        if (pthread_create(&tid[i], NULL, Produtor, (void *)id[i])){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    // criando as threads consumidoras
    for(int i = 0; i < NTHREADS_CONS; i++){
        if (pthread_create(&tid[i + NTHREADS_PROD], NULL, Consumidor, (void *)id[i + NTHREADS_PROD])){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    // esprando as threads terminarem
    for(int i = 0; i < nthreads_total; i++){
        if (pthread_join(tid[i], NULL)){
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    pthread_exit(NULL);
    return 0;
}