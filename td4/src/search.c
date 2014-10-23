#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef struct  //struct for 2 parameters
   {
     int * T;
     int n;
     int x;
   } $params;

//Generates a tab of size N with numbers between 0 and 49,999,999
void initRandVect(int* tab, int n)
{
    srand(time(0));
    for(int i=0; i<n; i++)
    {
        tab[i]=rand()%50000000;
    }
}

void *srchLoop(void *arg)
{
    clock_t cstart = clock();
    clock_t cend = 0;

    $params *p = ($params*)arg;
    int* T = p->T;
    int n = p->n;
    int x = p->x;
    for(int i=0; i<n; i++)
    {
        if(T[i] == x)
        {
            cend = clock();
            printf("first appearance at index : %d\n",i);
            printf ("%d cpu clocks\n", ((int)cend - (int)cstart));
            exit(0);
        }
    }
}

void search(int * T, int n, int x)
{
    pthread_t t[4];

    $params params[4];

    params[0].T = T;
    params[1].T = &T[n/4+1];
    params[2].T = &T[2*(n/4)+1];
    params[3].T = &T[3*(n/4)+1];

    for(int i=0; i<4; i++)
    {
        params[i].n=n/4;
        params[i].x=x;
    }

    clock_t cstart = clock();
    clock_t cend = 0;

    for(int i=0; i<4; i++)
    {
        pthread_create(&t[i], NULL, srchLoop, &params[i]);
    }

    for(int i=0; i<4; i++)
    {
        pthread_join(t[i], NULL);
    }

    cend = clock();
    printf("%d\n",-1);
    printf ("%d cpu clocks\n", ((int)cend - (int)cstart));
    exit(0);
}

void main(int argc, char** argv)
{
    int n = 100000000;
    int target = 25000000;
    static int tab[100000000];
    
    initRandVect(tab, n);

    search(tab, n, target);
}