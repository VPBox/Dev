// Copyright 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SECURITY_KEYSTORE_INCLUDE_KEYSTORE_KEYMASTER_TYPES_H_
#define SECURITY_KEYSTORE_INCLUDE_KEYSTORE_KEYMASTER_TYPES_H_

#include <android/hardware/keymaster/3.0/types.h>
#include <android/hardware/keymaster/4.0/IKeymasterDevice.h>
#include <android/hardware/keymaster/4.0/types.h>

#include <keymasterV4_0/authorization_set.h>
#include <keymasterV4_0/keymaster_tags.h>

/**
 * This header lifts the types from the current Keymaster version into the keystore namespace.
 */

namespace keystore {

// Changing this namespace alias will change the keymaster version.
namespace keymaster = ::android::hardware::keymaster::V4_0;

using android::hardware::hidl_vec;
using android::hardware::Return;

using keymaster::IKeymasterDevice;
using keymaster::SecurityLevel;

using keymaster::AuthorizationSet;
using keymaster::AuthorizationSetBuilder;

using keymaster::Algorithm;
using keymaster::BlockMode;
using keymaster::Digest;
using keymaster::EcCurve;
using keymaster::ErrorCode;
using keymaster::HardwareAuthenticatorType;
using keymaster::HardwareAuthToken;
using keymaster::HmacSharingParameters;
using keymaster::KeyCharacteristics;
using keymaster::KeyFormat;
using keymaster::KeyParameter;
using keymaster::KeyPurpose;
using keymaster::OperationHandle;
using keymaster::PaddingMode;
using keymaster::SecurityLevel;
using keymaster::Tag;
using keymaster::TagType;
using keymaster::VerificationToken;

using keymaster::TAG_ACTIVE_DATETIME;
using keymaster::TAG_ALGORITHM;
using keymaster::TAG_ALLOW_WHILE_ON_BODY;
using keymaster::TAG_APPLICATION_DATA;
using keymaster::TAG_APPLICATION_ID;
using keymaster::TAG_ATTESTATION_APPLICATION_ID;
using keymaster::TAG_AUTH_TIMEOUT;
using keymaster::TAG_BLOB_USAGE_REQUIREMENTS;
using keymaster::TAG_BLOCK_MODE;
using keymaster::TAG_DIGEST;
using keymaster::TAG_EC_CURVE;
using keymaster::TAG_KEY_SIZE;
using keymaster::TAG_MAC_LENGTH;
using keymaster::TAG_MAX_USES_PER_BOOT;
using keymaster::TAG_MIN_MAC_LENGTH;
using keymaster::TAG_MIN_SECONDS_BETWEEN_OPS;
using keymaster::TAG_NO_AUTH_REQUIRED;
using keymaster::TAG_NONCE;
using keymaster::TAG_ORIGIN;
using keymaster::TAG_ORIGINATION_EXPIRE_DATETIME;
using keymaster::TAG_PADDING;
using keymaster::TAG_PURPOSE;
using keymaster::TAG_RESET_SINCE_ID_ROTATION;
using keymaster::TAG_RSA_PUBLIC_EXPONENT;
using keymaster::TAG_USAGE_EXPIRE_DATETIME;
using keymaster::TAG_USER_AUTH_TYPE;
using keymaster::TAG_USER_ID;
using keymaster::TAG_USER_SECURE_ID;

using keymaster::NullOr;

using Km3HardwareAuthToken = ::android::hardware::keymaster::V3_0::HardwareAuthToken;
using Km3HardwareAuthenticatorType =
    ::android::hardware::keymaster::V3_0::HardwareAuthenticatorType;

// The following create the numeric values that KM_TAG_PADDING and KM_TAG_DIGEST used to have.  We
// need these old values to be able to support old keys that use them.
constexpr int32_t KM_TAG_DIGEST_OLD = static_cast<int32_t>(TagType::ENUM) | 5;
constexpr int32_t KM_TAG_PADDING_OLD = static_cast<int32_t>(TagType::ENUM) | 7;

}  // namespace keystore

#endif  // SYSTEM_SECURITY_KEYSTORE_KM4_AUTHORIZATION_SET_H_
