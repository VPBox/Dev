/******************************************************************************
 *
 *  Copyright 2017 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include "osi/include/config.h"

#include <base/files/file_util.h>
#include <base/logging.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <log/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <type_traits>

// Empty definition; this type is aliased to list_node_t.
struct config_section_iter_t {};

static bool config_parse(FILE* fp, config_t* config);

template <typename T,
          class = typename std::enable_if<std::is_same<
              config_t, typename std::remove_const<T>::type>::value>>
static auto section_find(T& config, const std::string& section) {
  return std::find_if(
      config.sections.begin(), config.sections.end(),
      [&section](const section_t& sec) { return sec.name == section; });
}

static const entry_t* entry_find(const config_t& config,
                                 const std::string& section,
                                 const std::string& key) {
  auto sec = section_find(config, section);
  if (sec == config.sections.end()) return nullptr;

  for (const entry_t& entry : sec->entries) {
    if (entry.key == key) return &entry;
  }

  return nullptr;
}

std::unique_ptr<config_t> config_new_empty(void) {
  return std::make_unique<config_t>();
}

std::unique_ptr<config_t> config_new(const char* filename) {
  CHECK(filename != nullptr);

  std::unique_ptr<config_t> config = config_new_empty();

  FILE* fp = fopen(filename, "rt");
  if (!fp) {
    LOG(ERROR) << __func__ << ": unable to open file '" << filename
               << "': " << strerror(errno);
    return nullptr;
  }

  if (!config_parse(fp, config.get())) {
    config.reset();
  }

  fclose(fp);
  return config;
}

std::string checksum_read(const char* filename) {
  base::FilePath path(filename);
  if (!base::PathExists(path)) {
    LOG(ERROR) << __func__ << ": unable to locate file '" << filename << "'";
    return "";
  }
  std::string encrypted_hash;
  if (!base::ReadFileToString(path, &encrypted_hash)) {
    LOG(ERROR) << __func__ << ": unable to read file '" << filename << "'";
  }
  return encrypted_hash;
}

std::unique_ptr<config_t> config_new_clone(const config_t& src) {
  std::unique_ptr<config_t> ret = config_new_empty();

  for (const section_t& sec : src.sections) {
    for (const entry_t& entry : sec.entries) {
      config_set_string(ret.get(), sec.name, entry.key, entry.value);
    }
  }

  return ret;
}

bool config_has_section(const config_t& config, const std::string& section) {
  return (section_find(config, section) != config.sections.end());
}

bool config_has_key(const config_t& config, const std::string& section,
                    const std::string& key) {
  return (entry_find(config, section, key) != nullptr);
}

int config_get_int(const config_t& config, const std::string& section,
                   const std::string& key, int def_value) {
  const entry_t* entry = entry_find(config, section, key);
  if (!entry) return def_value;

  char* endptr;
  int ret = strtol(entry->value.c_str(), &endptr, 0);
  return (*endptr == '\0') ? ret : def_value;
}

uint64_t config_get_uint64(const config_t& config, const std::string& section,
                           const std::string& key, uint64_t def_value) {
  const entry_t* entry = entry_find(config, section, key);
  if (!entry) return def_value;

  char* endptr;
  uint64_t ret = strtoull(entry->value.c_str(), &endptr, 0);
  return (*endptr == '\0') ? ret : def_value;
}

bool config_get_bool(const config_t& config, const std::string& section,
                     const std::string& key, bool def_value) {
  const entry_t* entry = entry_find(config, section, key);
  if (!entry) return def_value;

  if (entry->value == "true") return true;
  if (entry->value == "false") return false;

  return def_value;
}

const std::string* config_get_string(const config_t& config,
                                     const std::string& section,
                                     const std::string& key,
                                     const std::string* def_value) {
  const entry_t* entry = entry_find(config, section, key);
  if (!entry) return def_value;

  return &entry->value;
}

void config_set_int(config_t* config, const std::string& section,
                    const std::string& key, int value) {
  config_set_string(config, section, key, std::to_string(value));
}

void config_set_uint64(config_t* config, const std::string& section,
                       const std::string& key, uint64_t value) {
  config_set_string(config, section, key, std::to_string(value));
}

void config_set_bool(config_t* config, const std::string& section,
                     const std::string& key, bool value) {
  config_set_string(config, section, key, value ? "true" : "false");
}

void config_set_string(config_t* config, const std::string& section,
                       const std::string& key, const std::string& value) {
  CHECK(config);

  auto sec = section_find(*config, section);
  if (sec == config->sections.end()) {
    config->sections.emplace_back(section_t{.name = section});
    sec = std::prev(config->sections.end());
  }

  std::string value_no_newline;
  size_t newline_position = value.find('\n');
  if (newline_position != std::string::npos) {
    android_errorWriteLog(0x534e4554, "70808273");
    value_no_newline = value.substr(0, newline_position);
  } else {
    value_no_newline = value;
  }

  for (entry_t& entry : sec->entries) {
    if (entry.key == key) {
      entry.value = value_no_newline;
      return;
    }
  }

  sec->entries.emplace_back(entry_t{.key = key, .value = value_no_newline});
}

bool config_remove_section(config_t* config, const std::string& section) {
  CHECK(config);

  auto sec = section_find(*config, section);
  if (sec == config->sections.end()) return false;

  config->sections.erase(sec);
  return true;
}

bool config_remove_key(config_t* config, const std::string& section,
                       const std::string& key) {
  CHECK(config);
  auto sec = section_find(*config, section);
  if (sec == config->sections.end()) return false;

  for (auto entry = sec->entries.begin(); entry != sec->entries.end();
       ++entry) {
    if (entry->key == key) {
      sec->entries.erase(entry);
      return true;
    }
  }

  return false;
}

bool config_save(const config_t& config, const std::string& filename) {
  CHECK(!filename.empty());

  // Steps to ensure content of config file gets to disk:
  //
  // 1) Open and write to temp file (e.g. bt_config.conf.new).
  // 2) Flush the stream buffer to the temp file.
  // 3) Sync the temp file to disk with fsync().
  // 4) Rename temp file to actual config file (e.g. bt_config.conf).
  //    This ensures atomic update.
  // 5) Sync directory that has the conf file with fsync().
  //    This ensures directory entries are up-to-date.
  int dir_fd = -1;
  FILE* fp = nullptr;
  std::stringstream serialized;

  // Build temp config file based on config file (e.g. bt_config.conf.new).
  const std::string temp_filename = filename + ".new";

  // Extract directory from file path (e.g. /data/misc/bluedroid).
  const std::string directoryname = base::FilePath(filename).DirName().value();
  if (directoryname.empty()) {
    LOG(ERROR) << __func__ << ": error extracting directory from '" << filename
               << "': " << strerror(errno);
    goto error;
  }

  dir_fd = open(directoryname.c_str(), O_RDONLY);
  if (dir_fd < 0) {
    LOG(ERROR) << __func__ << ": unable to open dir '" << directoryname
               << "': " << strerror(errno);
    goto error;
  }

  fp = fopen(temp_filename.c_str(), "wt");
  if (!fp) {
    LOG(ERROR) << __func__ << ": unable to write to file '" << temp_filename
               << "': " << strerror(errno);
    goto error;
  }

  for (const section_t& section : config.sections) {
    serialized << "[" << section.name << "]" << std::endl;

    for (const entry_t& entry : section.entries)
      serialized << entry.key << " = " << entry.value << std::endl;

    serialized << std::endl;
  }

  if (fprintf(fp, "%s", serialized.str().c_str()) < 0) {
    LOG(ERROR) << __func__ << ": unable to write to file '" << temp_filename
               << "': " << strerror(errno);
    goto error;
  }

  // Flush the stream buffer to the temp file.
  if (fflush(fp) < 0) {
    LOG(ERROR) << __func__ << ": unable to write flush buffer to file '"
               << temp_filename << "': " << strerror(errno);
    goto error;
  }

  // Sync written temp file out to disk. fsync() is blocking until data makes it
  // to disk.
  if (fsync(fileno(fp)) < 0) {
    LOG(WARNING) << __func__ << ": unable to fsync file '" << temp_filename
                 << "': " << strerror(errno);
  }

  if (fclose(fp) == EOF) {
    LOG(ERROR) << __func__ << ": unable to close file '" << temp_filename
               << "': " << strerror(errno);
    goto error;
  }
  fp = nullptr;

  // Change the file's permissions to Read/Write by User and Group
  if (chmod(temp_filename.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) ==
      -1) {
    LOG(ERROR) << __func__ << ": unable to change file permissions '"
               << filename << "': " << strerror(errno);
    goto error;
  }

  // Rename written temp file to the actual config file.
  if (rename(temp_filename.c_str(), filename.c_str()) == -1) {
    LOG(ERROR) << __func__ << ": unable to commit file '" << filename
               << "': " << strerror(errno);
    goto error;
  }

  // This should ensure the directory is updated as well.
  if (fsync(dir_fd) < 0) {
    LOG(WARNING) << __func__ << ": unable to fsync dir '" << directoryname
                 << "': " << strerror(errno);
  }

  if (close(dir_fd) < 0) {
    LOG(ERROR) << __func__ << ": unable to close dir '" << directoryname
               << "': " << strerror(errno);
    goto error;
  }

  return true;

error:
  // This indicates there is a write issue.  Unlink as partial data is not
  // acceptable.
  unlink(temp_filename.c_str());
  if (fp) fclose(fp);
  if (dir_fd != -1) close(dir_fd);
  return false;
}

bool checksum_save(const std::string& checksum, const std::string& filename) {
  CHECK(!checksum.empty()) << __func__ << ": checksum cannot be empty";
  CHECK(!filename.empty()) << __func__ << ": filename cannot be empty";

  // Steps to ensure content of config checksum file gets to disk:
  //
  // 1) Open and write to temp file (e.g.
  // bt_config.conf.encrypted-checksum.new). 2) Sync the temp file to disk with
  // fsync(). 3) Rename temp file to actual config checksum file (e.g.
  // bt_config.conf.encrypted-checksum).
  //    This ensures atomic update.
  // 4) Sync directory that has the conf file with fsync().
  //    This ensures directory entries are up-to-date.
  FILE* fp = nullptr;
  int dir_fd = -1;

  // Build temp config checksum file based on config checksum file (e.g.
  // bt_config.conf.encrypted-checksum.new).
  const std::string temp_filename = filename + ".new";
  base::FilePath path(temp_filename);

  // Extract directory from file path (e.g. /data/misc/bluedroid).
  const std::string directoryname = base::FilePath(filename).DirName().value();
  if (directoryname.empty()) {
    LOG(ERROR) << __func__ << ": error extracting directory from '" << filename
               << "': " << strerror(errno);
    goto error2;
  }

  dir_fd = open(directoryname.c_str(), O_RDONLY);
  if (dir_fd < 0) {
    LOG(ERROR) << __func__ << ": unable to open dir '" << directoryname
               << "': " << strerror(errno);
    goto error2;
  }

  if (base::WriteFile(path, checksum.data(), checksum.size()) !=
      (int)checksum.size()) {
    LOG(ERROR) << __func__ << ": unable to write file '" << filename.c_str();
    goto error2;
  }

  fp = fopen(temp_filename.c_str(), "rb");
  if (!fp) {
    LOG(ERROR) << __func__ << ": unable to write to file '" << temp_filename
               << "': " << strerror(errno);
    goto error2;
  }

  // Sync written temp file out to disk. fsync() is blocking until data makes it
  // to disk.
  if (fsync(fileno(fp)) < 0) {
    LOG(WARNING) << __func__ << ": unable to fsync file '" << temp_filename
                 << "': " << strerror(errno);
  }

  if (fclose(fp) == EOF) {
    LOG(ERROR) << __func__ << ": unable to close file '" << temp_filename
               << "': " << strerror(errno);
    goto error2;
  }
  fp = nullptr;

  // Change the file's permissions to Read/Write by User and Group
  if (chmod(temp_filename.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) ==
      -1) {
    LOG(ERROR) << __func__ << ": unable to change file permissions '"
               << filename << "': " << strerror(errno);
    goto error2;
  }

  // Rename written temp file to the actual config file.
  if (rename(temp_filename.c_str(), filename.c_str()) == -1) {
    LOG(ERROR) << __func__ << ": unable to commit file '" << filename
               << "': " << strerror(errno);
    goto error2;
  }

  // This should ensure the directory is updated as well.
  if (fsync(dir_fd) < 0) {
    LOG(WARNING) << __func__ << ": unable to fsync dir '" << directoryname
                 << "': " << strerror(errno);
  }

  if (close(dir_fd) < 0) {
    LOG(ERROR) << __func__ << ": unable to close dir '" << directoryname
               << "': " << strerror(errno);
    goto error2;
  }

  return true;

error2:
  // This indicates there is a write issue.  Unlink as partial data is not
  // acceptable.
  unlink(temp_filename.c_str());
  if (fp) fclose(fp);
  if (dir_fd != -1) close(dir_fd);
  return false;
}

static char* trim(char* str) {
  while (isspace(*str)) ++str;

  if (!*str) return str;

  char* end_str = str + strlen(str) - 1;
  while (end_str > str && isspace(*end_str)) --end_str;

  end_str[1] = '\0';
  return str;
}

static bool config_parse(FILE* fp, config_t* config) {
  CHECK(fp != nullptr);
  CHECK(config != nullptr);

  int line_num = 0;
  char line[1024];
  char section[1024];
  strcpy(section, CONFIG_DEFAULT_SECTION);

  while (fgets(line, sizeof(line), fp)) {
    char* line_ptr = trim(line);
    ++line_num;

    // Skip blank and comment lines.
    if (*line_ptr == '\0' || *line_ptr == '#') continue;

    if (*line_ptr == '[') {
      size_t len = strlen(line_ptr);
      if (line_ptr[len - 1] != ']') {
        VLOG(1) << __func__ << ": unterminated section name on line "
                << line_num;
        return false;
      }
      strncpy(section, line_ptr + 1, len - 2);  // NOLINT (len < 1024)
      section[len - 2] = '\0';
    } else {
      char* split = strchr(line_ptr, '=');
      if (!split) {
        VLOG(1) << __func__ << ": no key/value separator found on line "
                << line_num;
        return false;
      }

      *split = '\0';
      config_set_string(config, section, trim(line_ptr), trim(split + 1));
    }
  }
  return true;
}
