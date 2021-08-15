/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef __KEYSTORE_H__
#define __KEYSTORE_H__

#include <stdint.h>

// note state values overlap with ResponseCode for the purposes of the state() API
enum State {
    STATE_NO_ERROR      = 1,
    STATE_LOCKED        = 2,
    STATE_UNINITIALIZED = 3,
};

// must be in sync with KeyStore.java,
enum class ResponseCode : int32_t {
    NO_ERROR = STATE_NO_ERROR,            // 1
    LOCKED = STATE_LOCKED,                // 2
    UNINITIALIZED = STATE_UNINITIALIZED,  // 3
    SYSTEM_ERROR = 4,
    PROTOCOL_ERROR = 5,
    PERMISSION_DENIED = 6,
    KEY_NOT_FOUND = 7,
    VALUE_CORRUPTED = 8,
    UNDEFINED_ACTION = 9,
    WRONG_PASSWORD_0 = 10,
    WRONG_PASSWORD_1 = 11,
    WRONG_PASSWORD_2 = 12,
    WRONG_PASSWORD_3 = 13,  // MAX_RETRY = 4
    SIGNATURE_INVALID = 14,
    OP_AUTH_NEEDED = 15,  // Auth is needed for this operation before it can be used.
    KEY_ALREADY_EXISTS = 16,
    KEY_PERMANENTLY_INVALIDATED = 17,
};

/*
 * All the flags for import and insert calls.
 */
enum KeyStoreFlag : uint8_t {
    KEYSTORE_FLAG_NONE = 0,
    KEYSTORE_FLAG_ENCRYPTED = 1 << 0,
    KEYSTORE_FLAG_FALLBACK = 1 << 1,
    // KEYSTORE_FLAG_SUPER_ENCRYPTED is for blobs that are already encrypted by keymaster but have
    // an additional layer of password-based encryption applied.  The same encryption scheme is used
    // as KEYSTORE_FLAG_ENCRYPTED, but it's safe to remove super-encryption when the password is
    // cleared, rather than deleting blobs, and the error returned when attempting to use a
    // super-encrypted blob while keystore is locked is different.
    KEYSTORE_FLAG_SUPER_ENCRYPTED = 1 << 2,
    // KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION is for blobs that are part of device encryption
    // flow so it receives special treatment from keystore. For example this blob will not be super
    // encrypted, and it will be stored separately under an unique UID instead. This flag should
    // only be available to system uid.
    KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION = 1 << 3,
    KEYSTORE_FLAG_STRONGBOX = 1 << 4,
};

#endif
