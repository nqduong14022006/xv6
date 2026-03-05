#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void tree(char *path, int depth)
{
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;

    if((fd = open(path,0))<0)
    {
        printf("tree: cannot open %s\n", path);
        return;
    }

    if(fstat(fd,&st)<0)
    {
        printf("tree: cannot stat %s\n",path);
        close(fd);
        return;
    }

    if(st.type != T_DIR)
    {
        printf("%s\n", path);
        close(fd);
        return;
    }

    while(read(fd,&de,sizeof(de))==sizeof(de))
    {
        if(de.inum==0) continue;
        
        if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0) continue;
        
        for(int i=0;i<depth;i++) printf("  ");
        
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++='/';
        strcpy(p,de.name);

        if(stat(buf,&st)<0)
        {
            printf("tree: cannot stat %s\n",buf);
            continue;
        }

        if(st.type==T_DIR)
        {
            printf("%s/\n",de.name);
            tree(buf, depth+1);
        }
        else printf("%s\n",de.name);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    char *path;

    if(argc==1) path=".";
    else path=argv[1];

    printf("%s/\n",path);
    tree(path,1);
    
    exit(0);
}