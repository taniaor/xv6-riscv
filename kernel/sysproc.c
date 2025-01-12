#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include <stddef.h>

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_calculate(void)
{
    int x=0;
    int y=0;
    char input_op[16];
    int temp_result;
    int* result;

    argint(0,&x);
    argint(1,&y);
    argstr(2, input_op, sizeof(input_op));
    argint(3, (int*)&result);
    
    if (strncmp(input_op, "+", 1) == 0) {   
        temp_result = x + y;
    } else if (strncmp(input_op, "-", 1) == 0) {
        temp_result = x - y;
    } else if (strncmp(input_op, "*", 1) == 0) {
        temp_result = x * y;
    } else if (strncmp(input_op, "/", 1) == 0) {
        if (y == 0) {
            return -1; // divide by zero error
        }
        temp_result = x / y;
    } else {
        return -1; // Unknown opcode provided or another error occured
    }
    
    copyout(myproc()->pagetable, (uint64)result, (char*)&temp_result, sizeof(int)); 

    return 0; // Success
}

uint64 
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
