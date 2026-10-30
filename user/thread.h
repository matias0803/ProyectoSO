#ifndef THREAD_H
#define THREAD_H

#include "kernel/types.h"

#define STACK_SIZE 4096
#define MAX_THREADS 8

enum thread_state {
  LIBRE,
  LISTA_PARA_CORRER,
  CORRIENDO
};

struct context {
  uint64 ra;
  uint64 sp;
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

struct thread {
  int id;
  enum thread_state state;
  struct context context;
  char stack[STACK_SIZE];
};

extern struct thread *current_thread;

void thread_init(void);
int thread_create(void (*func)(void));
void thread_schedule(int old_thread_id);
void thread_yield(void);

extern void thread_switch(struct context *old_ctx, struct context *new_ctx);

#endif
