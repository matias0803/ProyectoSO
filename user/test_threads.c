#include "kernel/types.h"
#include "user/user.h"
#include "thread.h"


void rutina_hebra(void) {

    int id_actual = current_thread->id;
    for (int i = 0; i < 5; i++) {
        printf("Hebra %d: Iteracion %d\n", id_actual, i);
        thread_yield();
    }

    printf("Hebra %d: Finalizada.\n", id_actual);
    current_thread->state = LIBRE;
    thread_schedule();
}

int main(int argc, char *argv[]) {

    printf("Iniciando prueba de Hebras\n");
    thread_init();
    thread_create(rutina_hebra);
    thread_create(rutina_hebra);
    thread_create(rutina_hebra);
    thread_create(rutina_hebra);
    thread_create(rutina_hebra);
    thread_create(rutina_hebra);

    current_thread->state = LIBRE;
    thread_schedule();
    exit(0);
} 
