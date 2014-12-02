#include "reader_writer.h"
#include "reader_writer_tracing.h"
#include <stdio.h>

extern tracing_t t; 

typedef struct reader_writer
{
	pthread_mutex_t MutexRead;
	pthread_mutex_t Mutex;
	pthread_cond_t Cond;
	int nbreader;
	int writing;
} reader_writer_s; 

reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&(rw->Mutex), NULL);
	pthread_cond_init(&(rw->Cond), NULL);
	rw->nbreader = 0;
	rw->writing = 0;
	return rw; 
}


void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	if (rw->writing > 0)
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	rw->nbreader++;
	tracing_record_event(t, BR_EVENT_ID);
	fprintf(stderr,"BR\n");
	pthread_mutex_unlock(&(rw->Mutex));
}

void end_read(reader_writer_t rw)
{
	fprintf(stderr,"ER\n");
	rw->nbreader--;
	if (rw->nbreader == 0)
		{pthread_cond_broadcast(&(rw->Cond));}
	tracing_record_event(t, ER_EVENT_ID);
}

void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	rw->writing++;
	int temp = rw->writing;
	while((rw->nbreader>0)||(rw->writing>1)) {
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
		if (temp !=1 && rw->nbreader>0)
		{
			temp--;
		}
		else
			break;
	}
	tracing_record_event(t, BW_EVENT_ID);
	fprintf(stderr,"BW\n");
}

void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);
	fprintf(stderr,"EW\n");
	rw->writing--;
	pthread_cond_broadcast(&(rw->Cond));
	pthread_mutex_unlock(&(rw->Mutex));
}
