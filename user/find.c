#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

char *name(char *path) {
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  return p+1;
}

void find(char* path,char* target){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    fd = open(path, 0);
    fstat(fd,&st);
    switch(st.type){
        case T_FILE:
            if(strcmp(name(path),target)==0){
                printf("%s\n",path);
                close(fd);
                return ;
            }break;
        case T_DIR:
            if(strcmp(name(path), target) == 0) {
                printf("%s\n", path);
            } 
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)){
                if (de.inum == 0) continue;
                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
                int len = 0;
                while(len < DIRSIZ && de.name[len] != '\0') len++;
                memmove(p, de.name, len);
                p[len] = 0;
                find(buf,target);
            }break; 
    }
    close(fd);
}

int main(int argc,char* argv[]){
    char* dir_name = argv[1];
    char* target = argv[2];
    find(dir_name,target);
    exit(0);
}