
#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

int 
drvmemread(struct inode *ip, char *dst, int n, uint off)
{
    return 0;
}

int 
drvmemwrite(struct inode *ip, char *buf, int n, uint off)
{
    return 0;
}

void
drvmeminit(void)
{
    devsw[DNULL].read = drvmemread;
    devsw[DNULL].write = drvmemwrite;
}