#include "config.h"

int nm_config_load(nm_config **out, const char *path) {
  if (!path) {
    (*out) = NULL;
    return 1;
  }

  nm_config *cfg;
  //TDOD: need a parsing library or do it myself. Thinking JSON makes sense. See cJSON
}

int nm_config_update(nm_config *cfg) {
  return 0;
}

void nm_config_free(nm_config *cfg) {
  free(cfg);
}