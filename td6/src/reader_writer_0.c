#include "reader_writer.h"
#include "reader_writer_tracing.h"


extern tracing_t t; 

typedef struct reader_writer
{
	pthread_mutex_t Mutex;
} reader_writer_s; 

reader_writer_t rw_init()
{
	reader_writer_t rw = malloc(sizeof(reader_writer_s));
	pthread_mutex_init(&Mutex);
	return rw; 
}


void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock(rw->Mutex);
	tracing_record_event(t, BR_EVENT_ID);
}

void end_read(reader_writer_t rw)
{

	tracing_record_event(t, ER_EVENT_ID);
	pthread_mutex_unlock(rw->Mutex);
}

void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(rw->Mutex);
	tracing_record_event(t, BW_EVENT_ID); 
}

void end_write(reader_writer_t rw)
{
	tracing_record_event(t, EW_EVENT_ID);  
	pthread_mutex_unlock(rw->Mutex);
}