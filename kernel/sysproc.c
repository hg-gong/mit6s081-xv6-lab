#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
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
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  struct proc* p = myproc(); // get current process pagetable

  uint64 usrpg_va; // the strating virtual address of the first user page to check
  int npage; // num of page to check
  uint64 buf_addr; // user address of the buffer that store the bitmask
  uint64 clear_ptea = ~PTE_A; // used to claer PTE_A

  argaddr(0, &usrpg_va);
  argint(1, &npage);
  argaddr(2, &buf_addr);

  if(npage > 64) return -1; // uint64 could only record 64 page

  uint64 bitmap = 0; // bitmap record which page have beem accessed
  int cnt = 0;

  for(uint64 page_va = usrpg_va; page_va < usrpg_va + npage*PGSIZE; page_va+=PGSIZE)
  {
    pte_t* ppte = walk(p->pagetable, page_va, 0);
    if(*ppte & PTE_A)   bitmap = bitmap | (1<<cnt); // 更改对应的位图
    *ppte = (*ppte)&clear_ptea; // clear PTE_A after checking
    cnt++;
  }

  return copyout(p->pagetable, buf_addr, (char*)&bitmap, sizeof(bitmap));
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
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
