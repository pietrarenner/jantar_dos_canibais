
#include <stdio.h>
#include <stdbool.h>
#include "lamport.h"
#include <stdlib.h>

int qtd_threads;

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
{
    qtd_threads = qtd;
    mutex->entering = (bool *)malloc(qtd_threads * sizeof(bool));
    mutex->number = (int *)malloc(qtd_threads * sizeof(int));
}

bool compare(volatile int *number, int i, int id)
{
    if (number[i] < number[id]) return true;
    if (number[i] == number[id] && i < id) return true;

    return false;
}

void lock(LamportMutex *mutex, int id)
{
    mutex->entering[id] = true;
    mutex->number[id] = 1 + max(mutex->number);
    mutex->entering[id] = false;

    for(int i = 0; i < qtd_threads; i++) 
    {
        while(mutex->entering[i] == true);
        while((mutex->number[i] != 0) && compare(mutex->number, i, id));
    }
}

void unlock(LamportMutex *mutex, int id)
{
    mutex->number[id] = 0;
}