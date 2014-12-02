#include "reader_writer.h"
#include "reader_writer_tracing.h"
#include <stdio.h>
#define SLEEP 0
#define AWAKE 1

extern tracing_t t; 

typedef struct reader_writer
{
	pthread_mutex_t Mutex;
	pthread_cond_t Cond;
	int nbreader;
	int writing;
} reader_writer_s;

typedef struct buffer *buffer;
struct buffer
{
	pthread_t id;
	char type;
	char state;
	struct buffer *next;
} buffer_s;

buffer first, last;

buffer addme(char type)
{
	buffer this = malloc(sizeof(buffer_s));
	this->type = type;
	this->state = SLEEP;
	if (first == NULL)
	{
		first = this;
		last = this;
	}
	else
	{
		last->next = this;
		last = this;
	}
	return this;
}

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
	}
}

reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&(rw->Mutex), NULL);
	pthread_cond_init(&(rw->Cond), NULL);
	rw->nbreader = 0;
	rw->writing = 0;
	first = NULL;
	return rw; 
}


void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	buffer me = addme('r');
	while ((me->state == SLEEP && me != first))
	{
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	}
	rw->nbreader++;
	if (me->next != NULL && me->next->type == 'w')
		me->next->state = AWAKE;
	removefirst();
	first->state = AWAKE;
	fprintf(stderr, "BR\n");
	tracing_record_event(t, BR_EVENT_ID);
	pthread_mutex_unlock(&(rw->Mutex));
}

void end_read(reader_writer_t rw)
{
//	pthread_mutex_lock(&(rw->Mutex));
	rw->nbreader--;
	fprintf(stderr, "ER %d\n",rw->nbreader);
	tracing_record_event(t, ER_EVENT_ID);
	pthread_cond_broadcast(&(rw->Cond));
//	pthread_mutex_unlock(&(rw->Mutex));
}

void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&(rw->Mutex));
	buffer me = addme('w');
	fprintf(stderr, "BBW\n");
	while (rw->nbreader > 0 || (me->state == SLEEP && me != first))
	{
		pthread_cond_wait(&(rw->Cond), &(rw->Mutex));
	}
	removefirst();
	first->state = AWAKE;
	tracing_record_event(t, BW_EVENT_ID);
	fprintf(stderr, "BW\n");
}

void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);
	fprintf(stderr, "EW\n");
	pthread_cond_broadcast(&(rw->Cond));
	pthread_mutex_unlock(&(rw->Mutex));
}
