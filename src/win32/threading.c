#include "threading.h"

static int t_count = 0;

void nm_free_thread(nm_thread_ctx *ctx) {
  if (ctx->t_handle)
    CloseHandle(ctx->t_handle);

  if (ctx->data)
    free(ctx->data);
  
  free(ctx);
  t_count--;
}

nm_thread_ctx * nm_create_thread(LPTHREAD_START_ROUTINE fn, void *param) {
  if (t_count == MAX_THREADS) {
    return NULL;
  }
  
  nm_thread_ctx *ctx = (nm_thread_ctx *)malloc(sizeof(nm_thread_ctx));

  ctx->data = param;
  ctx->t_handle = CreateThread(
    NULL,
    0,
    fn,
    ctx->data,
    0,
    ctx->t_id
  );

  if (!ctx->t_handle) {
    nm_free_thread(ctx);
    return NULL;
  }

  t_count++;
  return ctx;
}

void nm_thread_join(const nm_thread_ctx *ctx) {
  WaitForSingleObject(ctx->t_handle, INFINITE);
}