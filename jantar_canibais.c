#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

// int travessa_vazia = 0;
// int porcoes = 0;
// int max_porcoes = 0;

pthread_mutex_t protege_porcoes;
sem_t cozinhando, enchendo, comida;
// pq tem que ser volatile?
volatile int porcoes = 0; 

void *chama_cozinheiro(void *arg)
{
    int id = *((int *) arg);
    printf("o canibal %d chamou o cozinheiro\n", id);
    // sleep(1);
}

void *come(void *arg)
{
    int id = *((int *) arg);
    printf("o canibal %d está comendo\n", id);
    sleep(2);
}

void *enche_travessa()
{
    printf("o cozinheiro está enchendo a travessa\n");
    // printf("a quantidade total de porções é %d\n", porcoes);
    sleep(2);
}

void *canibal(void *arg) 
{
    int id = *((int *) arg);
    // int id = id;
    while(1)
    {
        sem_wait(&comida);
        pthread_mutex_lock(&protege_porcoes);
        porcoes--;
        if(porcoes == 0) 
        {
            // chama_cozinheiro(&id);
            sem_wait(&cozinhando);
            sem_wait(&enchendo);
        }
        pthread_mutex_unlock(&protege_porcoes);
        come(&id);
        
        // pthread_mutex_lock(&mtx_porcoes);
        // if(porcoes <= 0) 
        // {
            // printf("porcoes 1: %d\n", porcoes);
            // pthread_mutex_lock(&mtx_travessa_vazia);
            
        //     printf("id: %d\n", id);
        //     // printf("porcoes: %d\n", porcoes);
        //     printf("acordando o cozinheiro\n");
        //     sleep(1);
        //     travessa_vazia = 1;

        //     // pthread_mutex_unlock(&mtx_travessa_vazia);
        //     pthread_mutex_unlock(&mtx_porcoes);
        // } 
        // else 
        // {
        //     printf("o canibal %d está se servindo\n", id);
        //     porcoes--;
        //     sleep(1); 
        //     pthread_mutex_unlock(&mtx_porcoes);
            
        //     printf("o canibal %d está comendo\n", id);
        //     sleep(1);
        // }
    }
}

void *cozinha(void *arg)
{
    // int M = M;
    int M = *((int *) arg);
    while(1)
    {
        // localização dos mutex está correta?
        // pthread_mutex_lock(&mtx_travessa_vazia);
        // if(travessa_vazia == 1)
        // {
        //     printf("enchendo a travessa\n");
        //     sleep(1);

        //     porcoes = max_porcoes;
        //     travessa_vazia = 0;
        //     printf("travessa cheia\n");
        // }
        // pthread_mutex_unlock(&mtx_travessa_vazia);

        sem_post(&cozinhando);
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

    pthread_t canibais[N];
    porcoes = M; 
    pthread_t cozinheiro = 0; 

    pthread_mutex_init(&protege_porcoes, NULL);

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