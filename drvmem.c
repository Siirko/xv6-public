
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
    if(ip->major == DNULL)
    {
        switch (ip->minor)
        {
           case 0:
                return 0;
            case 1:
                memset(dst, 0, n);
                return n;
            case 2:
                if (off < EXTMEM || off + n > PHYSTOP)
                    return -1;
            default:
                return -1;
        }
    }
    return 0;
}

int 
drvmemwrite(struct inode *ip, char *buf, int n, uint off)
{
    if (ip->major == 2)
    {
        switch (ip->minor)
        {
            case 0:
                return n;
            case 1:
                return n;
            default:
                return -1;
        }
    }
    return -1;
}

void
drvmeminit(void)
{
    devsw[DNULL].read = drvmemread;
    devsw[DNULL].write = drvmemwrite;

    // ioapicenable(IRQ_KBD, 0);
}