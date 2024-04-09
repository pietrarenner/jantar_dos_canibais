
#include <stdio.h>
#include <stdbool.h>
#include "lamport.h"
#include <stdlib.h>

// volatile bool *entering;
// volatile int *number;
int qtd_threads;

// struct LamportMutex
// {
//     volatile bool *entering;
//     volatile int *number;
// };

int max(volatile int *number)
{
    int r, x;

    r = number[0];
    for (x = 0; x < qtd_threads; x++) {
        if (number[x] >= r)
            r = number[x];
    }
    return(r);
}

void initialize_mutex(LamportMutex *mutex, int qtd)
// void initialize_mutex(Mutex *mutex)
{
    qtd_threads = qtd;
    mutex->entering = (bool *)malloc(qtd_threads * sizeof(bool));
    mutex->number = (int *)malloc(qtd_threads * sizeof(int));
    // entering = (bool *)malloc(qtd_threads * sizeof(bool));
    // number = (int *)malloc(qtd_threads * sizeof(int));
    
    // printf("entering: ");
    // for (int i = 0; i < qtd_threads; i++)
	// {
    //     mutex->entering[i] = false; 
	// 	printf("%d ", mutex->entering[i]);
	// }
    // printf("\n");

    // printf("number: ");
    // for (int i = 0; i < qtd_threads; i++)
	// {
    //     mutex->number[i] = 0; 
	// 	printf("%d ", mutex->number[i]);
	// }
    // printf("\n");
}

bool compare(volatile int *number, int i, int id)
{
    if (number[i] < number[id]) return true;
    if (number[i] == number[id] && i < id) return true;

    return false;
}

// void lock(int id)
void lock(LamportMutex *mutex, int id)
{
    // printf("%d \n", id);
    // entering[id] = true;
    mutex->entering[id] = true;
    // printf("%d \n", entering[id]);
    mutex->number[id] = 1 + max(mutex->number);
    // number[id] = 1 + max();
    // printf("%d \n", number[id]);
    mutex->entering[id] = false;
    // entering[id] = false;
    // printf("%d \n", entering[id]);

    for(int i = 0; i < qtd_threads; i++) 
    {
        // esperando até a thread i receber seu ticket
        while(mutex->entering[i] == true);
        // while(entering[i] == true);
        // esperar outras threads com número menor ou igual que tenham prioridade maior terminem seu trabalho
        // while((number[i] != 0) && compare(i, id));
        while((mutex->number[i] != 0) && compare(mutex->number, i, id));
    }
}

// void unlock(int id)
void unlock(LamportMutex *mutex, int id)
{
    // number[id] = 0;
    mutex->number[id] = 0;
}

// int main(int argc, char **argv)
// {
//     if (argc != 2) {
//         printf("para rodar o código use: %s <qtd_threads>\n", argv[0]);
//         return 1;
//     }

//     qtd_threads = atoi(argv[1]); 

//     initialize_mutex(qtd_threads);
// }