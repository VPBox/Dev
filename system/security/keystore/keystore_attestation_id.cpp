/*
 * Copyright (C) 2016 The Android Open Source Project
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
#include "keystore_attestation_id.h"

#define LOG_TAG "keystore_att_id"

#include <log/log.h>

#include <memory>
#include <string>
#include <vector>

#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/Parcelable.h>
#include <binder/PersistableBundle.h>

#include <android/security/keymaster/BpKeyAttestationApplicationIdProvider.h>
#include <android/security/keymaster/IKeyAttestationApplicationIdProvider.h>
#include <keystore/KeyAttestationApplicationId.h>
#include <keystore/KeyAttestationPackageInfo.h>
#include <keystore/Signature.h>

#include <private/android_filesystem_config.h> /* for AID_SYSTEM */

#include <openssl/asn1t.h>
#include <openssl/bn.h>
#include <openssl/sha.h>

#include <utils/String8.h>

namespace android {

namespace {

constexpr const char* kAttestationSystemPackageName = "AndroidSystem";
constexpr const char* kUnknownPackageName = "UnknownPackage";

std::vector<uint8_t> signature2SHA256(const content::pm::Signature& sig) {
    std::vector<uint8_t> digest_buffer(SHA256_DIGEST_LENGTH);
    SHA256(sig.data().data(), sig.data().size(), digest_buffer.data());
    return digest_buffer;
}

using ::android::security::keymaster::BpKeyAttestationApplicationIdProvider;

class KeyAttestationApplicationIdProvider : public BpKeyAttestationApplicationIdProvider {
  public:
    KeyAttestationApplicationIdProvider();

    static KeyAttestationApplicationIdProvider& get();

  private:
    android::sp<android::IServiceManager> service_manager_;
};

KeyAttestationApplicationIdProvider& KeyAttestationApplicationIdProvider::get() {
    static KeyAttestationApplicationIdProvider mpm;
    return mpm;
}

KeyAttestationApplicationIdProvider::KeyAttestationApplicationIdProvider()
    : BpKeyAttestationApplicationIdProvider(
          android::defaultServiceManager()->getService(String16("sec_key_att_app_id_provider"))) {}

DECLARE_STACK_OF(ASN1_OCTET_STRING);

typedef struct km_attestation_package_info {
    ASN1_OCTET_STRING* package_name;
    ASN1_INTEGER* version;
} KM_ATTESTATION_PACKAGE_INFO;

// Estimated size:
// 4 bytes for the package name + package_name length
// 11 bytes for the version (2 bytes header and up to 9 bytes of data).
constexpr size_t AAID_PKG_INFO_OVERHEAD = 15;
ASN1_SEQUENCE(KM_ATTESTATION_PACKAGE_INFO) = {
    ASN1_SIMPLE(KM_ATTESTATION_PACKAGE_INFO, package_name, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_ATTESTATION_PACKAGE_INFO, version, ASN1_INTEGER),
} ASN1_SEQUENCE_END(KM_ATTESTATION_PACKAGE_INFO);
IMPLEMENT_ASN1_FUNCTIONS(KM_ATTESTATION_PACKAGE_INFO);

DECLARE_STACK_OF(KM_ATTESTATION_PACKAGE_INFO);

// Estimated size:
// See estimate above for the stack of package infos.
// 34 (32 + 2) bytes for each signature digest.
constexpr size_t AAID_SIGNATURE_SIZE = 34;
typedef struct km_attestation_application_id {
    STACK_OF(KM_ATTESTATION_PACKAGE_INFO) * package_infos;
    STACK_OF(ASN1_OCTET_STRING) * signature_digests;
} KM_ATTESTATION_APPLICATION_ID;

// Estimated overhead:
// 4 for the header of the octet string containing the fully-encoded data.
// 4 for the sequence header.
// 4 for the header of the package info set.
// 4 for the header of the signature set.
constexpr size_t AAID_GENERAL_OVERHEAD = 16;
ASN1_SEQUENCE(KM_ATTESTATION_APPLICATION_ID) = {
    ASN1_SET_OF(KM_ATTESTATION_APPLICATION_ID, package_infos, KM_ATTESTATION_PACKAGE_INFO),
    ASN1_SET_OF(KM_ATTESTATION_APPLICATION_ID, signature_digests, ASN1_OCTET_STRING),
} ASN1_SEQUENCE_END(KM_ATTESTATION_APPLICATION_ID);
IMPLEMENT_ASN1_FUNCTIONS(KM_ATTESTATION_APPLICATION_ID);

}  // namespace

}  // namespace android

