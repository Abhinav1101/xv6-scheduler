#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
//#include <conio.h>

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

struct processInfo
{
    int ppid;
    int psize;
    int numberContextSwitches;
};

int
sys_hello(void)
{
  cprintf("Hello\n");
  return 0;
}

int
sys_helloYou(void)
{ 
  char* name;
  if(argstr(0, &name) < 0)
    return -1;

  cprintf("%s\n", name);
  return 0;
}

int 
sys_getNumProc(void)
{
    struct proc *p;
    int count = 0;
    acquire(&ptable.lock);

    for(p = ptable.proc; p < &ptable.proc[NPROC]; ++p)
    {
      if(p->state != UNUSED)
        count++;
    }
    release(&ptable.lock);

//    cprintf("%d\n", count);

//    int count = getNumProc();
    return count;
}


int 
sys_getMaxPid(void)
{
    struct proc *p;
    int maxx = 0;
    acquire(&ptable.lock);

    for(p = ptable.proc; p < &ptable.proc[NPROC]; ++p)
    { 
      //cprintf("%d\n", p->pid);
      if(p->state != UNUSED && p->pid > maxx)   
        maxx = p->pid;
    }
    release(&ptable.lock);

//    cprintf("%d\n", maxx);
    //int maxx = getMaxPid();
    return maxx;
}
// https://stackoverflow.com/questions/53383938/pass-struct-to-xv6-system-call?rq=1
int 
sys_getProcInfo(void)
{ 
    int pid;
    struct processInfo* inf;
    struct proc *p;

    if(argint(0, &pid) < 0)
      return -1;

    // https://arjunkrishnababu96.gitlab.io/post/xv6-system-call/
    if(argptr(1, (void*)&inf, sizeof(inf)) < 0)
      return -1;

    //if(argint(0, &inf) < 0)
      //return -1;

    //cprintf("\n%d\n", p->parent);
    acquire(&ptable.lock);
    int flag = 0;
    // ask doubt: C not support call by ref. so do we print it here or if want to pass need pointer to a pointer
    // https://stackoverflow.com/questions/4844914/having-a-function-change-the-value-a-pointer-represents-in-c
    for(p = ptable.proc; p < &ptable.proc[NPROC]; ++p)
    { 
      //cprintf("%d\n", p->pid);
      if(p->pid == pid)   
      {   
          inf->ppid = 0;
          if(p->parent != 0)
          { 
              inf->ppid = p->parent->pid; // check if no process present with given pid;
          }
          inf->psize = p->sz;
          inf->numberContextSwitches = p->cswitches;
          //cprintf("\n%d\n", inf->psize);
          //cprintf("\n%d\n", inf->ppid);
          //cprintf("\n%d\n", inf->numberContextSwitches);
          flag = 1;//found
          break;
      }
    }
    release(&ptable.lock);

    if(flag == 0)  // no process with given pid
      return -1;

    return 0;
}

int
sys_setprio()
{   
  int pri;
  if(argint(0, &pri) < 0)
      return -1;

    myproc()->priority = pri;

  return 0;
}

int
sys_getprio()
{
  return myproc()->priority;
}

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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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
