#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define X 1 //quantidade de threads escritora
#define Y 2 //quantidade de threads leitora
#define Z 2 //quantidade de threads leitora e escritora

//variáveis do problema
int var = 0; //variável a ser modificada e lida pelas threads
int leit = 0; //quantidade de threads lendo
int escr = 0; //quantidade de threads escrevendo

//variáveis para sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit(int id, char *nome){
    pthread_mutex_lock(&mutex);
    printf("%s[%d] quer ler\n", nome, id);
    while(escr > 0){
        printf("%s[%d] bloqueou\n", nome, id);
        pthread_cond_wait(&cond_leit, &mutex);
        printf("%s[%d] desbloqueou\n", nome, id);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit(int id, char *nome){
    pthread_mutex_lock(&mutex);
    printf("%s[%d] terminou de ler\n", nome, id);
    leit--;
    if(leit == 0)
        pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr(int id, char *nome){
    pthread_mutex_lock(&mutex);
    printf("%s[%d] quer escrever\n", nome, id);
    while((leit > 0) || (escr > 0)) {
        printf("%s[%d] bloqueou\n", nome, id);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("%s[%d] desbloqueou\n", nome, id);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr(int id, char *nome){
    pthread_mutex_lock(&mutex);
    printf("%s[%d] terminou de escrever\n", nome, id);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

//thread1
void *Escritor(void *t){
    int *id = (int*) t;
    char nome[15] = "Escritor";

    while(1){
        InicEscr(*id, nome);
        printf("Escritor[%d] está escrevendo\n", *id);
        var++;
        FimEscr(*id, nome);
        sleep(1);
    }
    free(t);
    pthread_exit(NULL);
}

//thread2
void *Leitor(void *t){
    int *id = (int*) t;
    char nome[15] = "Leitor";

    while(1){
        InicLeit(*id, nome);
        printf("Leitor[%d] está lendo\n", *id);
        if (var % 2 == 0)
            printf("Leitor[%d], paridade = par\n", *id);
        else
            printf("Leitor[%d], paridade = impar\n", *id);
        FimLeit(*id, nome);
        sleep(1);
    }
    free(t);
    pthread_exit(NULL);
}

//thread3
void *LeitorEscritor(void *t){
    int *id = (int*) t;
    char nome[15] = "LeitorEscritor";

    while(1){
        InicLeit(*id, nome);
        printf("LeitorEscritor[%d] está lendo\n", *id);
        printf("LeitorEscritor[%d], var = %d\n", *id, var);
        FimLeit(*id, nome);
        for(int i = 0; i < 10000; i++);
        InicEscr(*id, nome);
        printf("LeitorEscritor[%d] está escrevendo\n", *id);
        var = *id;
        FimEscr(*id, nome);
        sleep(1);
    }
    free(t);
    pthread_exit(NULL);
}

int main(){
    //identificadores das threads
    pthread_t tid[X + Y + Z];
    int id[X + Y + Z];

    //incializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    //cria as threads Escritoras
    for(int i = 0; i < X; i++){
        id[i] = i + 1;
        if (pthread_create(&tid[i], NULL, Escritor, (void *) &id[i])) exit(-1);
    }

    //cria as threads Leitoras
    for(int i = 0; i < Y; i++){
        id[i + X] = i + 1;
        if (pthread_create(&tid[i + X], NULL, Leitor, (void *) &id[i + X])) exit(-1);
    }

    //cria as threads LeitorasEscritoras
    for(int i = 0; i < Z; i++){
        id[i + X + Y] = i + 1;
        if (pthread_create(&tid[i + X + Y], NULL, LeitorEscritor, (void *) &id[i + X + Y])) exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}