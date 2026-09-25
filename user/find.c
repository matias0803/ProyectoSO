#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// limite de caracteres por nombre en xv6
#define DIRSIZ 14

// estructura de entrada de directorio en disco
struct dirent {
    ushort inum;
    char name[DIRSIZ];
};

// funcion recursiva para buscar archivos en subdirectorios
void buscarArchivo(char *inicio, char *nombre) {
    char buf[512];
    char *p;
    int fd;
    struct dirent de;
    struct stat st;

    // abrir ruta base
    if ((fd = open(inicio, 0)) < 0) {
        return;
    }

    // obtener informacion del descriptor
    if (fstat(fd, &st) < 0) {
        close(fd);
        return;
    }

    // solo recorrer si es un directorio
    if (st.type != T_DIR) {
        close(fd);
        return;
    }

    // evitar desbordar el buffer de la ruta
    if (strlen(inicio) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        close(fd);
        return;
    }

    // preparar prefijo de la ruta con separador
    strcpy(buf, inicio);
    p = buf + strlen(buf);
    *p++ = '/';

    // leer cada entrada del directorio
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        // saltar entradas vacias o borradas
        if (de.inum == 0)
            continue;

        // copiar nombre y asegurar terminador nulo
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        // ignorar referencias a si mismo y al padre
        if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
            continue;

        // leer estado del archivo o carpeta encontrada
        if (stat(buf, &st) < 0)
            continue;

        // imprimir si coincide el nombre
        if (strcmp(p, nombre) == 0) {
            printf("%s\n", buf);
        }

        // descender si encontramos una subcarpeta
        if (st.type == T_DIR) {
            buscarArchivo(buf, nombre);
        }
    }

    // liberar descriptor
    close(fd);
}

int main(int argc, char *argv[]) {
    // validar argumentos de entrada
    if (argc != 3) {
        fprintf(2, "Uso: find <directorio> <nombre_archivo>\n");
        exit(1);
    }

    // iniciar busqueda recursiva
    buscarArchivo(argv[1], argv[2]);
    exit(0);
}
