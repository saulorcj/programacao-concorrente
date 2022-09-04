#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

// função que a thread irá executar
void* tarefa(void* arg){
    int ident = *((int*) arg);

    vetor[ident] = ident;

    printf("Ola, sou a thread %d!\n", ident);
    pthread_exit(NULL);
}

// função principal
int main(){
    pthread_t tid[NTHREADS]; // identificador da thread no sistema

    int ident[NTHREADS]; // identificador local da thread

    // cria as threads novas
    for(int i = 0;i < NTHREADS; i++){
        ident[i] = i;
        if (pthread_create(&tid[i], NULL, tarefa, (void*) &ident[i]))
            printf("ERRO -- pthread_create %d\n", i);
    }

    // espera as threads terminarem
    for(int i = 0;i < NTHREADS; i++){
        if (pthread_join(tid[i], NULL))
            printf("ERRO -- pthread_create %d\n", i);
    }

    // imprimir o vetor de identificadores
    for(int i = 0;i < NTHREADS; i++){
        printf("%d ", vetor[i]);
    } printf("\n");

    printf("Ola, sou a thread principal!\n");

    // desvincula o término da main do término da thread
    pthread_exit(NULL);

    return 0;
}