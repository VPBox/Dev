/*
 * Copyright 2016 The Android Open Source Project
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

#ifndef SYSTEM_KEYMASTER_ATTESTATION_RECORD_H_
#define SYSTEM_KEYMASTER_ATTESTATION_RECORD_H_

#include <hardware/keymaster_defs.h>

#include <keymaster/authorization_set.h>

#include <openssl/asn1t.h>

namespace keymaster {

struct stack_st_ASN1_TYPE_Delete {
    void operator()(stack_st_ASN1_TYPE* p) { sk_ASN1_TYPE_free(p); }
};

struct ASN1_STRING_Delete {
    void operator()(ASN1_STRING* p) { ASN1_STRING_free(p); }
};

struct ASN1_TYPE_Delete {
    void operator()(ASN1_TYPE* p) { ASN1_TYPE_free(p); }
};

#define ASN1_INTEGER_SET STACK_OF(ASN1_INTEGER)

typedef struct km_root_of_trust {
    ASN1_OCTET_STRING* verified_boot_key;
    ASN1_BOOLEAN* device_locked;
    ASN1_ENUMERATED* verified_boot_state;
} KM_ROOT_OF_TRUST;

ASN1_SEQUENCE(KM_ROOT_OF_TRUST) = {
    ASN1_SIMPLE(KM_ROOT_OF_TRUST, verified_boot_key, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_ROOT_OF_TRUST, device_locked, ASN1_BOOLEAN),
    ASN1_SIMPLE(KM_ROOT_OF_TRUST, verified_boot_state, ASN1_ENUMERATED),
} ASN1_SEQUENCE_END(KM_ROOT_OF_TRUST);
DECLARE_ASN1_FUNCTIONS(KM_ROOT_OF_TRUST);

typedef struct km_auth_list {
    ASN1_INTEGER_SET* purpose;
    ASN1_INTEGER* algorithm;
    ASN1_INTEGER* key_size;
    ASN1_INTEGER_SET* block_mode;
    ASN1_INTEGER_SET* digest;
    ASN1_INTEGER_SET* padding;
    ASN1_NULL* caller_nonce;
    ASN1_INTEGER* min_mac_length;
    ASN1_INTEGER_SET* kdf;
    ASN1_INTEGER* ec_curve;
    ASN1_INTEGER* rsa_public_exponent;
    ASN1_INTEGER* active_date_time;
    ASN1_INTEGER* origination_expire_date_time;
    ASN1_INTEGER* usage_expire_date_time;
    ASN1_NULL* no_auth_required;
    ASN1_INTEGER* user_auth_type;
    ASN1_INTEGER* auth_timeout;
    ASN1_NULL* allow_while_on_body;
    ASN1_NULL* trusted_confirmation_required;
    ASN1_NULL* unlocked_device_required;
    ASN1_NULL* all_applications;
    ASN1_OCTET_STRING* application_id;
    ASN1_INTEGER* creation_date_time;
    ASN1_INTEGER* origin;
    ASN1_NULL* rollback_resistant;
    KM_ROOT_OF_TRUST* root_of_trust;
    ASN1_INTEGER* os_version;
    ASN1_INTEGER* os_patchlevel;
    ASN1_OCTET_STRING* attestation_application_id;
    ASN1_OCTET_STRING* attestation_id_brand;
    ASN1_OCTET_STRING* attestation_id_device;
    ASN1_OCTET_STRING* attestation_id_product;
    ASN1_OCTET_STRING* attestation_id_serial;
    ASN1_OCTET_STRING* attestation_id_imei;
    ASN1_OCTET_STRING* attestation_id_meid;
    ASN1_OCTET_STRING* attestation_id_manufacturer;
    ASN1_OCTET_STRING* attestation_id_model;
} KM_AUTH_LIST;

ASN1_SEQUENCE(KM_AUTH_LIST) = {
    ASN1_EXP_SET_OF_OPT(KM_AUTH_LIST, purpose, ASN1_INTEGER, TAG_PURPOSE.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, algorithm, ASN1_INTEGER, TAG_ALGORITHM.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, key_size, ASN1_INTEGER, TAG_KEY_SIZE.masked_tag()),
    ASN1_EXP_SET_OF_OPT(KM_AUTH_LIST, block_mode, ASN1_INTEGER, TAG_BLOCK_MODE.masked_tag()),
    ASN1_EXP_SET_OF_OPT(KM_AUTH_LIST, digest, ASN1_INTEGER, TAG_DIGEST.masked_tag()),
    ASN1_EXP_SET_OF_OPT(KM_AUTH_LIST, padding, ASN1_INTEGER, TAG_PADDING.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, caller_nonce, ASN1_NULL, TAG_CALLER_NONCE.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, min_mac_length, ASN1_INTEGER, TAG_MIN_MAC_LENGTH.masked_tag()),
    ASN1_EXP_SET_OF_OPT(KM_AUTH_LIST, kdf, ASN1_INTEGER, TAG_KDF.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, ec_curve, ASN1_INTEGER, TAG_EC_CURVE.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, rsa_public_exponent, ASN1_INTEGER,
                 TAG_RSA_PUBLIC_EXPONENT.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, active_date_time, ASN1_INTEGER, TAG_ACTIVE_DATETIME.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, origination_expire_date_time, ASN1_INTEGER,
                 TAG_ORIGINATION_EXPIRE_DATETIME.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, usage_expire_date_time, ASN1_INTEGER,
                 TAG_USAGE_EXPIRE_DATETIME.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, no_auth_required, ASN1_NULL, TAG_NO_AUTH_REQUIRED.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, user_auth_type, ASN1_INTEGER, TAG_USER_AUTH_TYPE.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, auth_timeout, ASN1_INTEGER, TAG_AUTH_TIMEOUT.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, allow_while_on_body, ASN1_NULL,
                 TAG_ALLOW_WHILE_ON_BODY.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, unlocked_device_required, ASN1_NULL,
                 TAG_UNLOCKED_DEVICE_REQUIRED.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, all_applications, ASN1_NULL, TAG_ALL_APPLICATIONS.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, application_id, ASN1_OCTET_STRING, TAG_APPLICATION_ID.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, creation_date_time, ASN1_INTEGER,
                 TAG_CREATION_DATETIME.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, origin, ASN1_INTEGER, TAG_ORIGIN.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, rollback_resistant, ASN1_NULL, TAG_ROLLBACK_RESISTANT.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, root_of_trust, KM_ROOT_OF_TRUST, TAG_ROOT_OF_TRUST.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, os_version, ASN1_INTEGER, TAG_OS_VERSION.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, os_patchlevel, ASN1_INTEGER, TAG_OS_PATCHLEVEL.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_application_id, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_APPLICATION_ID.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_brand, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_BRAND.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_device, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_DEVICE.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_product, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_PRODUCT.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_serial, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_SERIAL.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_imei, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_IMEI.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_meid, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_MEID.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_manufacturer, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_MANUFACTURER.masked_tag()),
    ASN1_EXP_OPT(KM_AUTH_LIST, attestation_id_model, ASN1_OCTET_STRING,
                 TAG_ATTESTATION_ID_MODEL.masked_tag()),
} ASN1_SEQUENCE_END(KM_AUTH_LIST);
DECLARE_ASN1_FUNCTIONS(KM_AUTH_LIST);

typedef struct km_key_description {
    ASN1_INTEGER* attestation_version;
    ASN1_ENUMERATED* attestation_security_level;
    ASN1_INTEGER* keymaster_version;
    ASN1_ENUMERATED* keymaster_security_level;
    ASN1_OCTET_STRING* attestation_challenge;
    KM_AUTH_LIST* software_enforced;
    KM_AUTH_LIST* tee_enforced;
    ASN1_INTEGER* unique_id;
} KM_KEY_DESCRIPTION;

ASN1_SEQUENCE(KM_KEY_DESCRIPTION) = {
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, attestation_version, ASN1_INTEGER),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, attestation_security_level, ASN1_ENUMERATED),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, keymaster_version, ASN1_INTEGER),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, keymaster_security_level, ASN1_ENUMERATED),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, attestation_challenge, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, unique_id, ASN1_OCTET_STRING),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, software_enforced, KM_AUTH_LIST),
    ASN1_SIMPLE(KM_KEY_DESCRIPTION, tee_enforced, KM_AUTH_LIST),
} ASN1_SEQUENCE_END(KM_KEY_DESCRIPTION);
DECLARE_ASN1_FUNCTIONS(KM_KEY_DESCRIPTION);

class AttestationRecordContext {
  protected:
    virtual ~AttestationRecordContext() {}

  public:
    /**
     * Returns the security level (SW or TEE) of this keymaster implementation.
     */
    virtual keymaster_security_level_t GetSecurityLevel() const {
        return KM_SECURITY_LEVEL_SOFTWARE;
    }

    /**
     * Verify that the device IDs provided in the attestation_params match the device's actual IDs
     * and copy them to attestation. If *any* of the IDs do not match or verification is not
     * possible, return KM_ERROR_CANNOT_ATTEST_IDS. If *all* IDs provided are successfully verified
     * or no IDs were provided, return KM_ERROR_OK.
     *
     * If you do not support device ID attestation, ignore all arguments and return
     * KM_ERROR_UNIMPLEMENTED.
     */
    virtual keymaster_error_t
    VerifyAndCopyDeviceIds(const AuthorizationSet& /* attestation_params */,
                           AuthorizationSet* /* attestation */) const {
        return KM_ERROR_UNIMPLEMENTED;
    }
    /**
     * Generate the current unique ID.
     */
    virtual keymaster_error_t GenerateUniqueId(uint64_t /*creation_date_time*/,
                                               const keymaster_blob_t& /*application_id*/,
                                               bool /*reset_since_rotation*/,
                                               Buffer* /*unique_id*/) const {
        return KM_ERROR_UNIMPLEMENTED;
    }

    /**
     * Returns verified boot parameters for the Attestation Extension.  For hardware-based
     * implementations, these will be the values reported by the bootloader. By default,  verified
     * boot state is unknown, and KM_ERROR_UNIMPLEMENTED is returned.
     */
    virtual keymaster_error_t
    GetVerifiedBootParams(keymaster_blob_t* /* verified_boot_key */,
                          keymaster_verified_boot_t* /* verified_boot_state */,
                          bool* /* device_locked */) const {
        return KM_ERROR_UNIMPLEMENTED;
    }
};

