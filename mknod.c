#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char const *argv[])
{
    if(argc < 4){
        printf(2, "usage: mknod path major minor\n");
        exit();
    }
    const char* path = argv[1];
    short major = atoi(argv[2]);
    short minor = atoi(argv[3]);

    int res = mknod(path, major, minor);
    if(res == -1)
    {
        printf(2, "mknod failed\n");
        exit();
    }
    exit();
}
