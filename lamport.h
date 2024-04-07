#ifndef LAMPORT_H
#define LAMPORT_H

#include <stdbool.h>

extern volatile bool *entering;
extern volatile int *number;
extern int qtd_threads;

// Function declaration

int max();
void lock(int id);
void unlock(int id);
void initialize_mutex(int qtd_threads);
bool compare(int i, int id);

#endif // lamport