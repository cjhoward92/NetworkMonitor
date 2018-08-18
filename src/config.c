#include "config.h"
#include "utils.h"

static FILE * open_config_file(const char *path, const char *mode) {
  FILE *fd;
#ifdef WIN32
  fopen_s(&fd, path, mode);
#endif
#ifdef LINUX
  fd = fopen(path, mode);
#endif
  return fd;
}

static int write_config_tofile(const char *path, const cJSON *json) {
  FILE *fd = open_config_file(path, "w+");
  if (!fd)
    return NM_CONFIG_CANNOT_READ;

  char *cfgstr = cJSON_Print(json);
  fwrite(cfgstr, sizeof(char), strlen(cfgstr), fd);

  free(cfgstr);
  fclose(fd);

  return NM_CONFIG_OK;
}

static size_t read_config_fromfile(char **out, const char *path) {
  FILE *fd = open_config_file(path, "r");
  if (!fd)
    return 0;

  size_t sum = 0;
  size_t read = 0;
  char *file_contents = nm_alloc_string(MAX_BUFFER_SIZE);
  while ((read = fread(file_contents, sizeof(char), MAX_BUFFER_SIZE, fd)) > 0)
    sum += read;

  fclose(fd);

  *out = file_contents;

  return sum;
}

static cJSON * get_config_child(cJSON *json, const char *key) {
  if (!json || json->child)
    return NULL;

  cJSON *child = json->child;
  while (!strcmp(child->string, key)) {
    if (!child->next)
      return NULL;

    child = child->next;
  }

  return child;
}

static char * get_config_strvalue(cJSON *json, const char *key) {  
  cJSON *child = get_config_child(json, key);
  if (!child)
    return NULL;

  return child->valuestring;
}

static int get_config_intvalue(cJSON *json, const char *key) {  
  cJSON *child = get_config_child(json, key);
  if (!child)
    return 0;

  return child->valueint;
}

static int set_config_strvalue(cJSON *json, const char *key, char *value) {
  cJSON *child = get_config_child(json, key);
  if (!child)
    return NM_CONFIG_CANNOT_READ;

  child->valuestring = value;

  return NM_CONFIG_OK;
}

static int set_config_intvalue(cJSON *json, const char *key, int value) {
  cJSON *child = get_config_child(json, key);
  if (!child)
    return NM_CONFIG_CANNOT_READ;

  child->valueint = value;

  return NM_CONFIG_OK;
}

static cJSON * create_new_config_ondisk(const char *path) {
  cJSON *config_json = cJSON_CreateObject();
  if (!config_json)
    return NULL;

  cJSON *db_path = cJSON_CreateString("");
  if (db_path)
    cJSON_AddItemToObject(config_json, DB_PATH, db_path);

  write_config_tofile(path, config_json);
  return config_json;
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

  nm_config *cfg = nm_config_alloc();
  cfg->path = path;

  char *file_contents;
  size_t content_len = read_config_fromfile(&file_contents, path);
  
  int error = NM_CONFIG_OK;
  cJSON *config_json;
  if (!content_len) {
    config_json = create_new_config_ondisk(path);
  } else {
    config_json = cJSON_Parse(file_contents);
  }
  free(file_contents);

  if (!config_json) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  cfg->db_path = get_config_strvalue(config_json, DB_PATH);
  
  *out = cfg;

cleanup:
  if (config_json)
    cJSON_Delete(config_json);

  return error;
}

int nm_config_update(nm_config *cfg) {
  if (!cfg)
    return NM_CONFIG_NO_CONFIG;

  int error = NM_CONFIG_OK;
  char *file_contents;
  size_t content_len = read_config_fromfile(&file_contents, cfg->path);

  if (!content_len) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  cJSON *config_json = cJSON_Parse(file_contents);
  if (!config_json) {
    error = NM_CONFIG_CANNOT_READ;
    goto cleanup;
  }

  set_config_strvalue(config_json, DB_PATH, cfg->db_path);

  error = write_config_tofile(cfg->path, config_json);

cleanup:
  if (config_json)
    cJSON_Delete(config_json);

  free(file_contents);

  return error;
}

void nm_config_free(nm_config *cfg) {
  if (cfg->db_path)
    free(cfg->db_path);

  free(cfg);
}