#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

void find(char* path, char* file);

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(2,"ERROR: you should only path 2 arguments\n");
    }
    char *path = argv[1];
    char *file = argv[2];
    find(path, file);

    exit(0);
}


void find(char* path, char* file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
    
        if(de.inum ==0)
            continue;
    
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
      
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        
        switch (st.type)
        {
        case T_FILE:
            if(strcmp(file, de.name) == 0)
            {
                printf("%s\n", buf);
            }
            break;
        
        case T_DIR:
            if((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..")!=0) )
            {
                find(buf, file);
            }
            break;
        }
    }

    close(fd);
    return;
}