/**
 * The OID for Android attestation records.  For the curious, it breaks down as follows:
 *
 * 1 = ISO
 * 3 = org
 * 6 = DoD (Huh? OIDs are weird.)
 * 1 = IANA
 * 4 = Private
 * 1 = Enterprises
 * 11129 = Google
 * 2 = Google security
 * 1 = certificate extension
 * 17 = Android attestation extension.
 */
static const char kAttestionRecordOid[] = "1.3.6.1.4.1.11129.2.1.17";

keymaster_error_t build_attestation_record(const AuthorizationSet& attestation_params,
                                           AuthorizationSet software_enforced,
                                           AuthorizationSet tee_enforced,
                                           const AttestationRecordContext& context,
                                           UniquePtr<uint8_t[]>* asn1_key_desc,
                                           size_t* asn1_key_desc_len);

/**
 * Helper functions for attestation record tests. Caller takes ownership of
 * |attestation_challenge->data| and |unique_id->data|, deallocate using delete[].
 */
keymaster_error_t parse_attestation_record(const uint8_t* asn1_key_desc, size_t asn1_key_desc_len,
                                           uint32_t* attestation_version,  //
                                           keymaster_security_level_t* attestation_security_level,
                                           uint32_t* keymaster_version,
                                           keymaster_security_level_t* keymaster_security_level,
                                           keymaster_blob_t* attestation_challenge,
                                           AuthorizationSet* software_enforced,
                                           AuthorizationSet* tee_enforced,
                                           keymaster_blob_t* unique_id);

/**
 * Caller takes ownership of |verified_boot_key->data|, deallocate using delete[].
 */
keymaster_error_t parse_root_of_trust(const uint8_t* asn1_key_desc, size_t asn1_key_desc_len,
                                      keymaster_blob_t* verified_boot_key,
                                      keymaster_verified_boot_t* verified_boot_state,
                                      bool* device_locked);

keymaster_error_t build_auth_list(const AuthorizationSet& auth_list, KM_AUTH_LIST* record);

keymaster_error_t extract_auth_list(const KM_AUTH_LIST* record, AuthorizationSet* auth_list);
}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_ATTESTATION_RECORD_H_
