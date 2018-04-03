#include <stdio.h>
#include <sqlite3.h>

#ifdef WIN32
#include "win32/threading.h"
#include "win32/network.h"
#endif

#include "config.h"

//TODO: Cleanup so not windows dependent
// static DWORD WINAPI spawn_thread(LPVOID param) {
//   printf("We spawned a thread\n");
//   return 0;
// }

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "A database path is required\n");
    return 1;
  }

#ifdef WIN32
  printf("This is a windows build\n");
#endif

  // Testing DB
  // int error;
  // sqlite3 *db;
  // if ((error = sqlite3_open(argv[1], &db)) != 0) {
  //   const char *errmsg;
  //   errmsg = sqlite3_errstr(error);
  //   fprintf(stderr, errmsg);
  //   return error;
  // }

  // printf("We are open!\n");

  // error = sqlite3_close(db);
  // return error;

  // Testing threads
  // nm_thread_ctx *ctx = nm_thread_create(spawn_thread, NULL);
  // printf("created thread\n");
  // nm_thread_join(ctx);
  // printf("waited on thread\n");
  // nm_thread_free(ctx);
  // printf("freed thread\n");
  // return 0;

  // char cur_dir[1024];
  // GetCurrentDirectory(1024, cur_dir);
  // printf("%s\n", cur_dir);

  // const char *config_path = "config.json";
  // nm_config *cfg;
  // int error = nm_config_load(&cfg, config_path);
  // if (error) {
  //   fprintf(stderr, "Could not get config. Error: %d\n", error);
  // }

  // return error;

  int error = nm_network_startserver();
  return error;
}