/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so pode imprimir "Bye" depois que duas threads A imprimirem  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond_1;
pthread_cond_t x_cond_3;

void *thread_1(void *t){
    pthread_mutex_lock(&x_mutex);
    while (x < 3)
        pthread_cond_wait(&x_cond_3, &x_mutex);

    printf("Volte sempre!\n");

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *thread_2(void *t){
    pthread_mutex_lock(&x_mutex);
    while (x < 1)
        pthread_cond_wait(&x_cond_1, &x_mutex);
    
    printf("Fique a vontade.\n");

    x++;
    if (x == 3)
        pthread_cond_signal(&x_cond_3);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);

}

void *thread_3(void *t){
    pthread_mutex_lock(&x_mutex);
    while (x < 1)
        pthread_cond_wait(&x_cond_1, &x_mutex);
    
    printf("Sente-se por favor.\n");

    x++;
    if (x == 3)
        pthread_cond_signal(&x_cond_3);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

void *thread_4(void *t){
    pthread_mutex_lock(&x_mutex);

    printf("Seja bem-vindo!\n");

    x++;
    pthread_cond_broadcast(&x_cond_1);

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond_1, NULL);
  pthread_cond_init (&x_cond_3, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, thread_1, NULL);
  pthread_create(&threads[1], NULL, thread_2, NULL);
  pthread_create(&threads[2], NULL, thread_3, NULL);
  pthread_create(&threads[3], NULL, thread_4, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond_1);
  pthread_cond_destroy(&x_cond_3);
}
