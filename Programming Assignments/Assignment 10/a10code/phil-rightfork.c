#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_THINKING_TIME 25000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

typedef struct fork {
  uthread_mutex_t lock;
  uthread_cond_t forfree;
  long free; // spinlock
} fork_t;

int num_phils, num_meals;    
fork_t *forks;

void deep_thoughts() {
  usleep(random() % MAX_THINKING_TIME);
}

void initfork(int i) {
  forks[i].lock    = uthread_mutex_create();
  forks[i].forfree = uthread_cond_create(forks[i].lock);
  forks[i].free    = 1; // 1 = free (default state)
}

// given a fork id, pick it up if it's free, else wait
long getfork(long i) {
  /* TO BE IMPLEMENTED BY THE STUDENTS. */
  uthread_mutex_lock(forks[(int) i].lock);
    int chosenFork = (int) i;

    // while the chosen fork is not free, wait
    while (forks[chosenFork].free != 1) {
      printf("GETFORK: Philosopher waiting for fork id #%d\n", i);
      uthread_cond_wait(forks[chosenFork].forfree);
    }

    forks[chosenFork].free = 0;
    printf("GETFORK: Philosopher took fork id #%d\n", i);
  uthread_mutex_unlock(forks[chosenFork].lock);
  return 1;
}

// puts the given fork down
void putfork(long i) {
  /* TO BE IMPLEMENTED BY THE STUDENTS. */
  
  // locks given fork id
  uthread_mutex_lock(forks[(int) i].lock);
    int chosenFork = (int) i;
    
    // if it's already free, signal, if it's taken, free it and signal
    if (forks[chosenFork].free == 0)
      forks[chosenFork].free = 1;
    
    uthread_cond_signal(forks[chosenFork].forfree);
  uthread_mutex_unlock(forks[chosenFork].lock);
}

// returns the leftfork of the given philosopher
int leftfork(long i) {
  return i;
}

// returns the rightfork of the given philosopher
int rightfork(long i) {
  return (i + 1) % num_phils;
}

void *phil_thread(void *arg) {
  uintptr_t* id = (uintptr_t*) arg;
  int meals = 0;

  long left = (long) leftfork(*id);
  printf ("Philosopher %d 's, left fork is fork %d\n", *id, left);
  long right = (long) rightfork(*id);
  printf ("Philosopher %d 's, right fork is fork %d\n", *id, right);
  
  while (meals < num_meals) {
    /* TO BE IMPLEMENTED BY THE STUDENTS. */
    deep_thoughts();
    printf ("Philosopher %d is trying to eat #%d\n", *id, meals);

    printf ("Philosopher %d is acquiring right fork #%d\n", *id, right);
    getfork(right);
    deep_thoughts();

    printf ("Philosopher %d is acquiring left fork #%d\n", *id, left);
    getfork(left);
    deep_thoughts();

    meals++;
    printf("Philosopher %d successfully eats!\n", *id);
    deep_thoughts();

    printf("Philosopher %d tried to drop right fork #%d\n", *id, right);
    putfork(right);
    deep_thoughts();

    printf("Philosopher %d tried to drop left fork #%d\n", *id, left);
    putfork(left);
  }

  printf("Philosopher %d's, FINAL MEAL TOTAL: %d\n", *id, meals);
  
  return 0;
}

int main(int argc, char **argv) {

  uthread_t *p; // list of philosophers
  uintptr_t i;
  
  if (argc != 3) {
    fprintf(stderr, "Usage: %s num_philosophers num_meals\n", argv[0]);
    return 1;
  }

  num_phils = strtol(argv[1], 0, 0);
  num_meals = strtol(argv[2], 0, 0);
  
  forks = malloc(num_phils * sizeof(fork_t)); 
  p = malloc(num_phils * sizeof(uthread_t));

  uthread_init(num_phils);
  
  srandom(time(0));
  for (i = 0; i < num_phils; ++i) {
    initfork(i);
  }

    /* TODO: Create num_phils threads, all calling phil_thread with a
   * different ID, and join all threads.
   */

  int phil_ids[num_phils];

  for (i = 0; i < num_phils; ++i) {
    phil_ids[i] = i;
    p[i] = uthread_create(phil_thread, &phil_ids[i]);
  }

  for (int k = 0; k < num_phils; k++) {
    uthread_join(p[k], NULL);
  }
  
  return 0;
}