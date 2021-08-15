/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef SYSTEM_KEYMASTER_WRAPPED_KEY_H_
#define SYSTEM_KEYMASTER_WRAPPED_KEY_H_

#include <hardware/keymaster_defs.h>

#include <keymaster/attestation_record.h>
#include <keymaster/authorization_set.h>

namespace keymaster {

typedef struct km_wrapped_key_description {
    ASN1_INTEGER* key_format;
    KM_AUTH_LIST* auth_list;
} KM_WRAPPED_KEY_DESCRIPTION;

ASN1_SEQUENCE(KM_WRAPPED_KEY_DESCRIPTION) = {
    ASN1_SIMPLE(KM_WRAPPED_KEY_DESCRIPTION, key_format, ASN1_INTEGER),
    ASN1_SIMPLE(KM_WRAPPED_KEY_DESCRIPTION, auth_list, KM_AUTH_LIST),
} ASN1_SEQUENCE_END(KM_WRAPPED_KEY_DESCRIPTION);
DECLARE_ASN1_FUNCTIONS(KM_WRAPPED_KEY_DESCRIPTION);

typedef struct km_wrapped_key {
    ASN1_INTEGER* version;
    ASN1_OCTET_STRING* transit_key;
    ASN1_OCTET_STRING* iv;
    KM_WRAPPED_KEY_DESCRIPTION* wrapped_key_description;
    ASN1_OCTET_STRING* secure_key;
    ASN1_OCTET_STRING* tag;
} KM_WRAPPED_KEY;

ASN1_SEQUENCE(KM_WRAPPED_KEY) = {
    ASN1_SIMPLE(KM_WRAPPED_KEY, version, ASN1_INTEGER),
    ASN1_SIMPLE(KM_WRAPPED_KEY, transit_key, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_WRAPPED_KEY, iv, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_WRAPPED_KEY, wrapped_key_description, KM_WRAPPED_KEY_DESCRIPTION),
    ASN1_SIMPLE(KM_WRAPPED_KEY, secure_key, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_WRAPPED_KEY, tag, ASN1_OCTET_STRING),
} ASN1_SEQUENCE_END(KM_WRAPPED_KEY);
DECLARE_ASN1_FUNCTIONS(KM_WRAPPED_KEY);

keymaster_error_t build_wrapped_key(const KeymasterKeyBlob& encrypted_ephemeral_key,
                                    const KeymasterBlob& iv, keymaster_key_format_t key_format,
                                    const KeymasterKeyBlob& secure_key, const KeymasterBlob& tag,
                                    const AuthorizationSet& authorization_list,
                                    KeymasterKeyBlob* der_wrapped_key);

keymaster_error_t parse_wrapped_key(const KeymasterKeyBlob& wrapped_key, KeymasterBlob* iv,
                                    KeymasterKeyBlob* transit_key, KeymasterKeyBlob* secure_key,
                                    KeymasterBlob* tag, AuthorizationSet* auth_list,
                                    keymaster_key_format_t* key_format,
                                    KeymasterBlob* wrapped_key_description);

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_WRAPPED_KEY_H_
