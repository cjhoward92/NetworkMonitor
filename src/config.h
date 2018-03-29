#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct nm_config {
  const char *path;
  const char *db_path;
} nm_config;

int nm_config_load(nm_config **out, const char *path);

int nm_config_update(nm_config *cfg);

void nm_config_free(nm_config *cfg);
#endif