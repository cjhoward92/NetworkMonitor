#include "config.h"
#include "utils.h"

static FILE * open_config_file(const char *path, const char *mode) {
  FILE *fd;
  fopen_s(&fd, path, mode);
  return fd;
}

static int write_config_tofile(FILE *fd, const cJSON *json) {
  char *cfgstr = cJSON_Print(json);
  int error = fputs(cfgstr, fd);
  free(cfgstr);
  return error;
}

nm_config * nm_config_alloc() {
  nm_config *cfg = (nm_config *)malloc(sizeof(nm_config));
  cfg->db_path = NULL;
  cfg->path = NULL;
  return cfg;
}

int nm_config_load(nm_config **out, const char *path) {
  *out = NULL;
  if (!path) {
    return NM_CONFIG_INVALID_PATH;
  }

  FILE *fd = open_config_file(path, "w+");
  if (!fd)
    return NM_CONFIG_CANNOT_READ;

  nm_config *cfg = nm_config_alloc();
  cfg->path = path;

  char *file_contents = nm_alloc_string(MAX_BUFFER_SIZE);

  int error = NM_CONFIG_OK;
  cJSON *config_json;
  cJSON *db_path;
  if (!fread(file_contents, sizeof(char), MAX_BUFFER_SIZE, fd)) {
    config_json = cJSON_CreateObject();
    db_path = cJSON_CreateString("");
    cJSON_AddItemToObject(config_json, DB_PATH, db_path);
    if ((error = write_config_tofile(fd, config_json)) != 0)
      goto cleanup;
  } else {
    config_json = cJSON_Parse(file_contents);
  }

  free(file_contents);

  if (!config_json) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  if (config_json->child) {
    while (
      config_json->child->next
      && config_json->child->string
      && !strcmp(config_json->child->string, DB_PATH)
    )
      config_json->child = config_json->child->next;

    if (config_json->child->valuestring)
      cfg->db_path = _strdup(config_json->valuestring);
  }
  
  *out = cfg;

cleanup:
  if (config_json)
    cJSON_Delete(config_json);

  fclose(fd);
  return error;
}

int nm_config_update(nm_config *cfg) {
  if (!cfg)
    return NM_CONFIG_NO_CONFIG;

  FILE *fd = open_config_file(cfg->path, "w+");
  if (!fd)
    return NM_CONFIG_CANNOT_READ;

  int error = NM_CONFIG_OK;
  char *file_contents = nm_alloc_string(MAX_BUFFER_SIZE);

  if (!fread(file_contents, sizeof(char), MAX_BUFFER_SIZE, fd)) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  cJSON *config_json = cJSON_Parse(file_contents);
  if (!config_json) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  while(!strcmp(config_json->string, DB_PATH))
    config_json = config_json->next;
  config_json->valuestring = cfg->db_path;

  error = write_config_tofile(fd, config_json);

cleanup:
  if (config_json)
    cJSON_Delete(config_json);

  free(file_contents);
  fclose(fd);

  return error;
}

void nm_config_free(nm_config *cfg) {
  if (cfg->db_path)
    free(cfg->db_path);

  free(cfg);
}