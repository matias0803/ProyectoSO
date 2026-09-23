#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void buscarArchivo(char* inicio, char* nombre){
    int fd;
    struct dirent de;
    struct stat st;
    char buf[512];
    char *p;

   
    fd = open(inicio, 0); 
    if (fd < 0) {
        return;
    }

    
    if (fstat(fd, &st) < 0) {
        close(fd);
        return;
    }

    
    if (st.type == T_DIR) {
        
        strcpy(buf, inicio);
        p = buf + strlen(buf);
        *p++ = '/'; 

        
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            
            
            if(de.inum == 0) continue;

            
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            
            if (strcmp(nombre, de.name) == 0) {
                printf("%s\n", buf);
            }

            
            if (stat(buf, &st) < 0) {
                continue;
            }

            
            if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                
                buscarArchivo(buf, nombre);
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
  if(argc != 3){
    fprintf(2, "Uso: find <ruta_inicial> <nombre_archivo>\n");
    exit(1);
  }
  buscarArchivo(argv[1], argv[2]);
  exit(0);
}
