#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;
volatile int pending_reads;

void interrupt_service_routine () {
  void* val;
  void (*callback)(void*,void*);
  queue_dequeue (pending_read_queue, &val, NULL, &callback);
  callback (val, NULL);
}

void handle_read (void* resultv, void* not_used) {
  // TODO add result to sum
  int* result = resultv;
  sum += *result;
  pending_reads--;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: aRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();
  pending_reads = num_blocks;

  // int ** is an array of int pointers 
  // int r[] is the same if you give your function the address of it e.g. &result[]
  int result[num_blocks];

  // Trigger reads and sum blocks
  // TODO
  for (int blockno = 0; blockno < num_blocks; blockno++) {
    void* handle_rf = handle_read;
    queue_enqueue(pending_read_queue, &result[num_blocks], NULL, handle_rf);
    disk_schedule_read(&result[num_blocks], blockno);
  }


  // polling loop so that main doesn't return before all of the reads complete
  while (pending_reads > 0);

  printf ("%d\n", sum);
}


