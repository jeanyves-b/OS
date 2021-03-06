#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
	int start, end, *q, pos;
}table_s, *table;

int nbthread = 4, n = 20;

//------------------------------------------------------------
int sumseq(int *q, int n)
{
	int ret=0;
	for (int i=0 ; i< n ; i++)
		ret += q[i];
	return ret;
}
//------------------------------------------------------------
void *sum(void *a)
{
	int *ret=malloc(sizeof(int));
	table s = (table)a;
	*ret = 0;
	for(int i=s->start ; i<=s->end ; i++)
		*ret += s->q[i];
	return ret;
}
//------------------------------------------------------------
int sumthread(int *q, int n)
{
	int ret = 0;
	void **temp = malloc(sizeof(void*));
	pthread_t tids[nbthread];
	table *s = malloc(sizeof(table_s)*nbthread);
	for(int i=0 ; i<nbthread ; i++)
	{
		s[i] = malloc(sizeof(table_s));
		s[i]->q = q;
		s[i]->start = i*(n/nbthread);
		s[i]->end = (i+1)*(n/nbthread) -1;
		s[i]->pos = i;
		pthread_create(&tids[i], NULL, sum, s[i]);
	}
	for(int i = 0 ; i<nbthread ; i++)
	{
		pthread_join (tids[i], temp);
		ret += **(int **)temp;
		free(s[i]);
	}
	free(s);
	free(temp);
	return ret;
}
//------------------------------------------------------------
int main(int argc, char *argv[])
{
	srand(time(0));
	if (argc < 2)
		printf("strarting with n = 20 and nbthread = 4\n");
	else if (argc == 2)
	{
		n = atoi(argv[1]);
		nbthread = atoi(argv[2]);
		printf("strarting with n = %i and nbthread = %i\n", n, nbthread);
	}
	else
	{
		printf("to many values.\n use : ./main n nbthread\n");
		exit(0);
	}

	int q[n];
	for(int i=0; i<n ; i++)
		q[i] = rand()%40;
	if (sumseq(q,n) == sumthread(q,n))
		printf("patate\n");
	else
		printf("papillon\n");
	return 0;
}
//------------------------------------------------------------
