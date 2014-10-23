#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef struct  //struct for 2 parameters
   {
    int * t1;
    int * t2;
    int * out;
    int n;
   } $params;

//Generates a tab of size N with numbers between 0 and 49,999,999
void initRandVect(int* tab, int n)
{
    srand(time(0));
    for(int i=0; i<n; i++)
    {
        tab[i]=rand()%100;
    }
}

void *srchLoop(void *arg)
{
    clock_t cstart = clock();
    clock_t cend = 0;

    $params *p = ($params*)arg;
    int* t1 = p->t1;
    int* t2 = p->t2;
    int n = p->n;
    for(int i=0; i<n; i++)
    {
        p->out[n] = t1[n] * t2[n];
    }
}

void search(int * t1, int* t2, int* out, int n)
{
    pthread_t th[4];

    $params params[4];

    params[0].t1 = t1;
    params[1].t1 = &t1[n/4+1];
    params[2].t1 = &t1[2*(n/4)+1];
    params[3].t1 = &t1[3*(n/4)+1];

    params[0].t2 = t2;
    params[1].t2 = &t2[n/4+1];
    params[2].t2 = &t2[2*(n/4)+1];
    params[3].t2 = &t2[3*(n/4)+1];

    params[0].out = out;
    params[1].out = &out[n/4+1];
    params[2].out = &out[2*(n/4)+1];
    params[3].out = &out[3*(n/4)+1];

    for(int i=0; i<4; i++)
    {
        params[i].n=n/4;
    }

    for(int i=0; i<4; i++)
    {
        pthread_create(&th[i], NULL, srchLoop, &params[i]);
    }

    for(int i=0; i<4; i++)
    {
        pthread_join(th[i], NULL);
    }

    for(int i=0; i<n; i++)
        printf("%d * %d = %d\n",t1[i], t2[i], out[i]);
}

void main(int argc, char** argv)
{
    int n = 100000000;
    static int t1[100000000];
    static int t2[100000000];
    static int out[100000000];

    initRandVect(t1, n);
    initRandVect(t2, n);

    search(t1, t2, out, n);
}