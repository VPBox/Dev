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

#include <keymaster/legacy_support/keymaster1_legacy_support.h>

#include <android-base/logging.h>

#include <assert.h>

#include <algorithm>
#include <vector>

namespace keymaster {

template <typename T> std::vector<T> make_vector(const T* array, size_t len) {
    return std::vector<T>(array, array + len);
}

// This helper class implements just enough of the C++ standard collection interface to be able to
// accept push_back calls, and it does nothing but count them.  It's useful when you want to count
// insertions but not actually store anything.  It's used in digest_set_is_full below to count the
// size of a set intersection.
struct PushbackCounter {
    struct value_type {
        // NOLINTNEXTLINE(google-explicit-constructor)
        template <typename T> value_type(const T&) {}
    };
    void push_back(const value_type&) { ++count; }
    size_t count = 0;
};

static std::vector<keymaster_digest_t> full_digest_list = {
    KM_DIGEST_MD5,       KM_DIGEST_SHA1,      KM_DIGEST_SHA_2_224,
    KM_DIGEST_SHA_2_256, KM_DIGEST_SHA_2_384, KM_DIGEST_SHA_2_512};

template <typename Iter> static bool digest_set_is_full(Iter begin, Iter end) {
    PushbackCounter counter;
    std::set_intersection(begin, end, full_digest_list.begin(), full_digest_list.end(),
                          std::back_inserter(counter));
    return counter.count == full_digest_list.size();
}

static keymaster_error_t add_digests(const keymaster1_device_t* dev, keymaster_algorithm_t algorithm,
                                     keymaster_purpose_t purpose,
                                     Keymaster1LegacySupport::DigestMap* map, bool* supports_all) {
    auto key = std::make_pair(algorithm, purpose);

    keymaster_digest_t* digests;
    size_t digests_length;
    keymaster_error_t error =
        dev->get_supported_digests(dev, algorithm, purpose, &digests, &digests_length);
    if (error != KM_ERROR_OK) {
        LOG(ERROR) << "Error " << error << " getting supported digests from keymaster1 device";
        return error;
    }
    std::unique_ptr<keymaster_digest_t, Malloc_Delete> digests_deleter(digests);

    auto digest_vec = make_vector(digests, digests_length);
    *supports_all = digest_set_is_full(digest_vec.begin(), digest_vec.end());
    (*map)[key] = std::move(digest_vec);
    return error;
}

static keymaster_error_t map_digests(const keymaster1_device_t* dev,
                                     Keymaster1LegacySupport::DigestMap* map,
                                     bool* supports_all) {
    map->clear();
    *supports_all = true;

    keymaster_algorithm_t sig_algorithms[] = {KM_ALGORITHM_RSA, KM_ALGORITHM_EC, KM_ALGORITHM_HMAC};
    keymaster_purpose_t sig_purposes[] = {KM_PURPOSE_SIGN, KM_PURPOSE_VERIFY};
    for (auto algorithm : sig_algorithms)
        for (auto purpose : sig_purposes) {
            bool alg_purpose_supports_all;
            keymaster_error_t error =
                add_digests(dev, algorithm, purpose, map, &alg_purpose_supports_all);
            if (error != KM_ERROR_OK)
                return error;
            *supports_all &= alg_purpose_supports_all;
        }

    keymaster_algorithm_t crypt_algorithms[] = {KM_ALGORITHM_RSA};
    keymaster_purpose_t crypt_purposes[] = {KM_PURPOSE_ENCRYPT, KM_PURPOSE_DECRYPT};
    for (auto algorithm : crypt_algorithms)
        for (auto purpose : crypt_purposes) {
            bool alg_purpose_supports_all;
            keymaster_error_t error =
                add_digests(dev, algorithm, purpose, map, &alg_purpose_supports_all);
            if (error != KM_ERROR_OK)
                return error;
            *supports_all &= alg_purpose_supports_all;
        }

    return KM_ERROR_OK;
}

Keymaster1LegacySupport::Keymaster1LegacySupport(const keymaster1_device_t* dev) {
    map_digests(dev, &device_digests_, &supports_all_);
}

template <typename Collection, typename Value> bool contains(const Collection& c, const Value& v) {
    return std::find(c.begin(), c.end(), v) != c.end();
}

template <typename T>
static bool findUnsupportedDigest(keymaster_algorithm_t algorithm,
                                  keymaster_purpose_t purpose,
                                  keymaster_digest_t digest,
                                  const T& params,
                                  const Keymaster1LegacySupport::DigestMap& digest_map) {
    auto supported_digests = digest_map.find(std::make_pair(algorithm, purpose));
    if (supported_digests == digest_map.end())
        // Invalid algorith/purpose pair (e.g. EC encrypt).  Let the error be handled by HW module.
        return false;

    if (digest != KM_DIGEST_NONE && !contains(supported_digests->second, digest)) {
        LOG(WARNING) << "Digest " << digest << " requested but not supported by KM1 hal";
        return true;
    }

    for (auto& entry : params)
        if (entry.tag == TAG_DIGEST)
            if (!contains(supported_digests->second, entry.enumerated)) {
                LOG(WARNING) << "Digest " << entry.enumerated << " requested but not supported by KM1 hal";
                return true;
            }
    return false;
}

template <typename T>
bool requiresSoftwareDigesting(keymaster_algorithm_t algorithm, keymaster_purpose_t purpose,
                               keymaster_digest_t digest,
                               const T& params,
                               const Keymaster1LegacySupport::DigestMap& digest_map) {
    switch (algorithm) {
    case KM_ALGORITHM_AES:
    case KM_ALGORITHM_TRIPLE_DES:
        LOG(WARNING) << "Not performing software digesting for symmetric cipher keys";
        return false;
    case KM_ALGORITHM_HMAC:
    case KM_ALGORITHM_RSA:
    case KM_ALGORITHM_EC:
        break;
    }

    if (!findUnsupportedDigest(algorithm, purpose, digest, params, digest_map)) {
        LOG(DEBUG) << "Requested digest(s) supported for algorithm " << algorithm << " and purpose " << purpose;
        return false;
    }

    return true;
}
bool Keymaster1LegacySupport::RequiresSoftwareDigesting(
        const AuthorizationSet& key_description) const {

    keymaster_algorithm_t algorithm;
    if (!key_description.GetTagValue(TAG_ALGORITHM, &algorithm)) {
        // The hardware module will return an error during keygen.
        return false;
    }

    if (supports_all_) return false;

    bool has_purpose = false;
    for (auto& entry : key_description)
        if (entry.tag == TAG_PURPOSE) {
            has_purpose = true;
            keymaster_purpose_t purpose = static_cast<keymaster_purpose_t>(entry.enumerated);
            if (requiresSoftwareDigesting(algorithm, purpose, KM_DIGEST_NONE, key_description,
                                          device_digests_))
                return true;
        }

    return !has_purpose;
}

bool Keymaster1LegacySupport::RequiresSoftwareDigesting(const keymaster_digest_t digest,
        const AuthProxy& key_description) const {

    keymaster_algorithm_t algorithm;
    if (!key_description.GetTagValue(TAG_ALGORITHM, &algorithm)) {
        // The hardware module will return an error during keygen.
        return false;
    }

    if (supports_all_) return false;

    bool has_purpose = false;
    for (auto& entry : key_description) {
        if (entry.tag == TAG_PURPOSE) {
            has_purpose = true;
            keymaster_purpose_t purpose = static_cast<keymaster_purpose_t>(entry.enumerated);
            if (requiresSoftwareDigesting(algorithm, purpose, digest, key_description,
                                          device_digests_))
                return true;
        }
    }

    /*
     * If the key does not have a purpose it is unusable, i.e., for private key operations.
     * The public key operations which don't need purpose authorization may as well be done
     * in software. This also addresses a bug by which begin operation on keys without purpose and
     * unauthorized digest which is also not supported by the wrapped KM1 device fail with
     * KM_UNSUPPORTED_DIGEST although they should not fail during the begin operation.
     * If it has a purpose and we reach this point we did not find unsupported digests, and
     * therefore do not required software digesting.
     */
    return !has_purpose;
}

template<>
keymaster_error_t
Keymaster1ArbitrationFactory<EcdsaKeymaster1KeyFactory>::GenerateKey(
        const AuthorizationSet& key_description,
        KeymasterKeyBlob* key_blob, AuthorizationSet* hw_enforced,
        AuthorizationSet* sw_enforced) const {
    if (legacy_support_.RequiresSoftwareDigesting(key_description)) {
        return software_digest_factory_.GenerateKey(key_description, key_blob, hw_enforced,
                                             sw_enforced);
    } else {
        AuthorizationSet mutable_key_description = key_description;
        keymaster_ec_curve_t curve;
        if (key_description.GetTagValue(TAG_EC_CURVE, &curve)) {
            // Keymaster1 doesn't know about EC curves. We need to translate to key size.
            uint32_t key_size_from_curve;
            keymaster_error_t error = EcCurveToKeySize(curve, &key_size_from_curve);
            if (error != KM_ERROR_OK) {
                return error;
            }

            uint32_t key_size_from_desc;
            if (key_description.GetTagValue(TAG_KEY_SIZE, &key_size_from_desc)) {
                if (key_size_from_desc != key_size_from_curve) {
                    return KM_ERROR_INVALID_ARGUMENT;
                }
            } else {
                mutable_key_description.push_back(TAG_KEY_SIZE, key_size_from_curve);
            }
        }

        return passthrough_factory_.GenerateKey(mutable_key_description, key_blob, hw_enforced,
                                                sw_enforced);
    }
}

template<>
keymaster_error_t
Keymaster1ArbitrationFactory<EcdsaKeymaster1KeyFactory>::LoadKey(KeymasterKeyBlob&& key_material,
        const AuthorizationSet& additional_params,
        AuthorizationSet&& hw_enforced,
        AuthorizationSet&& sw_enforced,
        UniquePtr<Key>* key) const {
    keymaster_digest_t digest;
    if (!additional_params.GetTagValue(TAG_DIGEST, &digest)) {
        digest = KM_DIGEST_NONE;
    }
    bool requires_software_digesting = legacy_support_.RequiresSoftwareDigesting(digest,
                                                           AuthProxy(hw_enforced, sw_enforced));
    auto rc = software_digest_factory_.LoadKey(move(key_material), additional_params,
                                               move(hw_enforced), move(sw_enforced), key);
    if (rc != KM_ERROR_OK) return rc;
    if (!requires_software_digesting) {
        (*key)->key_factory() = & passthrough_factory_;
    }
    return KM_ERROR_OK;
}

template<>
keymaster_error_t
Keymaster1ArbitrationFactory<RsaKeymaster1KeyFactory>::LoadKey(KeymasterKeyBlob&& key_material,
        const AuthorizationSet& additional_params,
        AuthorizationSet&& hw_enforced,
        AuthorizationSet&& sw_enforced,
        UniquePtr<Key>* key) const {
    keymaster_digest_t digest;
    if (!additional_params.GetTagValue(TAG_DIGEST, &digest)) {
        digest = KM_DIGEST_NONE;
    }
    bool requires_software_digesting = legacy_support_.RequiresSoftwareDigesting(digest,
                                                           AuthProxy(hw_enforced, sw_enforced));
    auto rc = software_digest_factory_.LoadKey(move(key_material), additional_params,
                                               move(hw_enforced), move(sw_enforced), key);
    if (rc != KM_ERROR_OK) return rc;
    if (!requires_software_digesting) {
        (*key)->key_factory() = & passthrough_factory_;
    }
    return KM_ERROR_OK;
}


} // namespace keymaster
