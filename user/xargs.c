#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *args[MAXARG];
    int i;
    int base=argc-1;

    for(i=1;i<argc;i++) args[i-1]=argv[i];
    int n=0;
    char c;

    while(read(0,&c,1)==1)
    {
        if(c=='\n')
        {
            buf[n]=0;
            args[base]=buf;
            args[base+1]=0;
            
            if(fork()==0)
            {
                exec(args[0],args);
                exit(1);
            }
            else wait(0);

            n=0;
        }
        else buf[n++]=c;
    }
    exit(0);
}