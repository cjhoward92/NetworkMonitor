#include "config.h"

nm_config * nm_config_create(const char *path) {
  if (!path)
    return NULL;

  FILE *fd;
  int error;
  if ((error = fopen_s(&fd, path, "w+") != 0)
    return NULL;

  nm_config *cfg = (nm_config *)malloc(sizeof(nm_config));
  cfg->path = path;
  cfg->db_path = "";

  cJSON *config = cJSON_CreateObject();
  if (!config)
    goto out;

  cJSON *db_path = cJSON_CreateString("");
  cJSON_AddItemToObject(config, "dbPath", db_path);
  
  char *cfgstr = cJSON_Print(config);
  fputs(cfgstr, fd);

  cJSON_Delete(db_path);
  cJSON_Delete(config);

out:
  fclose(fd);
  return cfg;
}

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