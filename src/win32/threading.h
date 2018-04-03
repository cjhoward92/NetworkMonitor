#ifndef __THREADING_H__
#define __THREADING_H__

// Just leave these dumbass values in here... fucking windows, man.
// https://stackoverflow.com/questions/5971332/redefinition-errors-in-winsock2-h?lq=1
// https://developercommunity.visualstudio.com/content/problem/96399/with-153-update-c-projects-generate-type-redefinit.html
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_THREADS 3

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682516(v=vs.85).aspx
typedef struct nm_thread_ctx {
  HANDLE t_handle;
  DWORD t_id;
  void *data;
} nm_thread_ctx;

/* closes and frees the specified thread */
void nm_thread_free(nm_thread_ctx *ctx);

/* spawns a thread using the supplied function and parameter */
nm_thread_ctx * nm_thread_create(LPTHREAD_START_ROUTINE fn, void *param);

void nm_thread_join(const nm_thread_ctx *ctx);
#endif
