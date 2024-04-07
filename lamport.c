
#include <stdio.h>
#include <stdbool.h>
#include "lamport.h"
#include <stdlib.h>

volatile bool *entering;
volatile int *number;
int qtd_threads;

int max()
{
    int r, x;

    r = number[0];
    for (x = 0; x < qtd_threads; x++) {
        if (number[x] >= r)
            r = number[x];
    }
    return(r);
}

void initialize_mutex(int qtd_threads)
{
    entering = (bool *)malloc(qtd_threads * sizeof(bool));
    number = (int *)malloc(qtd_threads * sizeof(int));

    printf("entering: ");
    for (int i = 0; i < qtd_threads; i++)
	{
        entering[i] = false; 
		printf("%d ", entering[i]);
	}
    printf("\n");

    printf("number: ");
    for (int i = 0; i < qtd_threads; i++)
	{
        number[i] = 0; 
		printf("%d ", number[i]);
	}
    printf("\n");
}

bool compare(int i, int id)
{
    if (number[i] < number[id]) return true;
    if (number[i] == number[id] && i < id) return true;

    return false;
}

void lock(int id)
{
    printf("%d \n", id);
    entering[id] = true;
    printf("%d \n", entering[id]);
    number[id] = 1 + max();
    printf("%d \n", number[id]);
    entering[id] = false;
    printf("%d \n", entering[id]);

    for(int i = 0; i < qtd_threads; i++) 
    {
        // esperando até a thread i receber seu ticket
        while(entering[i] == true);
        // esperar outras threads com número menor ou igual que tenham prioridade maior terminem seu trabalho
        while((number[i] != 0) && compare(i, id));
    }
}

void unlock(int id)
{
    number[id] = 0;
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