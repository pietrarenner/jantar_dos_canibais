#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int travessa_vazia = 0;
int porcoes = 0;
int max_porcoes = 0;

pthread_mutex_t mtx_travessa_vazia, mtx_porcoes;

void *canibal(void *arg) 
{
    int id = *((int *) arg);
    while(1)
    {
        pthread_mutex_lock(&mtx_porcoes);
        if(porcoes <= 0) 
        {
            // printf("porcoes 1: %d\n", porcoes);
            // pthread_mutex_lock(&mtx_travessa_vazia);
            
            printf("id: %d\n", id);
            // printf("porcoes: %d\n", porcoes);
            printf("acordando o cozinheiro\n");
            sleep(1);
            travessa_vazia = 1;

            // pthread_mutex_unlock(&mtx_travessa_vazia);
            pthread_mutex_unlock(&mtx_porcoes);
        } 
        else 
        {
            printf("o canibal %d está se servindo\n", id);
            porcoes--;
            sleep(1); 
            pthread_mutex_unlock(&mtx_porcoes);
            
            printf("o canibal %d está comendo\n", id);
            sleep(1);
        }
    }
}

void *cozinhando(void *arg)
{
    while(1)
    {
        // localização dos mutex está correta?
        pthread_mutex_lock(&mtx_travessa_vazia);
        if(travessa_vazia == 1)
        {
            printf("enchendo a travessa\n");
            sleep(1);

            porcoes = max_porcoes;
            travessa_vazia = 0;
            printf("travessa cheia\n");
        }
        pthread_mutex_unlock(&mtx_travessa_vazia);
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
    max_porcoes = M;
    pthread_t cozinheiro = 0; 

    // precisa usar mais de um mutex?
    pthread_mutex_init(&mtx_travessa_vazia, NULL);
    pthread_mutex_init(&mtx_porcoes, NULL);

    pthread_create(&cozinheiro, NULL, cozinhando, NULL);

    for(int i = 0; i < N; i++) 
    {
        int *id = (int *) malloc(sizeof(int));
        *id = i + 1;
        // tem que especificar algo no último argumento? por quê?
        pthread_create(&canibais[i], NULL, canibal, (void *) id);
    }

    for(int i = 0; i < N; i++) 
    {
        pthread_join(canibais[i], NULL);
    }
    pthread_join(cozinheiro, NULL);

    pthread_exit(NULL);
}