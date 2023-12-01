#include "syslog.h"

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
klogread(struct inode *ip, char *dst, int n)
{
    return 0;
}
int 
klogwrite(struct inode *ip, char *buf, int n)
{
    return 0;
}

void
kloginit(void)
{
    devsw[KLOG].read = klogread;
    devsw[KLOG].write = klogwrite;
}