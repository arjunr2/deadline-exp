#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sched.h>

#define gettid() syscall(__NR_gettid)
//#define SCHED_DEADLINE_POLICY 6

#define __NR_sched_setattr 314
#define __NR_sched_getattr 315


typedef struct {
  __u32 size;
  __u32 sched_policy;
  __u64 sched_flags;

  /* SCHED_NORMAL, SCHED_BATCH */
  __s32 sched_nice;
  /* SCHED_FIFO, SCHED_RR */
  __u32 sched_priority;

  /* SCHED_DEADLINE params */
  __u64 sched_runtime;
  __u64 sched_deadline;
  __u64 sched_period;

} sched_attr;

/* Flags is for future extensions, currently set it to 0 always */
int sched_setattr(pid_t pid, const sched_attr *attr, unsigned int flags);

/* Flags is for future extensions, currently set it to 0 always */
int sched_getattr(pid_t pid, const sched_attr *attr, unsigned int size, unsigned int flags);

