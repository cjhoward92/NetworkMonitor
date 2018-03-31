#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

#define DB_PATH "dbPath"

typedef enum {
  NM_CONFIG_OK = 0,
  NM_CONFIG_INVALID_PATH = 1,
  NM_CONFIG_CANNOT_READ = 2,
  NM_CONFIG_NO_CONFIG = 3
} nm_config_error;

typedef struct nm_config {
  const char *path;
  char *db_path;
} nm_config;

nm_config * nm_config_alloc();

int nm_config_load(nm_config **out, const char *path);

int nm_config_update(nm_config *cfg);

void nm_config_free(nm_config *cfg);
#endif