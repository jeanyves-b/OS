#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "reader_writer_tracing.h"

#define BR_EVENT_ID 0
#define ER_EVENT_ID 1
#define BW_EVENT_ID 2
#define EW_EVENT_ID 3

tracing_t t; 
void *func(void *a){
  int id = (long int)a; 
  tracing_register_thread(t, id); 

  int i; 
  for(i = 0; i < 10; i++){    
    tracing_record_event(t, rand()%4);
  }
  return NULL;
}

int main(){
  tracing_s tracing; 
  t = &tracing; 

  tracing_init(t, 4); 

  /* Give a human readable name to events */ 
  tracing_register_event(t, BR_EVENT_ID, "BEGIN READ");
  tracing_register_event(t, ER_EVENT_ID, "END READ");
  tracing_register_event(t, BW_EVENT_ID, "BEGIN WRITE");
  tracing_register_event(t, EW_EVENT_ID, "END READ");

  pthread_t tids[4]; 
  long int i; 
  for(i = 0; i < 4; i++){
    pthread_create(tids+i, NULL, func, (void*)i); 
  }

  for(i = 0; i < 4; i++){
    pthread_join(tids[i], NULL); 

  }

  print_all_events_per_date(t); 

  return EXIT_SUCCESS;
}
