#include "reader_writer.h"
#include "reader_writer_tracing.h"


extern tracing_t t; 

typedef struct reader_writer
{
	pthread_mutex_t MutexRead;
	pthread_mutex_t Mutex;
	pthread_cond_t Cond;
	int nbreader;
	char writing=0;
} reader_writer_s; 

reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&(rw->Mutex), NULL);
	pthread_cond_init(&(rw->Cond), NULL);
	rw->nbreader = 0;
	return rw; 
}


void begin_read(reader_writer_t rw)
{
	if (rw->writing == 1)
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	pthread_mutex_lock(&(rw->MutexRead));
	rw->nbreader++;
	pthread_mutex_unlock(&(rw->MutexRead));
	tracing_record_event(t, BR_EVENT_ID);
}

void end_read(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->MutexRead));
	rw->nbreader--;
	pthread_mutex_unlock(&(rw->MutexRead));
	pthread_cond_broadcast(&(rw->CondRead))
	tracing_record_event(t, ER_EVENT_ID);
}

void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	rw->writing = 1;
	while (rw->nbreader > 0)
		pthread_cond_wait(&(rw->CondRead), &(rw->MutexRead));
	tracing_record_event(t, BW_EVENT_ID);
}

void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);
	rw->writing = 0;
	pthread_cond_broadcast(&(rw->Cond));
	pthread_mutex_unlock(&(rw->Mutex));
}