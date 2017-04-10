#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_getprocs(void) {

    char* copy;
    if (argptr(0, &copy, sizeof(struct uproc) * NPROC) < 0) return -1;

    int count;

    struct uproc* uprocs = (struct uproc*) copy;

    struct proc* procs = getptable();

    // copy kernel processes into user memory
    int i;
    for (i = 0; i < NPROC; i++) {
        struct proc proc = procs[i];
        if (proc.state == UNUSED) {
            continue;
        }

        memmove(uprocs[count].name, proc.name, 16);

        uprocs[count].parentpid = (proc.parent ? proc.parent->pid : -1);
        uprocs[count].procpid = proc.pid;
        uprocs[count].state = proc.state;
        uprocs[count].prio = proc.prio;

        count++;
    }

    return count;
}

int sys_setprio(void) {
  int n;

  if(argint(0, &n) < 0)
    return -1;

  proc->prio = n;
  return n;
}

int sys_getpstat(void) {
    char* csleep;
    char* cwait;
    char* crun;

    if (argptr(0, &csleep, sizeof(int)) < 0) return -1;
    if (argptr(1, &cwait, sizeof(int)) < 0) return -1;
    if (argptr(2, &crun, sizeof(int)) < 0) return -1;

    int* sleep = (int*) csleep;
    int* wait = (int*) cwait;
    int* run = (int*) crun;

    *sleep = proc->time_s;
    *wait = proc->time_w;
    *run = proc->time_r;

    return 0;
}
