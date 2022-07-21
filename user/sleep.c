#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        write(2, "no input\n", strlen("no input\n"));
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}