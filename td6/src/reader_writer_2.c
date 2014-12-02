#include "reader_writer.h"
#include "reader_writer_tracing.h"
#include <stdio.h>

extern tracing_t t; 

//--------------------------------------------------------------------
//structure définissant les variable de test et les mutex
//--------------------------------------------------------------------
typedef struct reader_writer
{
	pthread_mutex_t Mutex;
	pthread_cond_t Cond;
	int nbreader;
	int writing;
} reader_writer_s; 
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//initialise la structure et les variables globale
//--------------------------------------------------------------------
reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&(rw->Mutex), NULL);
	pthread_cond_init(&(rw->Cond), NULL);
	rw->nbreader = 0;
	rw->writing = 0;
	return rw; 
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//vérouille, s'ajoute à la file puis attend que plus personne n'écrive
//incrémente le nombre de lecteur
//--------------------------------------------------------------------
void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	while (rw->writing > 0)
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	rw->nbreader++;
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
//Incrémente le nombre d'écrivain pour empécher les lecteurs de rentrer
//puis attend que tous les lecteurs ai finis.
//Temp représente la position de l'écrivan dans la file. Quand son
//temp arrive à zéro, alors c'est à lui, autrement il le décrémente.
//--------------------------------------------------------------------
void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	rw->writing++;
	int temp = rw->writing;
	while((rw->nbreader > 0) || (rw->writing > 1))
	{
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
		if (temp !=1 && rw->nbreader>0)
		{
			temp--;
		}
		else
			break;
	}
	tracing_record_event(t, BW_EVENT_ID);
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//Décrémente le nombre d'écrivain puis broadcast et dévérouille
//--------------------------------------------------------------------
void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);
	rw->writing--;
	pthread_cond_broadcast(&(rw->Cond));
	pthread_mutex_unlock(&(rw->Mutex));
}
//--------------------------------------------------------------------