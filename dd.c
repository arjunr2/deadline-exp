#include "dd.h"

/* Flags is for future extensions, currently set it to 0 always */
int sched_setattr(pid_t pid, const sched_attr *attr, unsigned int flags) {
  return syscall(__NR_sched_setattr, pid, attr, flags);
}

/* Flags is for future extensions, currently set it to 0 always */
int sched_getattr(pid_t pid, const sched_attr *attr, unsigned int size, unsigned int flags) {
  return syscall(__NR_sched_setattr, pid, attr, size, flags);
}

