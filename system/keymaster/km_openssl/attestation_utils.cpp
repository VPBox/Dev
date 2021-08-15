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

#include <keymaster/km_openssl/attestation_utils.h>

#include <hardware/keymaster_defs.h>

#include <keymaster/authorization_set.h>
#include <keymaster/attestation_record.h>
#include <keymaster/km_openssl/asymmetric_key.h>
#include <keymaster/km_openssl/openssl_utils.h>
#include <keymaster/km_openssl/openssl_err.h>

#include <openssl/x509v3.h>
#include <openssl/evp.h>


namespace keymaster {

namespace {

constexpr int kDigitalSignatureKeyUsageBit = 0;
constexpr int kKeyEnciphermentKeyUsageBit = 2;
constexpr int kDataEnciphermentKeyUsageBit = 3;
constexpr int kMaxKeyUsageBit = 8;

template <typename T> T && min(T && a, T && b) {
    return (a < b) ? forward<T>(a) : forward<T>(b);
}

struct emptyCert {};

__attribute__((__unused__))
inline keymaster_blob_t certBlobifier(const emptyCert&, bool*){ return {}; }
template <size_t N>
inline keymaster_blob_t certBlobifier(const uint8_t (&cert)[N], bool* fail){
    keymaster_blob_t result = { dup_array(cert), N };
    if (!result.data) {
        *fail = true;
        return {};
    }
    return result;
}
inline keymaster_blob_t certBlobifier(const keymaster_blob_t& blob, bool* fail){
    if (blob.data == nullptr || blob.data_length == 0) return {};
    keymaster_blob_t result = { dup_array(blob.data, blob.data_length), blob.data_length };
    if (!result.data) {
        *fail = true;
        return {};
    }
    return result;
}
inline keymaster_blob_t certBlobifier(keymaster_blob_t&& blob, bool*){
    if (blob.data == nullptr || blob.data_length == 0) return {};
    keymaster_blob_t result = blob;
    blob = {};
    return result;
}
inline keymaster_blob_t certBlobifier(X509* certificate, bool* fail){
    int len = i2d_X509(certificate, nullptr);
    if (len < 0) {
        *fail = true;
        return {};
    }

    uint8_t* data = new(std::nothrow) uint8_t[len];
    if (!data) {
        *fail = true;
        return {};
    }
    uint8_t* p = data;

    i2d_X509(certificate, &p);

    return { data, (size_t)len };
}

inline bool certCopier(keymaster_blob_t** out, const keymaster_cert_chain_t& chain,
                              bool* fail) {
    for (size_t i = 0; i < chain.entry_count; ++i) {
        *(*out)++ = certBlobifier(chain.entries[i], fail);
    }
    return *fail;
}

__attribute__((__unused__))
inline bool certCopier(keymaster_blob_t** out, keymaster_cert_chain_t&& chain, bool* fail) {
    for (size_t i = 0; i < chain.entry_count; ++i) {
        *(*out)++ = certBlobifier(move(chain.entries[i]), fail);
    }
    delete[] chain.entries;
    chain.entries = nullptr;
    chain.entry_count = 0;
    return *fail;
}
template <typename CERT>
inline bool certCopier(keymaster_blob_t** out, CERT&& cert, bool* fail) {
    *(*out)++ = certBlobifier(forward<CERT>(cert), fail);
    return *fail;
}

inline bool certCopyHelper(keymaster_blob_t**, bool* fail) {
    return *fail;
}

template <typename CERT, typename... CERTS>
inline bool certCopyHelper(keymaster_blob_t** out, bool* fail, CERT&& cert, CERTS&&... certs) {
    certCopier(out, forward<CERT>(cert), fail);
    return certCopyHelper(out, fail, forward<CERTS>(certs)...);
}



template <typename T>
inline size_t noOfCert(T &&) { return 1; }
inline size_t noOfCert(const keymaster_cert_chain_t& cert_chain) { return cert_chain.entry_count; }

inline size_t certCount() { return 0; }
template <typename CERT, typename... CERTS>
inline size_t certCount(CERT&& cert, CERTS&&... certs) {
    return noOfCert(forward<CERT>(cert)) + certCount(forward<CERTS>(certs)...);
}

/*
 * makeCertChain creates a new keymaster_cert_chain_t from all the certs that get thrown at it
 * in the given order. A cert may be a X509*, uint8_t[], a keymaster_blob_t, an instance of
 * emptyCert, or another keymater_cert_chain_t in which case the certs of the chain are included
 * in the new chain. emptyCert is a placeholder which results in an empty slot at the given
 * position in the newly created certificate chain. E.g., makeCertChain(emptyCert(), someCertChain)
 * allocates enough slots to accommodate all certificates of someCertChain plus one empty slot and
 * copies in someCertChain starting at index 1 so that the slot with index 0 can be used for a new
 * leaf entry.
 *
 * makeCertChain respects move semantics. E.g., makeCertChain(emptyCert(), std::move(someCertChain))
 * will take possession of secondary resources for the certificate blobs so that someCertChain is
 * empty after the call. Also, because no allocation happens this cannot fail. Note, however, that
 * if another cert is passed to makeCertChain, that needs to be copied and thus requires
 * allocation, and this allocation fails, all resources - allocated or moved - will be reaped.
 */
template <typename... CERTS>
CertChainPtr makeCertChain(CERTS&&... certs) {
    CertChainPtr result(new (std::nothrow) keymaster_cert_chain_t);
    if (!result.get()) return {};
    result->entries = new (std::nothrow) keymaster_blob_t[certCount(forward<CERTS>(certs)...)];
    if (!result->entries) return {};
    result->entry_count = certCount(forward<CERTS>(certs)...);
    bool allocation_failed = false;
    keymaster_blob_t* entries = result->entries;
    certCopyHelper(&entries, &allocation_failed, forward<CERTS>(certs)...);
    if (allocation_failed) return {};
    return result;
}


keymaster_error_t build_attestation_extension(const AuthorizationSet& attest_params,
                                                     const AuthorizationSet& tee_enforced,
                                                     const AuthorizationSet& sw_enforced,
                                                     const AttestationRecordContext& context,
                                                     X509_EXTENSION_Ptr* extension) {
    ASN1_OBJECT_Ptr oid(
        OBJ_txt2obj(kAttestionRecordOid, 1 /* accept numerical dotted string form only */));
    if (!oid.get())
        return TranslateLastOpenSslError();

    UniquePtr<uint8_t[]> attest_bytes;
    size_t attest_bytes_len;
    keymaster_error_t error = build_attestation_record(attest_params, sw_enforced, tee_enforced,
                                                       context, &attest_bytes, &attest_bytes_len);
    if (error != KM_ERROR_OK)
        return error;

    ASN1_OCTET_STRING_Ptr attest_str(ASN1_OCTET_STRING_new());
    if (!attest_str.get() ||
        !ASN1_OCTET_STRING_set(attest_str.get(), attest_bytes.get(), attest_bytes_len))
        return TranslateLastOpenSslError();

    extension->reset(
        X509_EXTENSION_create_by_OBJ(nullptr, oid.get(), 0 /* not critical */, attest_str.get()));
    if (!extension->get())
        return TranslateLastOpenSslError();

    return KM_ERROR_OK;
}

keymaster_error_t add_key_usage_extension(const AuthorizationSet& tee_enforced,
                                                 const AuthorizationSet& sw_enforced,
                                                 X509* certificate) {
    // Build BIT_STRING with correct contents.
    ASN1_BIT_STRING_Ptr key_usage(ASN1_BIT_STRING_new());
    if (!key_usage) return KM_ERROR_MEMORY_ALLOCATION_FAILED;

    for (size_t i = 0; i <= kMaxKeyUsageBit; ++i) {
        if (!ASN1_BIT_STRING_set_bit(key_usage.get(), i, 0)) {
            return TranslateLastOpenSslError();
        }
    }

    if (tee_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_SIGN) ||
        tee_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_VERIFY) ||
        sw_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_SIGN) ||
        sw_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_VERIFY)) {
        if (!ASN1_BIT_STRING_set_bit(key_usage.get(), kDigitalSignatureKeyUsageBit, 1)) {
            return TranslateLastOpenSslError();
        }
    }

    if (tee_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_ENCRYPT) ||
        tee_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_DECRYPT) ||
        sw_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_ENCRYPT) ||
        sw_enforced.Contains(TAG_PURPOSE, KM_PURPOSE_DECRYPT)) {
        if (!ASN1_BIT_STRING_set_bit(key_usage.get(), kKeyEnciphermentKeyUsageBit, 1) ||
            !ASN1_BIT_STRING_set_bit(key_usage.get(), kDataEnciphermentKeyUsageBit, 1)) {
            return TranslateLastOpenSslError();
        }
    }

    // Convert to octets
    int len = i2d_ASN1_BIT_STRING(key_usage.get(), nullptr);
    if (len < 0) {
        return TranslateLastOpenSslError();
    }
    UniquePtr<uint8_t[]> asn1_key_usage(new(std::nothrow) uint8_t[len]);
    if (!asn1_key_usage.get()) {
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    uint8_t* p = asn1_key_usage.get();
    len = i2d_ASN1_BIT_STRING(key_usage.get(), &p);
    if (len < 0) {
        return TranslateLastOpenSslError();
    }

    // Build OCTET_STRING
    ASN1_OCTET_STRING_Ptr key_usage_str(ASN1_OCTET_STRING_new());
    if (!key_usage_str.get() ||
        !ASN1_OCTET_STRING_set(key_usage_str.get(), asn1_key_usage.get(), len)) {
        return TranslateLastOpenSslError();
    }

    X509_EXTENSION_Ptr key_usage_extension(X509_EXTENSION_create_by_NID(nullptr,        //
                                                                        NID_key_usage,  //
                                                                        false /* critical */,
                                                                        key_usage_str.get()));
    if (!key_usage_extension.get()) {
        return TranslateLastOpenSslError();
    }

    if (!X509_add_ext(certificate, key_usage_extension.get() /* Don't release; copied */,
                      -1 /* insert at end */)) {
        return TranslateLastOpenSslError();
    }

    return KM_ERROR_OK;
}

