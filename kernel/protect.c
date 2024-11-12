#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int mprotect(void *addr, int len) {
    struct proc *p = myproc();

    if ((uint64)addr % PGSIZE != 0 || len <= 0)
        return -1;

    uint64 start = (uint64)addr;
    uint64 end = start + len * PGSIZE;

    for (uint64 i = start; i < end; i += PGSIZE) {
        pte_t *pte = walk(p->pagetable, i, 0);
        if (pte == 0 || !(*pte & PTE_V)) {
            return -1;
        }
        *pte &= ~PTE_W;
    }
    return 0;
}

int munprotect(void *addr, int len) {
    struct proc *p = myproc();

    if ((uint64)addr % PGSIZE != 0 || len <= 0)
        return -1;

    uint64 start = (uint64)addr;
    uint64 end = start + len * PGSIZE;

    for (uint64 i = start; i < end; i += PGSIZE) {
        pte_t *pte = walk(p->pagetable, i, 0);
        if (pte == 0 || !(*pte & PTE_V)) {
            return -1;
        }
        *pte |= PTE_W;
    }
    return 0;
}
