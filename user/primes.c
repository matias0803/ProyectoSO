#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p_in[2]) {
    int prime;
    int curr_in = p_in[0];

    // Bucle iterativo que reemplaza la llamada recursiva directa
    while (1) {
        // Leer el primer número de la tubería entrante
        if (read(curr_in, &prime, sizeof(int)) <= 0) {
            close(curr_in);
            exit(0);
        }

        // El primer número leído siempre es primo
        printf("prime %d\n", prime);

        int p_out[2];
        pipe(p_out);

        int pid = fork();
        if (pid < 0) {
            fprintf(2, "Error al crear proceso con fork\n");
            close(curr_in);
            close(p_out[0]);
            close(p_out[1]);
            exit(1);
        }

        if (pid == 0) {
            // Proceso hijo: hereda el nuevo pipe y continúa la iteración
            close(curr_in);   // Ya no necesita la tubería previa
            close(p_out[1]);  // Solo leerá de la nueva tubería
            curr_in = p_out[0];
            // Repite el ciclo como un nuevo filtro
        } else {
            // Proceso padre actual: filtra los números hacia el nuevo hijo
            close(p_out[0]);

            int num;
            while (read(curr_in, &num, sizeof(int)) > 0) {
                // Solo reenviar números no divisibles
                if (num % prime != 0) {
                    write(p_out[1], &num, sizeof(int));
                }
            }

            // Fin del flujo de datos
            close(curr_in);
            close(p_out[1]);
            wait(0);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Error en fork inicial\n");
        exit(1);
    }

    if (pid == 0) {
        // Iniciar la criba con el primer hijo
        close(p[1]);
        primes(p);
    } else {
        // Padre generador: envía números del 2 al 35
        close(p[0]);

        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }

        close(p[1]); // Avisa el fin de datos al primer filtro
        wait(0);
    }

    exit(0);
}
