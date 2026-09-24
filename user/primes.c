#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// función usada por los procesos hijos para buscar primos
void buscarPrimosHijo(int* pIn) {
    // pIn: pipe por la que el padre envía números al hijo
    close(pIn[1]); // no vamos a escribir a pIn

    // leer número inicial desde pipe de entrada
    // cancelar función si no pueden llegar más números
    int number;
    if (read(pIn[0], &number, sizeof(int)) == 0) return;
    // TODO: imprimir number en terminal

    int pOut[2]; // nueva pipe para mandar números al nuevo hijo
    // hacer fork para mandar números a otro hijo
    int pid = fork();
    if (pid > 0) {
        // proceso actual, ahora padre
        close(pOut[0]); // cerrar escritura del nuevo pipe para este proceso
        int currNumber; // para cada número que llegue desde pIn
        // loop: por cada número que llega desde pIn:
        while (read(pIn[0], &currNumber, sizeof(int)) != 0) {
            if (currNumber % number == 0) {
                // el número actual es múltiplo del número inicial
                // enviar a proceso hijo
                write(pOut[1], &currNumber, sizeof(int));
            }
            // si no es múltiplo, se ignora el número
        }
        // terminado el loop
        close(pIn[0]); // no hay más números que leer
        close(pOut[1]); // no hay más números que enviar
        wait((int*) 0); // esperar a que el hijo muera
    } else if (pid == 0) {
        // nuevo proceso hijo
        // ejecutar función recursivamente
        buscarPrimosHijo(pOut);
        close(pIn[0]);
        close(pOut[0]);
        close(pOut[1]);
        exit(0);
    }
}

void buscarPrimos() {
    int number; // número actual
    int p[2]; // pipe

    pipe(p); // crear pipe en p

    int pid = fork();
    if (pid > 0) {
        // proceso padre original
        close(p[0]); // cerrar lectura de p, no vamos a leer nada
        // loop: por cada número de 2 a 35
        for (number = 2; number = 35; number++) {
            write(p[1], &number, sizeof(int)); // enviar por pipe p
        }
    } else if (pid == 0) {
        // proceso hijo inicial
        close(p[1]); // cerrar escritura de p, no vamos a escribir nada
        

        int p_new[2]; // nueva p
    }
}