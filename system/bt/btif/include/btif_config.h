/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
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

#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "bt_types.h"

#include <list>
#include <string>
#include "osi/include/config.h"

static const char BTIF_CONFIG_MODULE[] = "btif_config_module";

bool btif_config_has_section(const char* section);
bool btif_config_exist(const std::string& section, const std::string& key);
bool btif_config_get_int(const std::string& section, const std::string& key,
                         int* value);
bool btif_config_set_int(const std::string& section, const std::string& key,
                         int value);
bool btif_config_get_uint64(const std::string& section, const std::string& key,
                            uint64_t* value);
bool btif_config_set_uint64(const std::string& section, const std::string& key,
                            uint64_t value);
bool btif_config_get_str(const std::string& section, const std::string& key,
                         char* value, int* size_bytes);
bool btif_config_set_str(const std::string& section, const std::string& key,
                         const std::string& value);
bool btif_config_get_bin(const std::string& section, const std::string& key,
                         uint8_t* value, size_t* length);
bool btif_config_set_bin(const std::string& section, const std::string& key,
                         const uint8_t* value, size_t length);
bool btif_config_remove(const std::string& section, const std::string& key);

size_t btif_config_get_bin_length(const std::string& section,
                                  const std::string& key);

std::list<section_t>& btif_config_sections();

void btif_config_save(void);
void btif_config_flush(void);
bool btif_config_clear(void);

// TODO(zachoverflow): Eww...we need to move these out. These are peer specific,
// not config general.
bool btif_get_address_type(const RawAddress& bd_addr, int* p_addr_type);
bool btif_get_device_type(const RawAddress& bd_addr, int* p_device_type);

void btif_debug_config_dump(int fd);
