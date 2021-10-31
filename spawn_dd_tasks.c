#define _GNU_SOURCE
#include <time.h>
#include "dd.h"
#include <linux/sched.h>

#define US(x) (1000*x)
#define US_R(x) (x/1000)
#define MS(x) (1000*1000*x)
#define MS_R(x) (x/(1000*1000))
#define N 2

static volatile clock_t clock_start;
static volatile int done;

double get_time_us() {
  return ((double)(clock() - clock_start) / CLOCKS_PER_SEC) * 1000000;
}

void start_msg(char* name) {
  printf("[%0.2f] [Start, %d] %s: %ld\n", get_time_us(), sched_getcpu(), name, gettid());
}

void log_msg(char *msg) {
  printf("[%0.2f] [Log, %d] %s: %ld\n", get_time_us(), sched_getcpu(), msg, gettid());
}

void end_msg(char* name) {
  printf("[%0.2f] [End, %d] %s: %ld\n", get_time_us(), sched_getcpu(), name, gettid());
}


// Tasks to test deadlines
void *deadline_task(void* data) {
  sched_attr attr;
  unsigned int flags = 0;

  // Configure attrs
  attr.size = sizeof(attr);
  attr.sched_flags = 0;

  // Deadline reservation
  int runtime = MS(3);
  int period = MS(10);
  int util = (runtime * 100) / period;
  int deadline = period;

  attr.sched_policy = SCHED_DEADLINE;
  attr.sched_runtime = runtime;
  attr.sched_period = period;
  attr.sched_deadline = period;

  // Set attributes: pid = 0 implies calling thread params set
  int ret = sched_setattr(0, &attr, flags);
  if (ret < 0) {
    done = 0;
    perror("sched_setattr");
    exit(-1);
  }

  // Set affinity
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);

  char msg[80];
  sprintf(msg, "dd_task (%d)", util);
  start_msg(msg);
  char* input_message = data;

  /* Do the actual computation */
  int k = 0;
  while(!done) {
    for (int i = 0; i < 100000; i++) {
      k += i;
    }
    //sched_yield();
    //log_msg(input_message);
  }
  
  input_message += 3;
  end_msg(msg);
  return input_message;
}

int main (int argc, char **argv) {
  pthread_t thread[N];
  clock_start =  clock();

  start_msg("main");
  done = 0;

  char s[N][15] = {"hello_task1", "hello_task2"};
  // In microsecs
  for (int i = 0; i < N; i++) {
    int val = pthread_create(&thread[i], NULL, deadline_task, &s[i]);
    printf("%d\n", val);
    if (val != 0) {
      done = 0;
      perror("pthread_create");
      exit(-1);
    }
  }
  
  sleep(90);
  
  done = 1;
  void* ret_val;
  for (int i = 0; i < N; i++) {
    int s = pthread_join(thread[i], &ret_val);
    if (s != 0) {
      perror("pthread_join");
      exit(-1);
    }
    printf("Finished task %d (%s)\n", i, (char*)ret_val); 
  }

  end_msg("main");
}
