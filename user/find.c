#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define DIRSIZ 14

struct dirent {
    ushort inum;
    char name[DIRSIZ];
};

void buscarArchivo(char *inicio, char *nombre) {
    char buf[512];
    char *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(inicio, 0)) < 0) {
        return;
    }

    if (fstat(fd, &st) < 0) {
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        close(fd);
        return;
    }

    if (strlen(inicio) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        close(fd);
        return;
    }

    strcpy(buf, inicio);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
            continue;

        if (stat(buf, &st) < 0)
            continue;

        if (strcmp(p, nombre) == 0) {
            printf("%s\n", buf);
        }

        if (st.type == T_DIR) {
            buscarArchivo(buf, nombre);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Uso: find <directorio> <nombre_archivo>\n");
        exit(1);
    }

    buscarArchivo(argv[1], argv[2]);
    exit(0);
}
