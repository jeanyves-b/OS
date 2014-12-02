#include "reader_writer.h"
#include "reader_writer_tracing.h"
#include <stdio.h>

//--------------------------------------------------------------------
//structure servant à la définition des mutexs et condition.
//--------------------------------------------------------------------
typedef struct reader_writer
{
	pthread_mutex_t Mutex;
	pthread_cond_t Cond;
	int nbreader;
} reader_writer_s;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//structure de la file
//--------------------------------------------------------------------
typedef struct buffer *buffer;
struct buffer
{
	pthread_t id;
	char type;
	char state;
	struct buffer *next;
} buffer_s;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
#define SLEEP 0		//constante utilisé pour rendre le code lisible
#define AWAKE 1

buffer first, last;		//définition des premiers et dernier éléments de la file

extern tracing_t t; 	//définition du tracker
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//ajoute l'élément actuel à la fin de la file
//définie 'last' comme étant l'élément actuel
//--------------------------------------------------------------------
buffer addme(char type)
{
	buffer this = malloc(sizeof(buffer_s));
	this->type = type;
	if (first == NULL)
	{
		first = this;
		last = this;
		this->state = AWAKE;
	}
	else
	{
		last->next = this;
		last = this;
		this->state = SLEEP;
	}
	return this;
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//enlève le premier élément de la file
//si il n'y a plus d'élémnt dans la file, met 'first' à NULL
//--------------------------------------------------------------------
void removefirst()
{
	if (first->next != NULL)
	{
		buffer temp=first;
		first = first->next;
		temp->next = NULL;
		free(temp);
	}
	else
	{
		free(first);
		first = NULL;
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//initialise la structure et les variables globale
//--------------------------------------------------------------------
reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&(rw->Mutex), NULL);		//le mutex
	pthread_cond_init(&(rw->Cond), NULL);		//la condition
	rw->nbreader = 0;					//nombre de thread en lecture
	first = NULL;
	return rw; 
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//vérouille, s'ajoute à la file puis attend que quelqu'un la réveille
//une fois réveillé, s'enlève de la file et réveille le premier
//élémentde la file si il existe puis relache le verroux
//--------------------------------------------------------------------
void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	buffer me = addme('r');
	while (me->state == SLEEP)	//se réveille a chaque appel de cond
	{							//si ce n'était pas son tour, continue a attendre
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	}
	rw->nbreader++;
	removefirst();
	if(first!=NULL)
		first->state = AWAKE;
	tracing_record_event(t, BR_EVENT_ID);
	pthread_mutex_unlock(&(rw->Mutex));
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//diminue le nombre actuel de lecteur et broadcast 
//si il n'y a plus de lecteur qu'un lecteur est parti
//--------------------------------------------------------------------
void end_read(reader_writer_t rw)
{
	rw->nbreader--;
	if (rw->nbreader == 0)
		{pthread_cond_broadcast(&(rw->Cond));}
	tracing_record_event(t, ER_EVENT_ID);
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//vérouille, s'ajoute à la file puis attend son tour et qu'il n'y ai
//plus de lecteur.
//--------------------------------------------------------------------
void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	buffer me = addme('w');
	while (rw->nbreader > 0 || me->state == SLEEP)
	{
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	}
	tracing_record_event(t, BW_EVENT_ID);
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//s'enlève de la file et réveil le suivant en envoyant un broadcast,
//puis dévérouille
//--------------------------------------------------------------------
void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);
	removefirst();
	if(first!=NULL)
		first->state = AWAKE;
	pthread_cond_broadcast(&(rw->Cond));
	pthread_mutex_unlock(&(rw->Mutex));
}
//--------------------------------------------------------------------