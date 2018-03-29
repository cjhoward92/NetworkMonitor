#include <stdio.h>
#include <sqlite3.h>

#ifdef WIN32
#include "win32/threading.h"
#endif

static DWORD WINAPI spawn_thread(LPVOID param) {
  printf("We spawned a thread\n");
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "A database path is required\n");
    return 1;
  }

#ifdef WIN32
  printf("This is a windows build\n");
#endif

#ifdef DB_TEST
  int error;
  sqlite3 *db;
  if ((error = sqlite3_open(argv[1], &db)) != 0) {
    const char *errmsg;
    errmsg = sqlite3_errstr(error);
    fprintf(stderr, errmsg);
    return error;
  }

  printf("We are open!\n");

  error = sqlite3_close(db);
  return error;
#endif
#ifndef DB_TEST
  nm_thread_ctx *ctx = nm_create_thread(spawn_thread, NULL);
  nm_thread_join(ctx);
  return 0;
#endif
}