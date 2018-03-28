#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "A database path is required\n");
    return 1;
  }

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
}