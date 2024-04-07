#include <pthread.h>
#include <stdio.h>

#include "lamport.h"

pthread_mutex_t m1, m2, m3, m4, m5, m6;

void *task1(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m1);
		printf("task 1\n");
        // usleep(500000);
		pthread_mutex_unlock(&m2);
	}
}

void *task2(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m2);
		printf("task 2\n");
        // usleep(400000);
		pthread_mutex_unlock(&m3);
	}
}

void *task3(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m3);
		printf("task 3\n");
        // usleep(30000);
		pthread_mutex_unlock(&m1);
	}
}

void *task4(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m4);
		printf("task 4\n");
        // usleep(500000);
		pthread_mutex_unlock(&m5);
	}
}

void *task5(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m5);
		printf("task 5\n");
        // usleep(500000);
		pthread_mutex_unlock(&m6);
	}
}

void *task6(void *arg)
{
	while (1) {
		pthread_mutex_lock(&m6);
		printf("task 6\n");
        // usleep(500000);
		pthread_mutex_unlock(&m4);
	}
}

int main(int argc, char *argv[])
{
	pthread_t threads[6];

	initialize_mutex(6);
	
	// pthread_mutex_init(&m1, NULL);
	// pthread_mutex_init(&m2, NULL);
	// pthread_mutex_init(&m3, NULL);
    // pthread_mutex_init(&m4, NULL);
	// pthread_mutex_init(&m5, NULL);
	// pthread_mutex_init(&m6, NULL);

	// pthread_mutex_lock(&m2);
	// pthread_mutex_lock(&m3); // MUDA O VALOR PRA 0 -> UNLOCK MUDA O VALOR PRA 1
    // O QUE ACONTECE SE NÃO DER LOCK M2 NEM M3? TODOS TEM O VALOR EM 1
    // se não fizer lock no início todos poderão "iniciar"
    // pthread_mutex_lock(&m4);
	// pthread_mutex_lock(&m5);

	// pthread_create(&threads[0], NULL, task1, NULL);
	// pthread_create(&threads[1], NULL, task2, NULL);
	// pthread_create(&threads[2], NULL, task3, NULL);
    // pthread_create(&threads[3], NULL, task4, NULL);
	// pthread_create(&threads[4], NULL, task5, NULL);
	// pthread_create(&threads[5], NULL, task6, NULL);
	
	// pthread_exit(NULL);
	
	return 0;
}