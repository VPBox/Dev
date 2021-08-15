/*
**
** Copyright 2017, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef KM_OPENSSL_ATTESTATION_UTILS_H_
#define KM_OPENSSL_ATTESTATION_UTILS_H_

#include <hardware/keymaster_defs.h>
#include <keymaster/android_keymaster_utils.h>

#include <openssl/x509v3.h>

#include "openssl_utils.h"

namespace keymaster {

class AuthorizationSet;
class AttestationRecordContext;
class AsymmetricKey;


keymaster_error_t generate_attestation(const AsymmetricKey& key,
        const AuthorizationSet& attest_params, const keymaster_cert_chain_t& attestation_chain,
        const keymaster_key_blob_t& attestation_signing_key,
        const AttestationRecordContext& context, CertChainPtr* cert_chain_out);

} // namespace keymaster

#endif  // KM_OPENSSL_ATTESTATION_UTILS_H_
