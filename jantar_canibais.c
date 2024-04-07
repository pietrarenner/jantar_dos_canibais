#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "lamport.h"

sem_t cozinhando, enchendo, comida;
// pthread_mutex_t protege_porcoes;

volatile int porcoes = 0; 

void *chama_cozinheiro(void *arg)
{
    int id = *((int *) arg);
    printf("o canibal %d chamou o cozinheiro\n", id);
}

void *come(void *arg)
{
    int id = *((int *) arg);
    printf("o canibal %d está comendo\n", id);
    sleep(1);
}

void *enche_travessa()
{
    printf("o cozinheiro está enchendo a travessa\n");
    sleep(1);
}

void *canibal(void *arg) 
{
    int id = *((int *) arg);
    while(1)
    {
        sem_wait(&comida);
        // pthread_mutex_lock(&protege_porcoes);
        lock(id);
        porcoes--;
        come(&id);
        if(porcoes == 0) 
        {
            sem_post(&cozinhando);
            sem_wait(&enchendo);
        }
        // pthread_mutex_unlock(&protege_porcoes);
        unlock(id);
    }
}

void *cozinha(void *arg)
{
    int M = *((int *) arg);
    while(1)
    {
        sem_wait(&cozinhando);
        enche_travessa();
        for(int i = 0; i < M; i++)
        {
            sem_post(&comida);
        }
        porcoes = M;
        sem_post(&enchendo);
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) {
        printf("para rodar o código use: %s <N_canibais> <M_porcoes>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]); // Número de canibais
    int M = atoi(argv[2]); // Capacidade máxima da travessa

    sem_init(&cozinhando, 0, 0);
    sem_init(&enchendo, 0, 0); 
    sem_init(&comida, 0, M); 

    pthread_t canibais[N];
    pthread_t cozinheiro = 0; 
    porcoes = M; 

    // pthread_mutex_init(&protege_porcoes, NULL);
    initialize_mutex(N+1);

    int *novo_m = (int *) malloc(sizeof(int));
    *novo_m = M; 
    pthread_create(&cozinheiro, NULL, cozinha, (void *) novo_m);

    for(int i = 0; i < N; i++) 
    {
        int *id = (int *) malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&canibais[i], NULL, canibal, (void *) id);
    }

    for(int i = 0; i < N; i++) 
    {
        pthread_join(canibais[i], NULL);
    }
    pthread_join(cozinheiro, NULL);

    pthread_exit(NULL);
}