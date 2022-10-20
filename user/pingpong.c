#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int pid;
    int fds1[2];
    int fds2[2];
    char buf[512];

    pipe(fds1);
    pipe(fds2);

    pid = fork();

    if(pid == 0){
        printf("child\n");
        read(fds1[0], buf, sizeof(buf));
        printf(buf); printf("\n");
        printf("%d: received ping\n", getpid());
        write(fds2[1], "pong\n",5);
    } else {
        printf("parent\n");
        write(fds1[1], "ping\n",5);
        read(fds2[0], buf, sizeof(buf));
        printf(buf); printf("\n");
        printf("%d: received pong\n", getpid());
        wait((int*) 0);
    }
  
    exit(0);
    
}