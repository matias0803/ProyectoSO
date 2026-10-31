#include "kernel/types.h"
#include "user/user.h"
#include "thread.h"

struct thread threads[MAX_THREADS];
struct thread *current_thread = 0;

void thread_init(void) {
  for (int i = 0; i < MAX_THREADS; i++) {
    threads[i].id = i;
    threads[i].state = LIBRE;
  }

  current_thread = &threads[0];
  current_thread->state = CORRIENDO;
}

int thread_create(void (*func)(void)) {
  struct thread *t = 0;

  for (int i = 0; i < MAX_THREADS; i++) {
    if (threads[i].state == LIBRE) {
      t = &threads[i];
      break;
    }
  }

  if (t == 0) {
    return -1;
  }

  t->state = LISTA_PARA_CORRER;

  uint64 stack_top = (uint64)&t->stack[STACK_SIZE];
  stack_top = stack_top & ~15LL;

  t->context.sp = stack_top;
  t->context.ra = (uint64)func;

  return t->id;
}

void thread_schedule(void) {
  // cambio Guardar puntero a la hebra saliente ANTES de buscar
  struct thread *old_thread = current_thread;
  struct thread *new_thread = 0;

  // [CAMBIO 2]: Inicio circular (Round-Robin) desde la siguiente hebra (id + 1)
  int start = (old_thread != 0) ? (old_thread->id + 1) % MAX_THREADS : 0;
  for (int i = 0; i < MAX_THREADS; i++) {
    int idx = (start + i) % MAX_THREADS; // [CAMBIO 2]: Índice con módulo
    if (threads[idx].state == LISTA_PARA_CORRER) {
      new_thread = &threads[idx];
      break;
    }
  }

  // cambio Salir con exit(0) si no quedan hebras pendientes
  if (new_thread == 0) {
    if (old_thread && old_thread->state == CORRIENDO) return;
    exit(0);
  }

  // cambio Actualizar estado y puntero global ANTES de saltar en ensamblador
  new_thread->state = CORRIENDO;
  current_thread = new_thread;

  // cambio Usar old_thread para no perder la referencia de la hebra saliente
  thread_switch(&old_thread->context, &new_thread->context);
}

void thread_yield(void) {
  // cambio Marcar estado a LISTA_PARA_CORRER ANTES de conmutar
  if (current_thread != 0 && current_thread->state == CORRIENDO) {
    current_thread->state = LISTA_PARA_CORRER;
  }

  // llamar a schedule para que active otra hebra
  thread_schedule();

  // cambio: Se eliminó el bucle posterior (no se ejecutaba a tiempo por el congelamiento)
}
