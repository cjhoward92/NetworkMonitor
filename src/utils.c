#include "utils.h"

char * nm_alloc_string(size_t size) {
  if (size > MAX_BUFFER_SIZE)
    return NULL;
  
  char *str = malloc(sizeof(char) * size);
  memset(str, 0, size);

  return str;
}