bool add_public_key(EVP_PKEY* key, X509* certificate, keymaster_error_t* error) {
    if (!X509_set_pubkey(certificate, key)) {
        *error = TranslateLastOpenSslError();
        return false;
    }
    return true;
}

bool add_attestation_extension(const AuthorizationSet& attest_params,
                                      const AuthorizationSet& tee_enforced,
                                      const AuthorizationSet& sw_enforced,
                                      const AttestationRecordContext& context,
                                      X509* certificate,
                                      keymaster_error_t* error) {
    X509_EXTENSION_Ptr attest_extension;
    *error = build_attestation_extension(attest_params, tee_enforced, sw_enforced, context,
                                         &attest_extension);
    if (*error != KM_ERROR_OK)
        return false;

    if (!X509_add_ext(certificate, attest_extension.get() /* Don't release; copied */,
                      -1 /* insert at end */)) {
        *error = TranslateLastOpenSslError();
        return false;
    }

    return true;
}

} // anonymous namespace

keymaster_error_t generate_attestation(const AsymmetricKey& key,
        const AuthorizationSet& attest_params, const keymaster_cert_chain_t& attestation_chain,
        const keymaster_key_blob_t& attestation_signing_key,
        const AttestationRecordContext& context, CertChainPtr* cert_chain_out) {

    if (!cert_chain_out)
        return KM_ERROR_UNEXPECTED_NULL_POINTER;

    keymaster_algorithm_t sign_algorithm;
    if ((!key.sw_enforced().GetTagValue(TAG_ALGORITHM, &sign_algorithm) &&
         !key.hw_enforced().GetTagValue(TAG_ALGORITHM, &sign_algorithm)))
        return KM_ERROR_UNKNOWN_ERROR;

    EVP_PKEY_Ptr pkey(EVP_PKEY_new());
    if (!key.InternalToEvp(pkey.get()))
        return TranslateLastOpenSslError();

    X509_Ptr certificate(X509_new());
    if (!certificate.get())
        return TranslateLastOpenSslError();

    if (!X509_set_version(certificate.get(), 2 /* version 3, but zero-based */))
        return TranslateLastOpenSslError();

    ASN1_INTEGER_Ptr serialNumber(ASN1_INTEGER_new());
    if (!serialNumber.get() || !ASN1_INTEGER_set(serialNumber.get(), 1) ||
        !X509_set_serialNumber(certificate.get(), serialNumber.get() /* Don't release; copied */))
        return TranslateLastOpenSslError();

    X509_NAME_Ptr subjectName(X509_NAME_new());
    if (!subjectName.get() ||
        !X509_NAME_add_entry_by_txt(subjectName.get(), "CN", MBSTRING_ASC,
                                    reinterpret_cast<const uint8_t*>("Android Keystore Key"),
                                    -1 /* len */, -1 /* loc */, 0 /* set */) ||
        !X509_set_subject_name(certificate.get(), subjectName.get() /* Don't release; copied */))
        return TranslateLastOpenSslError();

    ASN1_TIME_Ptr notBefore(ASN1_TIME_new());
    uint64_t activeDateTime = 0;
    key.authorizations().GetTagValue(TAG_ACTIVE_DATETIME, &activeDateTime);
    if (!notBefore.get() || !ASN1_TIME_set(notBefore.get(), activeDateTime / 1000) ||
        !X509_set_notBefore(certificate.get(), notBefore.get() /* Don't release; copied */))
        return TranslateLastOpenSslError();

    ASN1_TIME_Ptr notAfter(ASN1_TIME_new());
    uint64_t usageExpireDateTime = UINT64_MAX;
    key.authorizations().GetTagValue(TAG_USAGE_EXPIRE_DATETIME, &usageExpireDateTime);
    // TODO(swillden): When trusty can use the C++ standard library change the calculation of
    // notAfterTime to use std::numeric_limits<time_t>::max(), rather than assuming that time_t is
    // 32 bits.
    time_t notAfterTime = min(static_cast<uint64_t>(UINT32_MAX), usageExpireDateTime / 1000);
    if (!notAfter.get() || !ASN1_TIME_set(notAfter.get(), notAfterTime) ||
        !X509_set_notAfter(certificate.get(), notAfter.get() /* Don't release; copied */))
        return TranslateLastOpenSslError();

    keymaster_error_t error = add_key_usage_extension(key.hw_enforced(), key.sw_enforced(), certificate.get());
    if (error != KM_ERROR_OK) {
        return error;
    }

    // We have established above that it is one of the two. So if it is not RSA its EC.
    int evp_key_type = (sign_algorithm == KM_ALGORITHM_RSA) ? EVP_PKEY_RSA : EVP_PKEY_EC;

    const uint8_t* key_material = attestation_signing_key.key_material;
    EVP_PKEY_Ptr sign_key(
            d2i_PrivateKey(evp_key_type, nullptr,
                    const_cast<const uint8_t**>(&key_material),
                    attestation_signing_key.key_material_size));
    if (!sign_key.get()) return TranslateLastOpenSslError();

    if (!add_public_key(pkey.get(), certificate.get(), &error) ||
        !add_attestation_extension(attest_params, key.hw_enforced(), key.sw_enforced(),
                                   context, certificate.get(), &error))
        return error;

    if (attestation_chain.entry_count < 1) {
        // the attestation chain must have at least the cert for the key that signs the new cert.
        return KM_ERROR_UNKNOWN_ERROR;
    }

    const uint8_t* p = attestation_chain.entries[0].data;
    X509_Ptr signing_cert(d2i_X509(nullptr, &p, attestation_chain.entries[0].data_length));
    if (!signing_cert.get()) {
        return TranslateLastOpenSslError();
    }

    // Set issuer to subject of batch certificate.
    X509_NAME* issuerSubject = X509_get_subject_name(signing_cert.get());
    if (!issuerSubject) {
        return KM_ERROR_UNKNOWN_ERROR;
    }
    if (!X509_set_issuer_name(certificate.get(), issuerSubject)) {
        return TranslateLastOpenSslError();
    }

    UniquePtr<X509V3_CTX> x509v3_ctx(new(std::nothrow) X509V3_CTX);
    if (!x509v3_ctx.get())
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    *x509v3_ctx = {};
    X509V3_set_ctx(x509v3_ctx.get(), signing_cert.get(), certificate.get(), nullptr /* req */,
                   nullptr /* crl */, 0 /* flags */);

    X509_EXTENSION_Ptr auth_key_id(X509V3_EXT_nconf_nid(nullptr /* conf */, x509v3_ctx.get(),
                                                        NID_authority_key_identifier,
                                                        const_cast<char*>("keyid:always")));
    if (!auth_key_id.get() ||
        !X509_add_ext(certificate.get(), auth_key_id.get() /* Don't release; copied */,
                      -1 /* insert at end */)) {
        return TranslateLastOpenSslError();
    }

    if (!X509_sign(certificate.get(), sign_key.get(), EVP_sha256()))
        return TranslateLastOpenSslError();

    *cert_chain_out = makeCertChain(certificate.get(), attestation_chain);
    if (!cert_chain_out->get())
        return KM_ERROR_MEMORY_ALLOCATION_FAILED;
    return KM_ERROR_OK;
}


} // namespace keymaster
