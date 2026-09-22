#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



char* buscarArchivo(char* inicio, char* nombre){
    int fd;
    struct dirent de;
    char *p;
    char buf[512];


    fd = open(inicio, O_RDONLY);

    if (fd < 0) {
        return 0;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){

        if (strcmp(nombre, de.name) == 0) {

            strcpy(buf, inicio);

            p = buf + strlen(buf);
            *p++ = '/';

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            close(fd);
            return buf;
        }
        
    }
    close(fd);
    return 0;

}