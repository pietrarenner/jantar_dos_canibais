#define MAX_THREADS 10
#define _TIMESPEC_DEFINED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

#define THREADCOUNT 5
CRITICAL_SECTION cs;
int listaItens[15];
int tamanhoAtual = 0;

typedef struct
{
	int data;
	int id;
} ThreadData;

void adicionaItem(void *arg)
{
	EnterCriticalSection(&cs);

	ThreadData *threadData = (ThreadData *)arg;

	if (tamanhoAtual >= 15)
	{
		printf("Lista cheia\n");
		LeaveCriticalSection(&cs);
		return;
	}

	printf("Produtor %d adicionando item %d\n", threadData->id, threadData->data);
	listaItens[tamanhoAtual] = threadData->data;
	tamanhoAtual++;
	LeaveCriticalSection(&cs);
}

void removeItem(void *arg)
{
	EnterCriticalSection(&cs);

	ThreadData *threadData = (ThreadData *)arg;

	if (tamanhoAtual < 0)
	{
		tamanhoAtual = 0;
		printf("Lista vazia\n");
		LeaveCriticalSection(&cs);
		return;
	}

	printf("Consumidor %d removendo item %d\n", threadData->id, threadData->data);
	listaItens[tamanhoAtual] = 0;
	tamanhoAtual--;
	LeaveCriticalSection(&cs);
}

DWORD WINAPI ThreadProdutor(void *data)
{
	while (1)
	{
		adicionaItem(data);
		printf("Tamanho atual: %d\n", tamanhoAtual);

		Sleep(1000);
	}

	return 0;
}

DWORD WINAPI ThreadConsumidor(void *data)
{
	while (1)
	{
		removeItem(data);
		printf("Tamanho atual: %d\n", tamanhoAtual);

		Sleep(1000);
	}

	return 0;
}

int main(int argc, char **argv)
{
	InitializeCriticalSection(&cs);
	HANDLE thread[THREADCOUNT];
	DWORD ThreadID;

	ThreadData threadData[THREADCOUNT];

	for (int i = 0; i < THREADCOUNT; i++)
	{
		threadData[i].data = 5;
    	threadData[i].id = i;

		if (i % 2 == 0)
		{
			printf("Criando produtor: %d\n", threadData[i].id);
			thread[i] = CreateThread(NULL, 0, ThreadProdutor, &threadData[i], 0, &ThreadID);
		}
		else
		{
			printf("Criando consumidor: %d\n", threadData[i].id);
			thread[i] = CreateThread(NULL, 0, ThreadConsumidor, &threadData[i], 0, &ThreadID);
		}
	}

	WaitForMultipleObjects(THREADCOUNT, thread, TRUE, INFINITE);

	for (int i = 0; i < THREADCOUNT; i++)
	{
		CloseHandle(thread[i]);
	}

	DeleteCriticalSection(&cs);
}