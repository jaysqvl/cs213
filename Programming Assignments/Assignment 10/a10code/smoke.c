#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Smoker {
  uthread_mutex_t mutex;
  uthread_cond_t match;
  uthread_cond_t paper;
  uthread_cond_t tobacco;
  uthread_cond_t smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

struct Smoker* createSmoker(struct Agent* a) {
  struct Smoker* smoker = malloc(sizeof(struct Smoker));
  smoker->mutex = a->mutex;
  smoker->paper = a->paper;
  smoker->match = a->match;
  smoker->tobacco = a->tobacco;
  smoker->smoke = a->smoke;
  return smoker;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

void* papert_Smoke(void* av) {
  struct Smoker* sv = av;
  
  uthread_mutex_lock(sv->mutex);
  num_active_threads++;

  uthread_cond_signal(sv->smoke);

  // if it's a not paper, and tobacco signals, then there has to be a match
  while(1) {
    int currP = signal_count[PAPER];

    while(currP == signal_count[PAPER]) {
      uthread_cond_wait(sv->tobacco);
    }

    smoke_count[PAPER] += 1;
    uthread_cond_signal(sv->smoke);
  }

  uthread_mutex_unlock(sv->mutex);
}

void* tobaccot_Smoke(void* av) {
  struct Smoker* sv = av;

  uthread_mutex_lock(sv->mutex);
  num_active_threads++;

  // signals every thread waiting to smoke, to smoke
  uthread_cond_signal(sv->smoke);

  // if it's a not tobacco, and match signals, then there has to be paper
  while(1) {
     int currT = signal_count[TOBACCO];

     while (currT == signal_count[TOBACCO]) {
       uthread_cond_wait(sv->match);
     }

     smoke_count[TOBACCO] += 1;
     uthread_cond_signal(sv->smoke);
  }
  uthread_mutex_unlock(sv->mutex);
}

void* matcht_Smoke(void* av) {
  struct Smoker* sv = av;
  uthread_mutex_lock(sv->mutex);
  num_active_threads++;

  uthread_cond_signal(sv->smoke);

  // if it's a not a match, and paper signals, then there has to be tobacco
  while(1) {
    int currM = signal_count[MATCH];
    
    while (currM == signal_count[MATCH]) {
      uthread_cond_wait(sv->paper);
    }

    smoke_count[MATCH] += 1;
    uthread_cond_signal(sv->smoke);
  }
  
  uthread_mutex_unlock(sv->mutex);
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random resources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    uthread_cond_wait (a->smoke);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 6;
    switch(r) {
    case 0:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    case 1:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 2:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 3:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 4:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 5:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }
  
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  
  // initialize agent
  struct Agent* a = createAgent();
  uthread_t agent_thread;

  struct Smoker* s1 = createSmoker(a);
  struct Smoker* s2 = createSmoker(a);
  struct Smoker* s3 = createSmoker(a);

  uthread_t tobacco_thread;
  uthread_t paper_thread;
  uthread_t matches_thread;


  uthread_init(5);

  // TODO

  agent_thread = uthread_create(agent, a);
  tobacco_thread = uthread_create(tobaccot_Smoke, s1);
  paper_thread = uthread_create(papert_Smoke, s2);
  matches_thread = uthread_create(matcht_Smoke, s3);

  uthread_join(agent_thread, NULL);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);

  return 0;
}
