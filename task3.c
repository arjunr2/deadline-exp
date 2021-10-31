#define _GNU_SOURCE
#include <time.h>
#include "dd.h"
#include <sched.h>

int main (int argc, char** argv) {
  int k = 0;
  //printf("[%s, %d]: %ld\n", argv[0], sched_getcpu(), gettid());
  while(1) {
    for (int i = 0; i < 1000; i++)
      k += i;
  }
  return 0;
}
