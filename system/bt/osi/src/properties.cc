/******************************************************************************
 *
 *  Copyright 2016 Google, Inc.
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

#include <string.h>

#include "osi/include/properties.h"

#if !defined(OS_GENERIC)
#undef PROPERTY_VALUE_MAX
#include <cutils/properties.h>
#if BUILD_SANITY_PROPERTY_VALUE_MAX != PROPERTY_VALUE_MAX
#error "PROPERTY_VALUE_MAX from osi/include/properties.h != the Android value"
#endif  // GENERIC_PROPERTY_VALUE_MAX != PROPERTY_VALUE_MAX
#endif  // !defined(OS_GENERIC)

int osi_property_get(const char* key, char* value, const char* default_value) {
#if defined(OS_GENERIC)
  /* For linux right now just return default value, if present */
  int len = 0;
  if (!default_value) return len;

  len = strlen(default_value);
  if (len >= PROPERTY_VALUE_MAX) len = PROPERTY_VALUE_MAX - 1;

  memcpy(value, default_value, len);
  value[len] = '\0';
  return len;
#else
  return property_get(key, value, default_value);
#endif  // defined(OS_GENERIC)
}

int osi_property_set(const char* key, const char* value) {
#if defined(OS_GENERIC)
  return -1;
#else
  return property_set(key, value);
#endif  // defined(OS_GENERIC)
}

int32_t osi_property_get_int32(const char* key, int32_t default_value) {
#if defined(OS_GENERIC)
  return default_value;
#else
  return property_get_int32(key, default_value);
#endif  // defined(OS_GENERIC)
}

bool osi_property_get_bool(const char* key, bool default_value) {
#if defined(OS_GENERIC)
  return default_value;
#else
  return property_get_bool(key, default_value);
#endif  // defined(OS_GENERIC)
}