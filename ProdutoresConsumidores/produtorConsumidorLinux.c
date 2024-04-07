#define MAX_THREADS 10
#define _TIMESPEC_DEFINED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

#define THREADCOUNT 5
pthread_mutex_t mutex;
int listaItens[15];
int tamanhoAtual = 0;

typedef struct
{
	int data;
	int id;
} ThreadData;

void adicionaItem(void *arg)
{
	pthread_mutex_lock(&mutex);

	ThreadData *threadData = (ThreadData *)arg;

	if (tamanhoAtual >= 15)
	{
		printf("Lista cheia\n");
		pthread_mutex_unlock(&mutex);
		return;
	}

	printf("Produtor %d adicionando item %d\n", threadData->id, threadData->data);
	listaItens[tamanhoAtual] = threadData->data;
	tamanhoAtual++;
	pthread_mutex_unlock(&mutex);
}

void removeItem(void *arg)
{
	pthread_mutex_lock(&mutex);

	ThreadData *threadData = (ThreadData *)arg;

	if (tamanhoAtual < 0)
	{
		tamanhoAtual = 0;
		printf("Lista vazia\n");
		pthread_mutex_unlock(&mutex);
		return;
	}

	printf("Consumidor %d removendo item %d\n", threadData->id, threadData->data);
	listaItens[tamanhoAtual] = 0;
	tamanhoAtual--;
	pthread_mutex_unlock(&mutex);
}

void* ThreadProdutor(void *data)
{
	while (1)
	{
		adicionaItem(data);
		printf("Tamanho atual: %d\n", tamanhoAtual);

		struct timespec ts = {1, 0}; // 1 segundo
        nanosleep(&ts, NULL);
	}

	return 0;
}

void* ThreadConsumidor(void *data)
{
	while (1)
	{
		removeItem(data);
		printf("Tamanho atual: %d\n", tamanhoAtual);

		struct timespec ts = {1, 0}; // 1 segundo
        nanosleep(&ts, NULL);
	}

	return 0;
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&mutex, NULL);
	pthread_t thread[THREADCOUNT];

	ThreadData threadData[THREADCOUNT];

	for (int i = 0; i < THREADCOUNT; i++)
	{
		threadData[i].data = 5;
    	threadData[i].id = i;

		if (i % 2 == 0)
		{
			printf("Criando produtor: %d\n", threadData[i].id);
			pthread_create(&thread[i], NULL, ThreadProdutor, &threadData[i]);
		}
		else
		{
			printf("Criando consumidor: %d\n", threadData[i].id);
			pthread_create(&thread[i], NULL, ThreadConsumidor, &threadData[i]);
		}
	}

    for (int i = 0; i < THREADCOUNT; i++)
    {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
}