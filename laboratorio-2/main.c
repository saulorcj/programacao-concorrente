#include<stdio.h>
#include<stdlib.h>
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
    
    /*
    //imprime a matriz na saida padrao
    for(int i=0; i<linhas; i++) { 
       for(int j=0; j<colunas; j++)
         fprintf(stdout, "%.6f ", matriz[i*colunas + j]);
       fprintf(stdout, "\n");
    }
    */
 
    //finaliza o uso das variaveis
    fclose(descritorArquivo);
    return matriz;
}

int main(int argc, char *argv[]){

    if (argc < 3) {
        fprintf(stderr, "Digite: %s <arquivo matriz 1> <arquivo matriz 2>\n", argv[0]);
        return 1;
    }

    int i, j, k;
    char erro[100];
    char *arquivoA = argv[1];
    char *arquivoB = argv[2];
    float *matriz_A, *matriz_B;
    double start, finish, elapsed;

    // começando o timer
    GET_TIME(start);

    int linhas_A, colunas_A;
    printf("Matriz A\n");
    // lendo a matriz A
    matriz_A = leMatrizBinario(arquivoA, erro, &linhas_A, &colunas_A);

    // verificando erro
    if (matriz_A == NULL){
        fprintf(stderr, "Ocorreu um erro!\n");
        return 0;
    }
    
    int linhas_B, colunas_B;
    printf("\nMatriz B\n");
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

    // gerando novas cimensões
    int linhas_C = linhas_A;
    int colunas_C = colunas_B;
    int tam_C = linhas_C * colunas_C;

    // alocando memória par a matriz resultado
    float *matriz_C = (float*) malloc(sizeof(float) * tam_C);

    // terminando o timer
    GET_TIME(finish);

    // armazenando intervalo
    elapsed = finish - start;

    printf("\nInicialização: %e segundos!\n", elapsed);

    printf("\nMatriz Resultante\n")    ;
    // começando o timer
    GET_TIME(start)

    float soma;
    for (i = 0; i < linhas_C; i++){
        for (j = 0; j < colunas_C; j++){
            soma = 0;
            //printf("%d, %d, %d\n", i, j, i * colunas_C + j);
            for (k = 0; k < colunas_A; k++){
                //printf("A - %d, B - %d\n", i * colunas_A + k, k * colunas_B + j);
                soma += matriz_A[i * colunas_A + k] * matriz_B[k * colunas_B + j];
            }
            
            matriz_C[i * colunas_C + j] = soma;
            //printf("%f ", soma);
        } //printf("\n");
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

    free(matriz_A);
    free(matriz_B);
    free(matriz_C);
    return 0;
}