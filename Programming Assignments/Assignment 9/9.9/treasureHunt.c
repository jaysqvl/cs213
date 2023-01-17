#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

queue_t pending_read_queue;
int pending_reads;
volatile int are_reads_pending = 0; // flag that indicates whether a read is still in progress


void interrupt_service_routine() {
  void* val;
  void* count;
  void (*handleRead)(void*,void*);

  queue_dequeue (pending_read_queue, &val, &count, &handleRead);
  handleRead (val, count);
}

void handleOtherReads(void *resultv, void *countv) { 
  int* result = (int*) resultv; // pointer to blockno
  int* count = (int*) countv; // reads left

  // update reads remaining
  *count -= 1;
  
  if (pending_reads > 0) {
    disk_schedule_read(result, *result);
    queue_enqueue(pending_read_queue, result, &pending_reads, handleOtherReads);
  } else {
    // if no more reads in queue finishes the program (same as exit)
    are_reads_pending = 1;
  }
}

void handleFirstRead(void *resultv, void *countv) {
  int* result = (int*) resultv; // pointer to blockno
  int* count = (int*) countv; // reads left
  *count = *result;

  // update reads
  pending_reads = *count;

  queue_enqueue(pending_read_queue, result, count, handleOtherReads);
  disk_schedule_read(result, *result);
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char* usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // size of int
  int* resultFirst = malloc(sizeof(int));

  // neeeds to point to our global pending reads so that enqueue can update it
  int* count = &pending_reads;

  queue_enqueue(pending_read_queue, resultFirst, count, handleFirstRead);
  disk_schedule_read(resultFirst, starting_block_number);
  //enqueue(queue, result value, reads left, function);

  // are_read_ending is a completion tag;
  while (are_reads_pending == 0);

  printf("%d\n", *resultFirst);
}
