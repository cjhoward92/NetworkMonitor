#ifndef __THREADING_H__
#define __THREADING_H__
#include <windows.h>
#include <stdlib.h>

#define MAX_THREADS 3

typedef struct nm_thread_ctx {
  HANDLE t_handle;
  LPDWORD t_id;
  void *data;
} nm_thread_ctx;

/* closes and frees the specified thread */
void nm_free_thread(nm_thread_ctx *ctx);

/* spawns a thread using the supplied function and parameter */
nm_thread_ctx * nm_create_thread(LPTHREAD_START_ROUTINE fn, void *param);

void nm_thread_join(const nm_thread_ctx *ctx);
#endif
