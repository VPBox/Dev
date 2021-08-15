/*
 * Copyright (C) 2018 The Android Open Source Project
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
#define LOG_TAG "KeystoreOperation"

#include "key_proto_handler.h"

#include <android/os/DropBoxManager.h>
#include <google/protobuf/message_lite.h>
#include <keymasterV4_0/Keymaster.h>
#include <keystore/keymaster_types.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>

#include "key_config.pb.h"

namespace keystore {

void checkEnforcedCharacteristics(const hidl_vec<KeyParameter>& keyParams, KeyConfig* keyConfig) {
    for (auto& keyParam : keyParams) {
        switch (keyParam.tag) {
        case Tag::PURPOSE:
            keyConfig->add_purpose(toString(accessTagValue(TAG_PURPOSE, keyParam)));
            break;
        case Tag::ALGORITHM:
            keyConfig->set_algorithm(toString(accessTagValue(TAG_ALGORITHM, keyParam)));
            break;
        case Tag::KEY_SIZE:
            keyConfig->set_key_size(accessTagValue(TAG_KEY_SIZE, keyParam));
            break;
        case Tag::BLOCK_MODE:
            keyConfig->add_block_mode(toString(accessTagValue(TAG_BLOCK_MODE, keyParam)));
            break;
        case Tag::PADDING:
            keyConfig->add_padding(toString(accessTagValue(TAG_PADDING, keyParam)));
            break;
        case Tag::DIGEST:
            keyConfig->add_digest(toString(accessTagValue(TAG_DIGEST, keyParam)));
            break;
        case Tag::EC_CURVE:
            keyConfig->set_ec_curve(toString(accessTagValue(TAG_EC_CURVE, keyParam)));
            break;
        case Tag::AUTH_TIMEOUT:
            keyConfig->set_user_auth_key_timeout(accessTagValue(TAG_AUTH_TIMEOUT, keyParam));
            break;
        case Tag::ORIGIN:
            keyConfig->set_origin(toString(accessTagValue(TAG_ORIGIN, keyParam)));
            break;
        case Tag::BLOB_USAGE_REQUIREMENTS:
            keyConfig->set_key_blob_usage_reqs(
                toString(accessTagValue(TAG_BLOB_USAGE_REQUIREMENTS, keyParam)));
            break;
        case Tag::USER_AUTH_TYPE:
            keyConfig->set_user_auth_type(toString(accessTagValue(TAG_USER_AUTH_TYPE, keyParam)));
            break;
        default:
            break;
        }
    }
}

void uploadKeyCharacteristicsAsProto(const hidl_vec<KeyParameter>& keyParams,
                                     bool wasCreationSuccessful) {
    KeyConfig keyConfig;
    checkEnforcedCharacteristics(keyParams, &keyConfig);
    android::sp<android::os::DropBoxManager> dropbox(new android::os::DropBoxManager());
    keyConfig.set_was_creation_successful(wasCreationSuccessful);

    size_t size = keyConfig.ByteSize();
    auto data = std::make_unique<uint8_t[]>(size);
    keyConfig.SerializeWithCachedSizesToArray(data.get());
    dropbox->addData(android::String16("keymaster"), data.get(), size, 0);
}

}  // namespace keystore
