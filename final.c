#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

//Declaração das Variáveis globais
float **matriz1; //Matriz 1 de entrada
float **matriz2; // Matriz 2 de entrada
float **matrizMult; //Matriz de saida
float matrizaux = 0;
int nthreads;

typedef struct {
    int id;
    int dim;
}tArgs;


//Função que as threads executarao

void *tarefaConcorrente(void *arg){
    tArgs *args = (tArgs*)arg;
    //printf("thread %d ", args-> id);
    for(int i = args->id; i< args->dim; i+= nthreads){
        for(int j = 0; j < args->dim; j++){
            for( int k = 0; k< args->dim; k++){
                matrizaux = matrizaux + ((matriz1[i][k]) * (matriz2[k][j]));
            }
        matrizMult[i][j] = matrizaux;
        matrizaux = 0;
        }
    }
    pthread_exit(NULL);
}



int main(int argc, char* argv[]){
    int dim; //dimensões da matriz de entrada
    pthread_t *tid;//identificadores do sistema
    tArgs *args;//Identificadores locais das threads e dimensão
    double inicio,fim, delta;

    GET_TIME(inicio);
    //leitura e avaliação dos parametros de entrada
    if(argc<2){
        printf("Digite: %s < dimensão da linha>, <numero de theads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if(nthreads > dim) nthreads = dim;

    //ALlocação de memória para as matrizes

    //alocar a quantidade de linhas da matriz1
    matriz1 = (float **)malloc(sizeof(float*)*dim*dim); 
    //alocar a quantidade de colunas de cada linha
    for(int i = 0; i< dim; i++){matriz1[i] = (float*)malloc(sizeof(float)*dim*dim);}
    if(matriz1 == NULL){printf("ERRO -- malloc\n"); return 2;}

    //alocar a quantidade de linhas da matriz2
    matriz2 = (float **)malloc(sizeof(float*) * dim *dim );
    //alocar a quantidade de colunas de cada linha
    for(int i = 0; i< dim; i++){matriz2[i] = (float*)malloc(sizeof(float)*dim*dim);}
    if(matriz2 == NULL){printf("ERRO -- malloc\n"); return 2;}

    //alocar a quantidade de linhas da matriz multiplicação
    matrizMult = (float **)malloc(sizeof(float*) * dim *dim );
    //alocar a quantidade de colunas de cada linha
    for(int i = 0; i< dim; i++){matrizMult[i] = (float*)malloc(sizeof(float)*dim*dim);}
    if(matrizMult == NULL){printf("ERRO -- malloc\n"); return 2;}   

    //Inicialização das estruturas
    for(int i = 0; i< dim; i++){
        for(int j = 0; j< dim; j++){
            matriz1[i][j] = 1;
        }
    }
    for(int i = 0; i< dim; i++){
        for(int j = 0; j < dim; j++){
            matriz2[i][j] = 2;
        }
    }

    //Criação das matrizes (Print) 

    /* printf("------- Matriz 1 -------");
    printf("\n");
    for(int i = 0; i< dim; i++){
        printf("|");
        for(int j = 0; j < dim; j++){
            printf("%d |", matriz1[i][j]);
        }
        printf("\n");
    }
        
    printf("\n");
        printf("------- Matriz 2 -------");
    printf("\n");
    for(int i = 0; i< dim; i++){
        printf("|");
        for(int j = 0; j < dim; j++){
            printf("%d |", matriz2[i][j]);
        }
        printf("\n");
    } */

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicialização: %lf\n", delta);

    //Multipilcação das matrizes

    
    //Alocação das estruturas
    tid = (pthread_t*)malloc(sizeof(pthread_t)* nthreads);
    if(tid == NULL){puts("ERRO -- malloc"); return 2;}
    args = (tArgs*)malloc(sizeof(tArgs)* nthreads);
    if(args == NULL){puts("ERRO -- malloc"); return 2;}

    GET_TIME(inicio);
    //Criação das theads
    for(int i = 0; i < nthreads; i++ ){
        (args+i) -> id = i;
        (args+i) -> dim = dim;
        if(pthread_create(tid+i, NULL, tarefaConcorrente, (void*)(args+i))){
        puts("ERRO -- pthread_create\n"); return 3;
        }  
    }

    //espera pelo termino das threads
    for(int i = 0; i< nthreads; i++){
        pthread_join(*(tid+i),NULL);

    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo multiplicação: %lf\n", delta);
    //Exibição do resultado
   /*   printf("Matriz de Multiplicação \n");
    for(int i = 0; i < dim; i++ ){
        printf("| ");
        for(int j = 0; j < dim; j++){
            printf("%lf |", matrizMult[i][j]);
        }
        printf("\n");
    }
    printf("\n");  */

    /* GET_TIME(inicio); */
    //Liberação da memória
    free(matriz1);
    free(matriz2);
    free(matrizMult); 
    free(args);
    free(tid);
   /*  GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo finalização: %lf\n", delta); */

    return 0;

}