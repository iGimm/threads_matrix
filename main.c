//
//  main.c
//  simple_matrix_multiply
//
//  Created by Gerardo López García on 21/04/15.
//  Copyright (c) 2015 __G.Logar___. All rights reserved.
//

//VERSION 4

//////////// LIBRERIAS /////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//<--------------------------------------------------------------------------------------------------- DEFINIR MATRICES MANUALMENTE (CTEs)
#define filasA 3
#define columnasA 3
#define filasB 3
#define columnasB 3

#define numThreads filasA

static pthread_mutex_t lock;
static int auxAf, auxBc, auxAB, sumCelda = 0;
static int first[filasA][columnasA], second[filasB][columnasB], multiply[filasA][columnasB];//<------- ARREGLOS DYNAMICOS
void inputMatrixABC();
void printMatrixC();
void *mapear(void *);
int idDelElegido;

///(TRHREAD_PADRE) /////////////////////////////////
int main(int argc, const char * argv[]) {
    int id_T[filasA];
    pthread_t *hilos;
    hilos = (pthread_t *)malloc(sizeof(pthread_t)*(numThreads));
    pthread_mutex_init(&lock, NULL);
    
    inputMatrixABC();
    printMatrixC(); //<------------------------------------------------------------------------------- DIAGONAL -1
    
    srand(time(NULL));  //<--------------------------------------------------------------------------- WARNING
    idDelElegido = rand()%filasA;
    
    for (auxAf = 0; auxAf < filasA; auxAf++) {
        
        id_T[auxAf] = auxAf;
        pthread_create(hilos+auxAf, NULL, mapear, id_T+auxAf);
    }
    
    //JOINs
    for (auxAf = 0; auxAf < filasA; auxAf++)
        pthread_join(hilos[auxAf], NULL);
    
    printMatrixC();
    return 0;
}

void * mapear(void *arg){
    printf("Estoy en el thread con el id: %d \n", *((int*)arg)    );
    
    for (auxBc = 0; auxBc < columnasB; auxBc++) {
        for (auxAB = 0; auxAB < filasB; auxAB++) {
            sumCelda = sumCelda + first[*((int*)arg)][auxAB]*second[auxAB][auxBc];
        }
        multiply[*((int*)arg)][auxBc] = sumCelda;
        sumCelda = 0;
    }
    
    if(*((int*)arg)==idDelElegido)
    printf("Se elegio el numero random %d y yo soy el thread Elegido con id %d\n", idDelElegido, *((int*)arg));
                                  //<-------------------------------------------- AQUI MANDARIA LLAMAR LA FUNCION DIAGONAL (MODIFICA VAR GLOBAL O RETURN INT)
    
    return NULL;
}

void inputMatrixABC(){
    
    //<----------------------------------------------------------------------------------------------- Inicializar diagonal en -1
    for (int x = 0; x < filasA; x++) {
        for (int y = 0; y < columnasB; y++)
            if(x==y)
            multiply[x][y]=-1;
    }
    
    /*
     //INPUT MATRIXs CTE
     first[0][0]=0;
     first[0][1]=2;
     first[1][0]=1;
     first[1][1]=3;
     
     second[0][0]=2;
     second[0][1]=3;
     second[0][2]=1;
     second[1][0]=3;
     second[1][1]=0;
     second[1][2]=2;
     */
    
    //<----------------------------------------------------------------------------------------------- Ingresar valores deseados. Cambiar DEFINES.
    printf("Enter the elements of first matrix\n");
    for (int x = 0; x < filasA; x++)
        for (int y = 0; y < columnasA; y++)
            scanf("%d", &first[x][y]);
    
    if (columnasA != filasB)
        printf("Matrices with entered orders can't be multiplied with each other.\n");
    else
    {
        printf("Enter the elements of second matrix\n");
        
        for (int x = 0; x < filasB; x++)
            for (int y = 0; y < columnasB; y++)
                scanf("%d", &second[x][y]);
    }
    
}

void printMatrixC(){
    for (int x = 0; x < filasA; x++) {
        for (int y = 0; y < columnasB; y++)
            printf("%d\t", multiply[x][y]);
        printf("\n");
    }
}


/*
//printf("Enter the number of rows and columns of first matrix\n");
//scanf("%d%d", &filasA, &columnasA);
printf("Enter the elements of first matrix\n");
for (int x = 0; x < filasA; x++)
for (int y = 0; y < columnasA; y++)
scanf("%d", &first[x][y]);

//printf("Enter the number of rows and columns of second matrix\n");
//scanf("%d%d", &filasB, &columnasB);
if (columnasA != filasB)
printf("Matrices with entered orders can't be multiplied with each other.\n");
else
{
    printf("Enter the elements of second matrix\n");
    
    for (int x = 0; x < filasB; x++)
        for (int y = 0; y < columnasB; y++)
            scanf("%d", &second[x][y]);
            }

*/
