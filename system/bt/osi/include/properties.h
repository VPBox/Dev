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

#pragma once

#include <cstdint>

#define PROPERTY_VALUE_MAX 92
#define BUILD_SANITY_PROPERTY_VALUE_MAX 92

// Get value associated with key |key| into |value|.
// Returns the length of the value which will never be greater than
// PROPERTY_VALUE_MAX - 1 and will always be zero terminated.
// (the length does not include the terminating zero).
// If the property read fails or returns an empty value, the |default_value|
// is used (if nonnull).  If the |default_value| is null, zero is returned.
int osi_property_get(const char* key, char* value, const char* default_value);

// Write value of property associated with key |key| to |value|.
// Returns 0 on success, < 0 on failure
int osi_property_set(const char* key, const char* value);

// Adapter function for property_get_int32 in
// libcutils/include/cutils/properties.h
//
// returns the value of |key| truncated and coerced into an
// int32_t. If the property is not set, then the |default_value| is used.
int32_t osi_property_get_int32(const char* key, int32_t default_value);

// Adapter function for property_get_bool in
// libcutils/include/cutils/properties.h
//
// returns the value of |key| coerced into a bool. If the property is not set,
// then the |default_value| is used.
bool osi_property_get_bool(const char* key, bool default_value);