namespace std {
template <> struct default_delete<android::KM_ATTESTATION_PACKAGE_INFO> {
    void operator()(android::KM_ATTESTATION_PACKAGE_INFO* p) {
        android::KM_ATTESTATION_PACKAGE_INFO_free(p);
    }
};
template <> struct default_delete<ASN1_OCTET_STRING> {
    void operator()(ASN1_OCTET_STRING* p) { ASN1_OCTET_STRING_free(p); }
};
template <> struct default_delete<android::KM_ATTESTATION_APPLICATION_ID> {
    void operator()(android::KM_ATTESTATION_APPLICATION_ID* p) {
        android::KM_ATTESTATION_APPLICATION_ID_free(p);
    }
};
}  // namespace std

namespace android {
namespace security {
namespace {

using ::android::security::keymaster::KeyAttestationApplicationId;
using ::android::security::keymaster::KeyAttestationPackageInfo;

status_t build_attestation_package_info(const KeyAttestationPackageInfo& pinfo,
    std::unique_ptr<KM_ATTESTATION_PACKAGE_INFO>* attestation_package_info_ptr) {

    if (!attestation_package_info_ptr) return BAD_VALUE;
    auto& attestation_package_info = *attestation_package_info_ptr;

    attestation_package_info.reset(KM_ATTESTATION_PACKAGE_INFO_new());
    if (!attestation_package_info.get()) return NO_MEMORY;

    if (!pinfo.package_name()) {
        ALOGE("Key attestation package info lacks package name");
        return BAD_VALUE;
    }

    std::string pkg_name(String8(*pinfo.package_name()).string());
    if (!ASN1_OCTET_STRING_set(attestation_package_info->package_name,
                               reinterpret_cast<const unsigned char*>(pkg_name.data()),
                               pkg_name.size())) {
        return UNKNOWN_ERROR;
    }

    BIGNUM* bn_version = BN_new();
    if (bn_version == nullptr) {
        return NO_MEMORY;
    }
    if (BN_set_u64(bn_version, static_cast<uint64_t>(pinfo.version_code())) != 1) {
        BN_free(bn_version);
        return UNKNOWN_ERROR;
    }
    status_t retval = NO_ERROR;
    if (BN_to_ASN1_INTEGER(bn_version, attestation_package_info->version) == nullptr) {
        retval = UNKNOWN_ERROR;
    }
    BN_free(bn_version);
    return retval;
}

/* The following function are not used. They are mentioned here to silence
 * warnings about them not being used.
 */
void unused_functions_silencer() __attribute__((unused));
void unused_functions_silencer() {
    i2d_KM_ATTESTATION_PACKAGE_INFO(nullptr, nullptr);
    d2i_KM_ATTESTATION_APPLICATION_ID(nullptr, nullptr, 0);
    d2i_KM_ATTESTATION_PACKAGE_INFO(nullptr, nullptr, 0);
}

}  // namespace

StatusOr<std::vector<uint8_t>>
build_attestation_application_id(const KeyAttestationApplicationId& key_attestation_id) {
    auto attestation_id =
        std::unique_ptr<KM_ATTESTATION_APPLICATION_ID>(KM_ATTESTATION_APPLICATION_ID_new());
    size_t estimated_encoded_size = AAID_GENERAL_OVERHEAD;

    auto attestation_pinfo_stack = reinterpret_cast<_STACK*>(attestation_id->package_infos);

    if (key_attestation_id.pinfos_begin() == key_attestation_id.pinfos_end()) return BAD_VALUE;

    for (auto pinfo = key_attestation_id.pinfos_begin(); pinfo != key_attestation_id.pinfos_end();
         ++pinfo) {
        if (!pinfo->package_name()) {
            ALOGE("Key attestation package info lacks package name");
            return BAD_VALUE;
        }
        std::string package_name(String8(*pinfo->package_name()).string());
        std::unique_ptr<KM_ATTESTATION_PACKAGE_INFO> attestation_package_info;
        auto rc = build_attestation_package_info(*pinfo, &attestation_package_info);
        if (rc != NO_ERROR) {
            ALOGE("Building DER attestation package info failed %d", rc);
            return rc;
        }
        estimated_encoded_size += AAID_PKG_INFO_OVERHEAD + package_name.size();
        if (estimated_encoded_size > KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE) {
            break;
        }
        if (!sk_push(attestation_pinfo_stack, attestation_package_info.get())) {
            return NO_MEMORY;
        }
        // if push succeeded, the stack takes ownership
        attestation_package_info.release();
    }

    /** Apps can only share a uid iff they were signed with the same certificate(s). Because the
     *  signature field actually holds the signing certificate, rather than a signature, we can
     *  simply use the set of signature digests of the first package info.
     */
    const auto& pinfo = *key_attestation_id.pinfos_begin();
    std::vector<std::vector<uint8_t>> signature_digests;

    for (auto sig = pinfo.sigs_begin(); sig != pinfo.sigs_end(); ++sig) {
        signature_digests.push_back(signature2SHA256(*sig));
    }

    auto signature_digest_stack = reinterpret_cast<_STACK*>(attestation_id->signature_digests);
    for (auto si : signature_digests) {
        estimated_encoded_size += AAID_SIGNATURE_SIZE;
        if (estimated_encoded_size > KEY_ATTESTATION_APPLICATION_ID_MAX_SIZE) {
            break;
        }
        auto asn1_item = std::unique_ptr<ASN1_OCTET_STRING>(ASN1_OCTET_STRING_new());
        if (!asn1_item) return NO_MEMORY;
        if (!ASN1_OCTET_STRING_set(asn1_item.get(), si.data(), si.size())) {
            return UNKNOWN_ERROR;
        }
        if (!sk_push(signature_digest_stack, asn1_item.get())) {
            return NO_MEMORY;
        }
        asn1_item.release();  // if push succeeded, the stack takes ownership
    }

    int len = i2d_KM_ATTESTATION_APPLICATION_ID(attestation_id.get(), nullptr);
    if (len < 0) return UNKNOWN_ERROR;

    std::vector<uint8_t> result(len);
    uint8_t* p = result.data();
    len = i2d_KM_ATTESTATION_APPLICATION_ID(attestation_id.get(), &p);
    if (len < 0) return UNKNOWN_ERROR;

    return result;
}

StatusOr<std::vector<uint8_t>> gather_attestation_application_id(uid_t uid) {
    KeyAttestationApplicationId key_attestation_id;

    if (uid == AID_SYSTEM) {
        /* Use a fixed ID for system callers */
        auto pinfo = std::make_unique<KeyAttestationPackageInfo>(
            String16(kAttestationSystemPackageName), 1 /* version code */,
            std::make_shared<KeyAttestationPackageInfo::SignaturesVector>());
        key_attestation_id = KeyAttestationApplicationId(std::move(pinfo));
    } else {
        /* Get the attestation application ID from package manager */
        auto& pm = KeyAttestationApplicationIdProvider::get();
        auto status = pm.getKeyAttestationApplicationId(uid, &key_attestation_id);
        // Package Manager call has failed, perform attestation but indicate that the
        // caller is unknown.
        if (!status.isOk()) {
            ALOGW("package manager request for key attestation ID failed with: %s %d",
                  status.exceptionMessage().string(), status.exceptionCode());
            auto pinfo = std::make_unique<KeyAttestationPackageInfo>(
                String16(kUnknownPackageName), 1 /* version code */,
                std::make_shared<KeyAttestationPackageInfo::SignaturesVector>());
            key_attestation_id = KeyAttestationApplicationId(std::move(pinfo));
        }
    }

    /* DER encode the attestation application ID */
    return build_attestation_application_id(key_attestation_id);
}

}  // namespace security
}  // namespace android
