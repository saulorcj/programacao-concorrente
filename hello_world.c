#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10

// função que a thread irá executar
void* tarefa(void* arg){
    int ident = *((int*) arg);

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

    printf("Ola, sou a thread principal!\n");

    pthread_exit(NULL); // desvincula o término da main do término da thread

    return 0;
}