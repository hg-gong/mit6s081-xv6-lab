#include "kernel/types.h"
#include "user/user.h"

#define WRITEEND 1
#define READEND 0

int sieve(int * pr);

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    
    if(fork() == 0)
    {
        sieve(p);
    }else
    {
        close(p[READEND]); // cloes the file descriptors that process dont need
        for(int i = 2; i <= 35; i++)
        {
            write(p[WRITEEND], &i, sizeof(int));
        }
        close(p[WRITEEND]);
        wait((int *) 0); // wait for child process to finish
    }
    exit(0);
}

/*
sieve, read from the left neighborhood and push those not divide by first num into right neighborhood

[args]
int* pr, the pipe of the left neiborhood 
*/
int sieve(int *pr)
{
    int p[2];
    int n;

    close(pr[WRITEEND]);
    int read_result = read(pr[READEND], &n, sizeof(int)); // read returns zero when the write-side of a pipe is closed.
    if(read_result == 0) exit(0);

    pipe(p);
    
    if(fork() == 0)
    {
        sieve(p);
    }else
    {
        close(p[READEND]);
        printf("prime %d\n", n);
        int prime = n;
        while(read(pr[READEND], &n, sizeof(int)) != 0 )
        {
            if(n % prime != 0)
            {
                write(p[WRITEEND], &n, sizeof(int));
            }
        }
        close(p[WRITEEND]);
        wait((int *) 0);
        exit(0);
    }
    exit(0);

}