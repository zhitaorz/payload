#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  uint used[NNSP];
} nsptable;

int
allocnsp(void)
{
  acquire(&nsptable.lock);
  
  int i;
  for (i = 0; i < NNSP; i++) {
    if (nsptable.used[i] == 0) {
      nsptable.used[i] = 1;
      release(&nsptable.lock);
      return i;
    }
  }

  release(&nsptable.lock);
  return -1;
}

void
freensp(int num)
{
  acquire(&nsptable.lock);
  nsptable.used[num] = 0;
  release(&nsptable.lock);
  return;
}


int
sys_spawn(void)
{
  // do the spawning of new container logic
  cprintf("spawn called\n");
  return 0;
}