#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
	char *song;
	int nb;
}ts_s, *ts;

void *supporter (void *arg)
{
	int NB_SONG = ((ts)arg)->nb;
	char *str = ((ts)arg)->song ;
	int	i ;
	int	pid ;
	pthread_t tid ;	
	pid = getpid () ;
	tid = pthread_self () ;
	srand ((int) tid) ;

	for (i = 0; i < NB_SONG; i++)
	{
		printf("Processus %d Thread %x : %s \n", pid, (unsigned int) tid, str);
		usleep(rand() / RAND_MAX * 1000000.);
	}
	return (void *) tid ;
}

int main (int argc, char **argv)
{
	
	int team1 ;
	int team2 ;
	int i ;
	int nb_threads = 0 ;
	pthread_t *tids ;

	if (argc != 3){
		fprintf(stderr, "usage : %s team1 team2\n", argv[0]) ;
		exit (-1) ;
	}
	
	team1 = atoi (argv[1]) ;
	team2	= atoi (argv[2]) ;
	nb_threads = team1 + team2; 
	tids = malloc (nb_threads*sizeof(pthread_t)) ;

	ts t1 = malloc(sizeof(ts_s)), t2 = malloc(sizeof(ts_s));

	t1->song = "Allons enfants de la patrie";
	t1->nb = 3;
	t2->song = "Swing low, sweet chariot";
	t2->nb = 5;

	/* Create the threads for team1 */
	for (i = 0 ; i < team1; i++)
	{
		pthread_create (&tids[i], NULL, supporter, t1) ;
	}
	/* Create the other threads (ie. team2) */
	for ( ; i < nb_threads; i++)
	{
		pthread_create (&tids[i], NULL, supporter, t2) ;
	}	

	/* Wait until every thread ened */ 
	for (i = 0; i < nb_threads; i++)
	{
		pthread_join (tids[i], NULL) ;
	}
	
	free (tids) ;
	return EXIT_SUCCESS;
}
