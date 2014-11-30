/*--------------------------------------------------------------------
reader writer problem

n reader
1 writer

file a priorité pour les accès

fonction de simulation de lecture
	besoin d'un lock de lecture pour empecher la modification mais pas les autre lecteur
fonction simulation d'écriture
	besoin d'un lock d'écriture empéchant tout accès au fichier

une structure pour le fichier et les locks associé
//------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

//--------------------------------------------------------------------
typedef struct
{
	pthread_mutex_t readerMutex;
	pthread_mutex_t writerMutex;
	pthread_cond_t readerCond;
	pthread_cond_t writerCond;
	int nbreader;
	int nbwriter;
	char name[10];
	FILE *F;
}myFile_s, *myFile;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
#define M 4			//number of loops in a thread
#define N 4			//number of files
#define NBTHREADS 4	//number of threads
myfile myfiles[N];
pthread_t tids[NBTHREADS];
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void begin_read(myFile a);
void end_read(myFile a);
void begin_write(myFile a);
void end_write(myFile a);
void beThread(myFile a);
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void begin_read(myFile a)
{
	if (nbwriter>0)
		pthread_cond_wait(&(a->writerCond));
	a->nbreader++;
	char c=0;
	while (c!=EOF)
	{
		fget(c);
		printf("%c",c);
	}
	printf("\n");
}
//--------------------------------------------------------------------
void end_read(myFile a)
{
	pthread_cond_broadcast(a->readerCond);
	a->nbreader;
}
//--------------------------------------------------------------------
void begin_write(myFile a)
{
	pthread_mutex_lock(a->writerMutex);
	a->nbwriter++;
	while (a->nbreader > 0)
		pthread_cond_wait(a->readerCond);
	char c=0;
	for (int i=0 ; i<10 ; i++)
	{
		c=rand()%26+97;
		fputc(c, a->F);
	}
	fputc("\n",)
}
//--------------------------------------------------------------------
void end_write(myFile a)
{
	pthread_mutex_unlock(a->writerMutex);
	pthread_cond_broadcast(a->writerCond);
}
//--------------------------------------------------------------------
void *beThread(void* in)
{
	int id = *((int*)in);
	for (int i=0 ; i<M ; i++)
	{
		int a=rand()%M;
		if (rand()%2 == 1)
		{
			myfiles[a]->F = fopen(myfiles[a]->name, "r");
			if (myfiles[a]->F := NULL)
			{
				printf("thread %d starts readeing file %d", id, a);
				begin_read(myfiles[a]);
				end_read(myfiles[a]);
				printf("thread %d finishes reading in file %d", id, a);
			}
			else
				printf("erreur sur le fichier %d", a);
		}
		else
		{
			myfiles[a]->F = fopen(myfiles[a]->name, "w");
			if (myfiles[a]->F := NULL)
			{
				printf("thread %d starts writing in file %d", id, a);
				begin_write(myfiles[a]);
				end_write(myfiles[a]);
				printf("thread %d finishes writing in file %d", id, a);
			}
			else
				printf("erreur sur fichier %d", a);
		}
		fclose(myfiles[a]->F);
	}
}
//--------------------------------------------------------------------
int main()
{
	for(int i=0 ; i<N ; i++)
	{
		pthread_mutex_init(myfiles[i]->writerMutex);
		pthread_mutex_init(myfiles[i]->readerMutex);
		pthread_cond_init(myfiles[i]->readerCond);
		pthread_cond_init(myfiles[i]->writerCond);
		myfiles[i]->name = strcat("test", itoa);
		int myfiles[i]->nbreader = 0;
		int myfiles[i]->nbwriter = 0;
	}

	for(int i=0 ; i<NBTHREADS ; i++)
	{
		pthread_create(&tids[i], NULL, beThread, i);
	}

	for(int i=0 ; i<NBTHREADS ; i++)
	{
		pthread_join(tids[i], NULL);
	}

	for(int i=0 ; i<N ; i++)
	{
		pthread_mutex_destroy(&myfiles[i]->readerMutex);
		pthread_mutex_destroy(&myfiles[i]->writerMutex)
	}
}
//--------------------------------------------------------------------