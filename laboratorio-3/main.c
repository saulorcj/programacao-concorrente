#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif

int geraMatrizBinario(char *arquivo, float* matriz, int linhas, int colunas) {
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   tam = linhas * colunas;

   //escreve a matriz no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(arquivo, "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
   ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   return 0;
}

float *leMatrizBinario(char *arquivo, char *erro, int *linhas_p, int *colunas_p){
    float *matriz; //matriz que será carregada do arquivo
    int linhas, colunas; //dimensoes da matriz
    long long int tam; //qtde de elementos na matriz
    FILE * descritorArquivo; //descritor do arquivo de 
    size_t ret; //retorno da funcao de leitura no arquivo de entrada

    //abre o arquivo para leitura binaria
    descritorArquivo = fopen(arquivo, "rb");
    if(!descritorArquivo) {
       //fprintf(stderr, "Erro de abertura do arquivo\n");
       erro = "Erro de abertura do arquivo\n";
       return NULL;
    }
    
    //le as dimensoes da matriz
    ret = fread(&linhas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
       //fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
       erro = "Erro de leitura das dimensoes da matriz arquivo \n";
       return NULL;
    }
    ret = fread(&colunas, sizeof(int), 1, descritorArquivo);
    if(!ret) {
       //fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
       erro = "Erro de leitura das dimensoes da matriz arquivo \n";
       return NULL;
    }
    tam = linhas * colunas; //calcula a qtde de elementos da matriz
    
    *linhas_p = linhas;
    *colunas_p = colunas;

    //aloca memoria para a matriz
    matriz = (float*) malloc(sizeof(float) * tam);
    if(!matriz) {
       //fprintf(stderr, "Erro de alocao da memoria da matriz\n");
       erro = "Erro de alocao da memoria da matriz\n";
       return NULL;
    }
    
    //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matriz, sizeof(float), tam, descritorArquivo);
    if(ret < tam) {
       //fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
       erro = "Erro de leitura dos elementos da matriz\n";
       return NULL;
    }
 
    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return matriz;
}

typedef struct {
    int linha_inicio, linha_fim, colunas_A, colunas_B, colunas_C;
    float *matriz_A, *matriz_B, *matriz_C;
} t_Args;

void *tarefa(void* arg) {
    t_Args *args = (t_Args*) arg;

    int linha_inicio = args->linha_inicio;
    int linha_fim = args->linha_fim;
    int colunas_A = args->colunas_A;
    int colunas_B = args->colunas_B;
    int colunas_C = args->colunas_C;
    int i, j, k;
    float *matriz_A = args->matriz_A;
    float *matriz_B = args->matriz_B;
    float *matriz_C = args->matriz_C;

    float soma;
    for(i = linha_inicio; i < linha_fim; i++){
        for(j = 0; j < colunas_C; j++){
            soma = 0;

            for(k = 0; k < colunas_A; k++)
                soma += matriz_A[i * colunas_A + k] * matriz_B[k * colunas_B + j];
            
            matriz_C[i * colunas_C + j] = soma;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    if (argc < 4) {
        fprintf(stderr, "Digite: %s <arquivo matriz 1> <arquivo matriz 2> <numero threads>\n", argv[0]);
        return 1;
    }

    int i, j;
    char erro[100];
    char *arquivoA = argv[1];
    char *arquivoB = argv[2];
    int nthreads = atoi(argv[3]);
    float *matriz_A, *matriz_B;
    double start, finish, elapsed;

    pthread_t tid_sistema[nthreads];
    t_Args *args;

    // começando o timer
    GET_TIME(start);

    int linhas_A, colunas_A;
    // lendo a matriz A
    matriz_A = leMatrizBinario(arquivoA, erro, &linhas_A, &colunas_A);

    // verificando erro
    if (matriz_A == NULL){
        fprintf(stderr, "Ocorreu um erro!\n");
        return 0;
    }
    
    int linhas_B, colunas_B;
    // lendo a matriz B
    matriz_B = leMatrizBinario(arquivoB, erro, &linhas_B, &colunas_B);

    // verificando erro
    if (matriz_B == NULL){
        fprintf(stderr, "Ocorreu um erro!\n");
        return 0;
    }

    // verificando erro
    if (colunas_A != linhas_B){
        fprintf(stderr, "Matriz A com nº de colunas diferente do nº de linhas da Matriz B!\n");
        return 0;
    }

    // gerando novas dimensões
    int linhas_C = linhas_A;
    int colunas_C = colunas_B;
    int tam_C = linhas_C * colunas_C;
    int intervalo_linha = linhas_C / (nthreads - 1);
    int intervalo_linha_resto = linhas_C % (nthreads - 1);

    // alocando memória para as entradas da função
    args = malloc(sizeof(t_Args) * nthreads);

    // alocando memória para a matriz resultado
    float *matriz_C = (float*) malloc(sizeof(float) * tam_C);

    // terminando o timer
    GET_TIME(finish);

    // armazenando intervalo
    elapsed = finish - start;

    printf("\nInicialização: %e segundos!\n", elapsed);

    // começando o timer
    GET_TIME(start)

    for (i = 0, j = 0; j < nthreads - 1; i += intervalo_linha, j++){
        (args + j)->colunas_A = colunas_A;
        (args + j)->colunas_B = colunas_B;
        (args + j)->colunas_C = colunas_C;
        (args + j)->matriz_A = matriz_A;
        (args + j)->matriz_B = matriz_B;
        (args + j)->matriz_C = matriz_C;
        (args + j)->linha_inicio = i;
        (args + j)->linha_fim = i + intervalo_linha;

        if (pthread_create(&tid_sistema[j], NULL, tarefa, (void*) (args + j))){
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }
    (args + j)->colunas_A = colunas_A;
    (args + j)->colunas_B = colunas_B;
    (args + j)->colunas_C = colunas_C;
    (args + j)->matriz_A = matriz_A;
    (args + j)->matriz_B = matriz_B;
    (args + j)->matriz_C = matriz_C;
    (args + j)->linha_inicio = i;
    (args + j)->linha_fim = i + intervalo_linha_resto;

    if (pthread_create(&tid_sistema[j], NULL, tarefa, (void*) (args + j))){
        printf("--ERRO: pthread_create()\n");
            exit(-1);
    }

    // terminando o timer
    GET_TIME(finish)
    elapsed = finish - start;
    printf("\nProcessamento: %e segundos!\n", elapsed);
    
    // começando o timer
    GET_TIME(start);

    geraMatrizBinario("C", matriz_C, linhas_C, colunas_C);

    // terminando o timer
    GET_TIME(finish);
    elapsed = finish - start;
    printf("\nFinalização: %e segundos!\n", elapsed);

    // liberando a memória
    free(matriz_A);
    free(matriz_B);
    free(matriz_C);
    return 0;
}
