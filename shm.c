#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {
//you write this
	int i; 
	acquire(&(shm_table.lock), "SHM lock");
///CS153 lab4 work: case 1
	for(i = 0; i < 64; ++i) {
		if(shm_table_pages[i] == id) {
			//we are not the first to open, remap and check for errors
			if(mappages(myproc()->pgdir, (char*)PGROUNDUP(myproc()->sz), V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U) == -1) {
				release(&(sh_table.lock));
				return -1;
			}
			//exists and no error, inc refcnt, return pointer to va, update sz
			shm_table.shm_pages[i].refcnt += 1;
			*pointer = (char*)PGROUNDUP(myproc()->sz);
			myproc()->sz += PGSIZE;
			release(&(shm_table.lock));
			return 0;
		}
	//else we searched for the id, and it is not already in our table
	}

//CS153 lab4 work: case 2
	for(i = 0; i < 64; ++i) {
		if(shm_table.shm_pages[i].id == 0) {
			shm_table.shm_pages[i].id == id;
			if((shm_table.shm_pages[i].frame = kalloc()) == 0) {
				release(&(shm_table.lock));
				return -1;
			}
			memset(shm_table.shm_pages[i].frame, 0, PGSIZE);
			shm_table.shm_pages[i].refcnt = 1;
			if(mappages(myproc()->pgdir, (char*)PGROUNDUP(myproc()->sz), PGSIZE, V2P(shm_table.shmpages[i].frame), PTE_W|PTE_U) == -1) {
				release(&(shm_table.lock));
				return -1;
			}
			*pointer = (char*)PGROUNDUP(myproc()->sz);
			myproc()->sz += PGSIZE;
			release(&(shm_table.lock));
			return 0;
		}

	}
	release(&(shm_table.lock));
	return -1; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!
   int i;
   acquire(&(shm_table.lock));
   for(i = 0; i < 64; ++i) {
	if(shm_table_pages[i].id == id) {
	  shm_table.shm_pages[i].refcnt -= 1;
	  if(shm_table.shm_pages[i].refcnt <= 0) {
		shm_table.shm_pages[i].id = 0;
		shm_table.shm_pages[i].frame = 0;
		shm_table.shm_pages[i].refcnt = 0;
	  }
	  release(&(shm_table.lock));
	  return 0;
	}
   }
   release(&(shm_table.lock));
   return -1;
}

