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

void interrupt_service_routine () {
  // TODO

  // Will unblock the thread when disk_schedule read completes
  void* val;
  queue_dequeue(pending_read_queue, &val, NULL, NULL);
  uthread_t unblockMe = val;
  uthread_unblock(unblockMe);
}

void* read_block (void* blocknov) {
  // TODO schedule read and the update (correctly)
  int* which_block = blocknov;
  int realResult;

  uthread_t currSum = uthread_self();
  queue_enqueue(pending_read_queue, currSum, NULL, NULL);

  // read the block given and store it into realResult
  disk_schedule_read(&realResult, *which_block);
  uthread_block();

  // sum the result
  sum += realResult;
  return NULL;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
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

  // Sum Blocks
  // TODO
  
  // where the disk read blocks are gonna get placed
  int result[num_blocks];

  // array of threads
  uthread_t threads[num_blocks];

  // fill our threads array
  for (int blockno = 0; blockno < num_blocks; blockno++) {
    // store the block number inside the result so that we know which block we're reading
    result[blockno] = blockno;

    // uthread_create(void *(*start_proc)(void *), void *start_arg);
    threads[blockno] = uthread_create(read_block, &result[blockno]);
  }

  for (int blockno = 0; blockno < num_blocks; blockno++) {
    uthread_join(threads[blockno], 0);
  }

  printf ("%d\n", sum);
}

