#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <semaphore.h>

#include "lamport.h"

pthread_mutex_t mutex; // acesso a região crítica
volatile int *listaItens; // Lista de itens, agora um ponteiro
volatile int *ptr_Inicio; // controla o ínicio da fila
volatile int *ptr_Fim; // controla o final da fila

sem_t empty, full; // empty: conta quantidade de posições vazias; full: conta quantidade de posições cheias

typedef struct
{
	int data;
	int id;
	int tam_lista;
} ThreadData;

void adicionaItem(void *arg)
{
	ThreadData *threadData = (ThreadData *)arg;

	printf("Produtor %d adicionando item %d\n", threadData->id, threadData->data);

	int pos =  ptr_Fim - listaItens;
	listaItens[pos] = threadData->data;
	ptr_Fim++;

	if (ptr_Fim >= listaItens + threadData->tam_lista) {
		ptr_Fim = listaItens;
	}

	for (int i = 0; i < threadData->tam_lista; i++)
	{
		printf("%d ", listaItens[i]);
	}

	printf("\n");

	printf("ptr inicio: %td\nptr fim: %td\n", ptr_Inicio - listaItens, ptr_Fim - listaItens);

	sleep(1);
}

void removeItem(void *arg)
{
	ThreadData *threadData = (ThreadData *)arg;

	printf("Consumidor %d removendo item %d\n", threadData->id, threadData->data);

	int pos =  ptr_Inicio - listaItens;
	listaItens[pos] = 0;

	ptr_Inicio++;

	if (ptr_Inicio >= listaItens + threadData->tam_lista) {
		ptr_Inicio = listaItens;
	}

	for (int i = 0; i < threadData->tam_lista; i++)
	{
		printf("%d ", listaItens[i]);
	}
	
	printf("\n");

	printf("ptr inicio: %td\nptr fim: %td\n", ptr_Inicio - listaItens, ptr_Fim - listaItens);

	sleep(1);
}



void* ThreadProdutor(void *data)
{
	while (1)
	{
		ThreadData *threadData = (ThreadData *)data;
		sem_wait(&empty); // decrementa contador do sem empty
		// lock(threadData->id);	
		pthread_mutex_lock(&mutex); // entra na região crítica
		adicionaItem(data);
		pthread_mutex_unlock(&mutex);
		// unlock(threadData->id);
		sem_post(&full); // incrementa contador do semáforo full
		
	}

	return 0;
}

void* ThreadConsumidor(void *data)
{
	while (1)
	{
		ThreadData *threadData = (ThreadData *)data;
		sem_wait(&full); // decrementar contador do full
		// lock(threadData->id);	
		pthread_mutex_lock(&mutex); // entrar na região crítica
		removeItem(data);
		pthread_mutex_unlock(&mutex);
		// unlock(threadData->id);	
		sem_post(&empty);		
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
        printf("para rodar o código use: %s <tam_lista> <qtd_threads>\n", argv[0]);
        return 1;
    }

    int tam_lista = atoi(argv[1]); // tamanho da lista
	int qtd_threads = atoi(argv[2]); // quantidade de threads

	listaItens = (int *)malloc(tam_lista * sizeof(int)); // Aloca a lista de itens

	ptr_Inicio = listaItens;
	ptr_Fim = listaItens;

	sem_init(&empty, 0, tam_lista);
	sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);
	// initialize_mutex(qtd_threads);
	pthread_t thread[qtd_threads];

	ThreadData threadData[qtd_threads];

	for (int i = 0; i < qtd_threads; i++)
	{
		threadData[i].data = i + 1; // item que ele tá passando (vai adicionar na lista)
    	threadData[i].id = i; // id da thread
		threadData[i].tam_lista = tam_lista;

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

    for (int i = 0; i < qtd_threads; i++)
    {
        pthread_join(thread[i], NULL);
    }
}