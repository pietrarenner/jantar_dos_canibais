#ifndef LAMPORT_H
#define LAMPORT_H

#include <stdbool.h>

extern int qtd_threads;

typedef struct 
{
    volatile bool *entering;
    volatile int *number;
} LamportMutex;

// Function declaration

int max(volatile int *number);
void lock(LamportMutex *mutex, int id);
void unlock(LamportMutex *mutex, int id);
void initialize_mutex(LamportMutex *mutex, int qtd_threads);
bool compare(volatile int *number, int i, int id);

#endif // lamport