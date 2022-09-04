#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

#define NTHREADS 2
#define VETOR_SIZE 10000

/*
inicio: posição de inicío da manipulação do vetor
fim: ultima posição do vetor, que não será manipulado
vetor_numeros: posição na memória onde vetor está alocado
*/
typedef struct {
    int inicio, fim;
    float *vetor_numeros;
} t_Args;

void *Aumentar10(void *arg) {
    t_Args *args = (t_Args*) arg;

    int inicio = args->inicio;
    int fim = args->fim;
    float *vetor = args->vetor_numeros;

    for (int i = inicio; i < fim; i++)
        *(vetor + i) = *(vetor + i) * 1.1;
}

int main() {
    int i; //variável auxiliar
    float vetor_numeros[VETOR_SIZE]; //vetor de 10_000 números
    int intervalo = VETOR_SIZE / NTHREADS;

    pthread_t tid_sistema[NTHREADS]; // identificadores das threads no sistema
    t_Args *arg;

    // preenchendo o vetor
    for (i = 0; i < VETOR_SIZE; i++)
        vetor_numeros[i] = i + 1;

    for (i = 0; i < NTHREADS; i++){
        arg = malloc(sizeof(t_Args));
        
        if (arg == NULL){
            printf("--ERR: malloc()\n");
            exit(-1);
        }

        // atribuindo o intervalo e posição do vetor
        arg->inicio = intervalo * i;
        arg->fim = intervalo * (i + 1);
        arg->vetor_numeros = vetor_numeros;

        if (pthread_create(&tid_sistema[i], NULL, Aumentar10, (void*) arg)){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    // espera as threads terminarem para poder verificar o resultado
    for (i = 0; i < NTHREADS; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1); 
        }
    }

    // verifica o resultado
    for (i = 0; i < VETOR_SIZE; i++){
        // numero esperado
        float numero_certo = ((float) (i + 1)) * 1.1;
        
        // verifica se os dois floats são iguais
        if(fabs(numero_certo - vetor_numeros[i]) >= (1.0 / 10000000))
            printf("--ERRO: posicao %d incorreta!", i);
    }

    // fim
    pthread_exit(NULL);
    return 1;
}