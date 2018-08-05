#include "threading.h"

/* closes and frees the specified thread */
void nm_thread_free(nm_thread_ctx *ctx) {
  free(ctx);
};

/* spawns a thread using the supplied function and parameter */
nm_thread_ctx * nm_thread_create(nm_thread_fn fn, void *param) {
  nm_thread_ctx *ctx = malloc(sizeof(nm_thread_ctx));
  ctx->t_id = 0;
  ctx->t_handle = 0;
  ctx->data = param;

  pthread_create(&ctx->t_id, NULL, fn, param);

  ctx->t_handle = ctx->t_id;

  return ctx;
};

void nm_thread_join(const nm_thread_ctx *ctx) {
  pthread_join(ctx->t_id, NULL);
};
