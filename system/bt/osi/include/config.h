#pragma once

// This module implements a configuration parser. Clients can query the
// contents of a configuration file through the interface provided here.
// The current implementation is read-only; mutations are only kept in
// memory. This parser supports the INI file format.

// Implementation notes:
// - Key/value pairs that are not within a section are assumed to be under
//   the |CONFIG_DEFAULT_SECTION| section.
// - Multiple sections with the same name will be merged as if they were in
//   a single section.
// - Empty sections with no key/value pairs will be treated as if they do
//   not exist. In other words, |config_has_section| will return false for
//   empty sections.
// - Duplicate keys in a section will overwrite previous values.
// - All strings are case sensitive.

#include <stdbool.h>
#include <list>
#include <memory>
#include <string>

// The default section name to use if a key/value pair is not defined within
// a section.
#define CONFIG_DEFAULT_SECTION "Global"

struct entry_t {
  std::string key;
  std::string value;
};

struct section_t {
  std::string name;
  std::list<entry_t> entries;
};

struct config_t {
  std::list<section_t> sections;
};

// Creates a new config object with no entries (i.e. not backed by a file).
// This function returns a unique pointer to config object.
std::unique_ptr<config_t> config_new_empty(void);

// Loads the specified file and returns a handle to the config file. If there
// was a problem loading the file, this function returns
// NULL. |filename| must not be NULL and must point to a readable
// file on the filesystem.
std::unique_ptr<config_t> config_new(const char* filename);

// Read the checksum from the |filename|
std::string checksum_read(const char* filename);

// Clones |src|, including all of it's sections, keys, and values.
// Returns a new config which is a copy and separated from the original;
// changes to the new config are not reflected in any way in the original.
//
// This function will not return NULL.
std::unique_ptr<config_t> config_new_clone(const config_t& src);

// Returns true if the config file contains a section named |section|. If
// the section has no key/value pairs in it, this function will return false.
bool config_has_section(const config_t& config, const std::string& section);

// Returns true if the config file has a key named |key| under |section|.
// Returns false otherwise.
bool config_has_key(const config_t& config, const std::string& section,
                    const std::string& key);

// Returns the integral value for a given |key| in |section|. If |section|
// or |key| do not exist, or the value cannot be fully converted to an integer,
// this function returns |def_value|.
int config_get_int(const config_t& config, const std::string& section,
                   const std::string& key, int def_value);

// Returns the uint64_t value for a given |key| in |section|. If |section|
// or |key| do not exist, or the value cannot be fully converted to an integer,
// this function returns |def_value|.
uint64_t config_get_uint64(const config_t& config, const std::string& section,
                           const std::string& key, uint64_t def_value);

// Returns the boolean value for a given |key| in |section|. If |section|
// or |key| do not exist, or the value cannot be converted to a boolean, this
// function returns |def_value|.
bool config_get_bool(const config_t& config, const std::string& section,
                     const std::string& key, bool def_value);

// Returns the string value for a given |key| in |section|. If |section| or
// |key| do not exist, this function returns |def_value|. The returned string
// is owned by the config module and must not be freed or modified. |def_value|
// may be NULL.
const std::string* config_get_string(const config_t& config,
                                     const std::string& section,
                                     const std::string& key,
                                     const std::string* def_value);

// Sets an integral value for the |key| in |section|. If |key| or |section| do
// not already exist, this function creates them. |config| must not be NULL.
void config_set_int(config_t* config, const std::string& section,
                    const std::string& key, int value);

// Sets a uint64_t value for the |key| in |section|. If |key| or |section| do
// not already exist, this function creates them. |config| must not be NULL.
void config_set_uint64(config_t* config, const std::string& section,
                       const std::string& key, uint64_t value);

// Sets a boolean value for the |key| in |section|. If |key| or |section| do
// not already exist, this function creates them. |config| must not be NULL.
void config_set_bool(config_t* config, const std::string& section,
                     const std::string& key, bool value);

// Sets a string value for the |key| in |section|. If |key| or |section| do
// not already exist, this function creates them. |config| must not be NULL.
void config_set_string(config_t* config, const std::string& section,
                       const std::string& key, const std::string& value);

// Removes |section| from the |config| (and, as a result, all keys in the
// section).
// Returns true if |section| was found and removed from |config|, false
// otherwise.
// |config| may be NULL.
bool config_remove_section(config_t* config, const std::string& section);

// Removes one specific |key| residing in |section| of the |config|. Returns
// true
// if the section and key were found and the key was removed, false otherwise.
// |config|may not be NULL.
bool config_remove_key(config_t* config, const std::string& section,
                       const std::string& key);

// Saves |config| to a file given by |filename|. Note that this could be a
// destructive operation: if |filename| already exists, it will be overwritten.
// The config module does not preserve comments or formatting so if a config
// file was opened with |config_new| and subsequently overwritten with
// |config_save|, all comments and special formatting in the original file will
// be lost. Neither |config| nor |filename| may be NULL.
bool config_save(const config_t& config, const std::string& filename);

// Saves the encrypted |checksum| of config file to a given |filename| Note
// that this could be a destructive operation: if |filename| already exists,
// it will be overwritten.
bool checksum_save(const std::string& checksum, const std::string& filename);
