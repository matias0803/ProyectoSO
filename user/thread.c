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

void thread_schedule(int old_thread_id) {
  struct thread *new_thread = 0;

  // buscar una hebra que esté lista para correr
  for (int i = 0; i < MAX_THREADS; i++) {
    if (threads[i].state == LISTA_PARA_CORRER) {
      new_thread = &threads[i];
      break;
    }
  }

  // si no se encuentra una hebra, retornar
  if (new_thread == 0) return;

  struct thread *old_thread = 0;

  // buscar la hebra que se quiere reemplazar
  // se busca por id
  for (int i = 0; i < MAX_THREADS; i++) {
    if (threads[i].id == old_thread_id) {
      old_thread = &threads[i];
      break;
    }
  }

  // si no se encuentra la hebra, retornar
  if (old_thread == 0) return;

  // cambio de contexto
  thread_switch(old_thread->context, new_thread->context);

  // cambiar estado de nueva hebra a CORRIENDO
  new_thread->state = CORRIENDO;
}