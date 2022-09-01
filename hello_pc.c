#include <stdio.h>
#include <pthread.h>

// função que a thread irá executar
void* tarefa(void* arg){
    printf("Ola, sou a thread nova!\n");
    pthread_exit(NULL);
}

// dunção principal
int main(){
    pthread_t tid; // identificador da thread no sistema

    if (pthread_create(&tid, NULL, tarefa, NULL)) // cria a thread nova
        printf("ERRO -- pthread_create\n");

    printf("Ola, sou a thread princial!\n");

    pthread_exit(NULL); // desvincula o término da main do término da thread

    return 0;
}