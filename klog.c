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

struct spinlock klogloc;

#define NEVENTS 16 // total of events

struct logev logev[NEVENTS] = {0};
uint logevempty;

int 
klogread(struct inode *ip, char *dst, int n)
{
    if(ip->major == KLOG)
    {
        ilock(ip);

        uint total_asked = n / sizeof(struct logev);
        if (total_asked == 0)
        {
            iunlock(ip);
            return 0;
        }

        // if buffer is empty, wait until one event appeared
        // active wating, didn't know how to do in another way at this moment
        // needs to be changed !
        while(logevempty == 0)
            ;
        
        for(uint i = 0; i < total_asked; i++)
            memmove(&logev[i % NEVENTS], dst, sizeof(struct logev));
            
        iunlock(ip);
        return sizeof(struct logev) * total_asked;
    }
    return 0;
}
int 
klogwrite(struct inode *ip, char *buf, int n)
{
    if(ip->major == KLOG)
    {
        ilock(ip);
        acquire(&klogloc);
        uint total_asked = n / sizeof(struct logev);
        if (n == 0) // based on the fac that uint trunc value and if it is < 1
                    // then it will be 0
        {
            iunlock(ip);
            return 0;
        }
        
        // dont handle when user ask not a multiple of sizeof logev

        for(uint i = 0; i < total_asked; i++)
        {
            struct logev curr_log = {0};
            memmove(&curr_log, buf, sizeof(int));
            memmove(&curr_log, buf + sizeof(int), MAX_SENDER);
            memmove(&curr_log, buf + sizeof(int) + MAX_SENDER, MAX_EVENT);
            memmove(&logev[i % NEVENTS], &curr_log, sizeof(struct logev));
            if(logevempty == 0)
                logevempty = 1;
        }

        release(&klogloc);
        iunlock(ip);
        return n;
    }
    return 0;
}


int klogopen(struct inode *in, int omode)
{
    return 0;
}

void
klogclose(struct inode *in, struct file* f)
{

}

void
kloginit(void)
{
    // initlock(&klogloc, "klog");
    devsw[KLOG].read = klogread;
    devsw[KLOG].write = klogwrite;
    devsw[KLOG].open = klogopen;
    devsw[KLOG].close = klogclose;
    logevempty = 0;
}