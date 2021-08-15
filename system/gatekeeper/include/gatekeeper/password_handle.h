/*
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GATEKEEPER_PASSWORD_HANDLE_H_
#define GATEKEEPER_PASSWORD_HANDLE_H_

#define HANDLE_FLAG_THROTTLE_SECURE 1

#define HANDLE_VERSION_THROTTLE 2

namespace gatekeeper {

typedef uint64_t secure_id_t;
typedef uint64_t salt_t;
/**
 * structure for easy serialization
 * and deserialization of password handles.
 */
static const uint8_t HANDLE_VERSION = 2;
struct __attribute__ ((__packed__)) password_handle_t {
    // fields included in signature
    uint8_t version;
    secure_id_t user_id;
    uint64_t flags;

    // fields not included in signature
    salt_t salt;
    uint8_t signature[32];

    bool hardware_backed;
};
}

#endif // GATEKEEPER_PASSWORD_HANDLE_H_
