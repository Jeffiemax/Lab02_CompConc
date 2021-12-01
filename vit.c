#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

//Declaração das Variáveis globais
int **matriz1; //Matriz 1 de entrada
int **matriz2; // Matriz 2 de entrada
int **matrizMult; //Matriz de saida
int matrizaux;


typedef struct {
    int id;
    int dim;
}tArgs;


//Função que as threads executarao
void *tarefa(void *arg){
    tArgs *args = (tArgs*)arg;
    for(int j = 0; j < args-> dim; j++){
        for( int k = 0; k< args ->dim; k++){
            matrizaux = matrizaux + ((matriz1[args->id][k]) * (matriz2[k][j]));
        }
    matrizMult[args->id][j] = matrizaux;
    matrizaux = 0;
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
    printf("Digite: %s < dimensão da linha>\n", argv[0]);
    return 1;
}

dim = atoi(argv[1]);

//ALlocação de memória para as matrizes

//alocar a quantidade de linhas da matriz1
matriz1 = (int **)malloc(sizeof(int*)*dim*dim); 
//alocar a quantidade de colunas de cada linha
for(int i = 0; i< dim; i++){matriz1[i] = (int*)malloc(sizeof(int)*dim*dim);}
if(matriz1 == NULL){printf("ERRO -- malloc\n"); return 2;}

//alocar a quantidade de linhas da matriz2
matriz2 = (int **)malloc(sizeof(int*) * dim *dim );
//alocar a quantidade de colunas de cada linha
for(int i = 0; i< dim; i++){matriz2[i] = (int*)malloc(sizeof(int)*dim*dim);}
if(matriz2 == NULL){printf("ERRO -- malloc\n"); return 2;}

//alocar a quantidade de linhas da matriz multiplicação
matrizMult = (int **)malloc(sizeof(int*) * dim *dim );
//alocar a quantidade de colunas de cada linha
for(int i = 0; i< dim; i++){matrizMult[i] = (int*)malloc(sizeof(int)*dim*dim);}
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
tid = (pthread_t*)malloc(sizeof(pthread_t)* dim);
if(tid == NULL){puts("ERRO -- malloc"); return 2;}
args = (tArgs*)malloc(sizeof(tArgs)* dim);
if(args == NULL){puts("ERRO -- malloc"); return 2;}

GET_TIME(inicio);
//Criação das theads
for(int i = 0; i < dim; i++ ){
    (args+i) -> id = i;
    (args+i) -> dim = dim;
    if(pthread_create(tid+i, NULL, tarefa, (void*)(args+i))){
    puts("ERRO -- pthread_create\n"); return 3;
    }  
}


//espera pelo termino das threads
for(int i = 0; i< dim; i++){
    pthread_join(*(tid+i),NULL);
}
GET_TIME(fim);
delta = fim - inicio;
printf("Tempo multiplicação: %lf\n", delta);
//Exibição do resultado
/* printf("Matriz de Multiplicação \n");
for(int i = 0; i < dim; i++ ){
    printf("| ");
    for(int j = 0; j < dim; j++){
        printf("%d |", matrizMult[i][j]);
    }
    printf("\n");
}
printf("\n");  */

//Liberação da memória
free(matriz1);
free(matriz2);
free(matrizMult); 
free(args);
free(tid);
return 0;

}