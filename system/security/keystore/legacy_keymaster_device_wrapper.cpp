/*
 **
 ** Copyright 2016, The Android Open Source Project
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

#define LOG_TAG "android.hardware.keymaster@3.0-impl"

#include "legacy_keymaster_device_wrapper.h"

#include <log/log.h>

#include <hardware/keymaster2.h>
#include <hardware/keymaster_defs.h>
#include <keymaster/keymaster_configuration.h>
#include <keymaster/soft_keymaster_device.h>

namespace android {
namespace keystore {

using ::keymaster::SoftKeymasterDevice;

LegacyKeymasterDeviceWrapper::LegacyKeymasterDeviceWrapper(keymaster2_device_t* dev)
    : keymaster_device_(dev) {}

LegacyKeymasterDeviceWrapper::~LegacyKeymasterDeviceWrapper() {
    if (keymaster_device_) keymaster_device_->common.close(&keymaster_device_->common);
}

static inline keymaster_tag_type_t typeFromTag(const keymaster_tag_t tag) {
    return keymaster_tag_get_type(tag);
}

/**
 * legacy_enum_conversion converts enums from hidl to keymaster and back. Currently, this is just a
 * cast to make the compiler happy. One of two thigs should happen though:
 * TODO The keymaster enums should become aliases for the hidl generated enums so that we have a
 *      single point of truth. Then this cast function can go away.
 */
inline static keymaster_tag_t legacy_enum_conversion(const Tag value) {
    return keymaster_tag_t(value);
}
inline static Tag legacy_enum_conversion(const keymaster_tag_t value) {
    return Tag(value);
}
inline static keymaster_purpose_t legacy_enum_conversion(const KeyPurpose value) {
    return keymaster_purpose_t(value);
}
inline static keymaster_key_format_t legacy_enum_conversion(const KeyFormat value) {
    return keymaster_key_format_t(value);
}
inline static ErrorCode legacy_enum_conversion(const keymaster_error_t value) {
    return ErrorCode(value);
}

class KmParamSet : public keymaster_key_param_set_t {
  public:
    explicit KmParamSet(const hidl_vec<KeyParameter>& keyParams) {
        params = new keymaster_key_param_t[keyParams.size()];
        length = keyParams.size();
        for (size_t i = 0; i < keyParams.size(); ++i) {
            auto tag = legacy_enum_conversion(keyParams[i].tag);
            switch (typeFromTag(tag)) {
            case KM_ENUM:
            case KM_ENUM_REP:
                params[i] = keymaster_param_enum(tag, keyParams[i].f.integer);
                break;
            case KM_UINT:
            case KM_UINT_REP:
                params[i] = keymaster_param_int(tag, keyParams[i].f.integer);
                break;
            case KM_ULONG:
            case KM_ULONG_REP:
                params[i] = keymaster_param_long(tag, keyParams[i].f.longInteger);
                break;
            case KM_DATE:
                params[i] = keymaster_param_date(tag, keyParams[i].f.dateTime);
                break;
            case KM_BOOL:
                if (keyParams[i].f.boolValue)
                    params[i] = keymaster_param_bool(tag);
                else
                    params[i].tag = KM_TAG_INVALID;
                break;
            case KM_BIGNUM:
            case KM_BYTES:
                params[i] =
                    keymaster_param_blob(tag, &keyParams[i].blob[0], keyParams[i].blob.size());
                break;
            case KM_INVALID:
            default:
                params[i].tag = KM_TAG_INVALID;
                /* just skip */
                break;
            }
        }
    }
    KmParamSet(KmParamSet&& other) noexcept
        : keymaster_key_param_set_t{other.params, other.length} {
        other.length = 0;
        other.params = nullptr;
    }
    KmParamSet(const KmParamSet&) = delete;
    ~KmParamSet() { delete[] params; }
};

inline static KmParamSet hidlParams2KmParamSet(const hidl_vec<KeyParameter>& params) {
    return KmParamSet(params);
}

inline static keymaster_blob_t hidlVec2KmBlob(const hidl_vec<uint8_t>& blob) {
    /* hidl unmarshals funny pointers if the the blob is empty */
    if (blob.size()) return {&blob[0], blob.size()};
    return {};
}

inline static keymaster_key_blob_t hidlVec2KmKeyBlob(const hidl_vec<uint8_t>& blob) {
    /* hidl unmarshals funny pointers if the the blob is empty */
    if (blob.size()) return {&blob[0], blob.size()};
    return {};
}

inline static hidl_vec<uint8_t> kmBlob2hidlVec(const keymaster_key_blob_t& blob) {
    if (blob.key_material == nullptr || blob.key_material_size == 0) {
        return {};
    } else {
        return hidl_vec<uint8_t>(blob.key_material, blob.key_material + blob.key_material_size);
    }
}
inline static hidl_vec<uint8_t> kmBlob2hidlVec(const keymaster_blob_t& blob) {
    if (blob.data == nullptr || blob.data_length == 0) {
        return {};
    } else {
        return hidl_vec<uint8_t>(blob.data, blob.data + blob.data_length);
    }
}

inline static hidl_vec<hidl_vec<uint8_t>>
kmCertChain2Hidl(const keymaster_cert_chain_t* cert_chain) {
    hidl_vec<hidl_vec<uint8_t>> result;
    if (!cert_chain || cert_chain->entry_count == 0 || !cert_chain->entries) return result;

    result.resize(cert_chain->entry_count);
    for (size_t i = 0; i < cert_chain->entry_count; ++i) {
        auto& entry = cert_chain->entries[i];
        result[i] = kmBlob2hidlVec(entry);
    }

    return result;
}

static inline hidl_vec<KeyParameter> kmParamSet2Hidl(const keymaster_key_param_set_t& set) {
    hidl_vec<KeyParameter> result;
    if (set.length == 0 || set.params == nullptr) return result;

    result.resize(set.length);
    keymaster_key_param_t* params = set.params;
    for (size_t i = 0; i < set.length; ++i) {
        auto tag = params[i].tag;
        result[i].tag = legacy_enum_conversion(tag);
        switch (typeFromTag(tag)) {
        case KM_ENUM:
        case KM_ENUM_REP:
            result[i].f.integer = params[i].enumerated;
            break;
        case KM_UINT:
        case KM_UINT_REP:
            result[i].f.integer = params[i].integer;
            break;
        case KM_ULONG:
        case KM_ULONG_REP:
            result[i].f.longInteger = params[i].long_integer;
            break;
        case KM_DATE:
            result[i].f.dateTime = params[i].date_time;
            break;
        case KM_BOOL:
            result[i].f.boolValue = params[i].boolean;
            break;
        case KM_BIGNUM:
        case KM_BYTES:
            result[i].blob = kmBlob2hidlVec(params[i].blob);
            break;
        case KM_INVALID:
        default:
            params[i].tag = KM_TAG_INVALID;
            /* just skip */
            break;
        }
    }
    return result;
}

// Methods from ::android::hardware::keymaster::V3_0::IKeymasterDevice follow.
Return<void> LegacyKeymasterDeviceWrapper::getHardwareFeatures(getHardwareFeatures_cb _hidl_cb) {
    _hidl_cb(false, false, false, false, false, "Fallback Device", "Google Android Security");
    return Void();
}

Return<ErrorCode> LegacyKeymasterDeviceWrapper::addRngEntropy(const hidl_vec<uint8_t>& data) {
    return legacy_enum_conversion(
        keymaster_device_->add_rng_entropy(keymaster_device_, &data[0], data.size()));
}

Return<void> LegacyKeymasterDeviceWrapper::generateKey(const hidl_vec<KeyParameter>& keyParams,
                                                       generateKey_cb _hidl_cb) {
    // result variables for the wire
    KeyCharacteristics resultCharacteristics;
    hidl_vec<uint8_t> resultKeyBlob;

    // result variables the backend understands
    keymaster_key_blob_t key_blob{nullptr, 0};
    keymaster_key_characteristics_t key_characteristics{{nullptr, 0}, {nullptr, 0}};

    // convert the parameter set to something our backend understands
    auto kmParams = hidlParams2KmParamSet(keyParams);

    auto rc = keymaster_device_->generate_key(keymaster_device_, &kmParams, &key_blob,
                                              &key_characteristics);

    if (rc == KM_ERROR_OK) {
        // on success convert the result to wire format
        resultKeyBlob = kmBlob2hidlVec(key_blob);
        resultCharacteristics.softwareEnforced = kmParamSet2Hidl(key_characteristics.sw_enforced);
        resultCharacteristics.teeEnforced = kmParamSet2Hidl(key_characteristics.hw_enforced);
    }

    // send results off to the client
    _hidl_cb(legacy_enum_conversion(rc), resultKeyBlob, resultCharacteristics);

    // free buffers that we are responsible for
    if (key_blob.key_material) free(const_cast<uint8_t*>(key_blob.key_material));
    keymaster_free_characteristics(&key_characteristics);

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::getKeyCharacteristics(
    const hidl_vec<uint8_t>& keyBlob, const hidl_vec<uint8_t>& clientId,
    const hidl_vec<uint8_t>& appData, getKeyCharacteristics_cb _hidl_cb) {
    // result variables for the wire
    KeyCharacteristics resultCharacteristics;

    // result variables the backend understands
    keymaster_key_characteristics_t key_characteristics{{nullptr, 0}, {nullptr, 0}};

    auto kmKeyBlob = hidlVec2KmKeyBlob(keyBlob);
    auto kmClientId = hidlVec2KmBlob(clientId);
    auto kmAppData = hidlVec2KmBlob(appData);

    auto rc = keymaster_device_->get_key_characteristics(
        keymaster_device_, keyBlob.size() ? &kmKeyBlob : nullptr,
        clientId.size() ? &kmClientId : nullptr, appData.size() ? &kmAppData : nullptr,
        &key_characteristics);

    if (rc == KM_ERROR_OK) {
        resultCharacteristics.softwareEnforced = kmParamSet2Hidl(key_characteristics.sw_enforced);
        resultCharacteristics.teeEnforced = kmParamSet2Hidl(key_characteristics.hw_enforced);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultCharacteristics);

    keymaster_free_characteristics(&key_characteristics);

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::importKey(const hidl_vec<KeyParameter>& params,
                                                     KeyFormat keyFormat,
                                                     const hidl_vec<uint8_t>& keyData,
                                                     importKey_cb _hidl_cb) {
    // result variables for the wire
    KeyCharacteristics resultCharacteristics;
    hidl_vec<uint8_t> resultKeyBlob;

    // result variables the backend understands
    keymaster_key_blob_t key_blob{nullptr, 0};
    keymaster_key_characteristics_t key_characteristics{{nullptr, 0}, {nullptr, 0}};

    auto kmParams = hidlParams2KmParamSet(params);
    auto kmKeyData = hidlVec2KmBlob(keyData);

    auto rc = keymaster_device_->import_key(keymaster_device_, &kmParams,
                                            legacy_enum_conversion(keyFormat), &kmKeyData,
                                            &key_blob, &key_characteristics);

    if (rc == KM_ERROR_OK) {
        // on success convert the result to wire format
        resultKeyBlob = kmBlob2hidlVec(key_blob);
        resultCharacteristics.softwareEnforced = kmParamSet2Hidl(key_characteristics.sw_enforced);
        resultCharacteristics.teeEnforced = kmParamSet2Hidl(key_characteristics.hw_enforced);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultKeyBlob, resultCharacteristics);

    // free buffers that we are responsible for
    if (key_blob.key_material) free(const_cast<uint8_t*>(key_blob.key_material));
    keymaster_free_characteristics(&key_characteristics);

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::exportKey(KeyFormat exportFormat,
                                                     const hidl_vec<uint8_t>& keyBlob,
                                                     const hidl_vec<uint8_t>& clientId,
                                                     const hidl_vec<uint8_t>& appData,
                                                     exportKey_cb _hidl_cb) {

    // result variables for the wire
    hidl_vec<uint8_t> resultKeyBlob;

    // result variables the backend understands
    keymaster_blob_t out_blob = {};

    auto kmKeyBlob = hidlVec2KmKeyBlob(keyBlob);
    auto kmClientId = hidlVec2KmBlob(clientId);
    auto kmAppData = hidlVec2KmBlob(appData);

    auto rc = keymaster_device_->export_key(keymaster_device_, legacy_enum_conversion(exportFormat),
                                            keyBlob.size() ? &kmKeyBlob : nullptr,
                                            clientId.size() ? &kmClientId : nullptr,
                                            appData.size() ? &kmAppData : nullptr, &out_blob);

    if (rc == KM_ERROR_OK) {
        // on success convert the result to wire format
        // (Can we assume that key_blob is {nullptr, 0} or a valid buffer description?)
        resultKeyBlob = kmBlob2hidlVec(out_blob);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultKeyBlob);

    // free buffers that we are responsible for
    if (out_blob.data) free(const_cast<uint8_t*>(out_blob.data));

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::attestKey(const hidl_vec<uint8_t>& keyToAttest,
                                                     const hidl_vec<KeyParameter>& attestParams,
                                                     attestKey_cb _hidl_cb) {

    hidl_vec<hidl_vec<uint8_t>> resultCertChain;

    for (size_t i = 0; i < attestParams.size(); ++i) {
        switch (attestParams[i].tag) {
            case Tag::ATTESTATION_ID_BRAND:
            case Tag::ATTESTATION_ID_DEVICE:
            case Tag::ATTESTATION_ID_PRODUCT:
            case Tag::ATTESTATION_ID_SERIAL:
            case Tag::ATTESTATION_ID_IMEI:
            case Tag::ATTESTATION_ID_MEID:
            case Tag::ATTESTATION_ID_MANUFACTURER:
            case Tag::ATTESTATION_ID_MODEL:
                // Device id attestation may only be supported if the device is able to permanently
                // destroy its knowledge of the ids. This device is unable to do this, so it must
                // never perform any device id attestation.
                _hidl_cb(ErrorCode::CANNOT_ATTEST_IDS, resultCertChain);
                return Void();
            default:
                break;
        }
    }

    keymaster_cert_chain_t cert_chain = {};

    auto kmKeyToAttest = hidlVec2KmKeyBlob(keyToAttest);
    auto kmAttestParams = hidlParams2KmParamSet(attestParams);

    auto rc = keymaster_device_->attest_key(keymaster_device_, &kmKeyToAttest, &kmAttestParams,
                                            &cert_chain);

    if (rc == KM_ERROR_OK) {
        resultCertChain = kmCertChain2Hidl(&cert_chain);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultCertChain);

    keymaster_free_cert_chain(&cert_chain);

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::upgradeKey(const hidl_vec<uint8_t>& keyBlobToUpgrade,
                                                      const hidl_vec<KeyParameter>& upgradeParams,
                                                      upgradeKey_cb _hidl_cb) {

    // result variables for the wire
    hidl_vec<uint8_t> resultKeyBlob;

    // result variables the backend understands
    keymaster_key_blob_t key_blob = {};

    auto kmKeyBlobToUpgrade = hidlVec2KmKeyBlob(keyBlobToUpgrade);
    auto kmUpgradeParams = hidlParams2KmParamSet(upgradeParams);

    auto rc = keymaster_device_->upgrade_key(keymaster_device_, &kmKeyBlobToUpgrade,
                                             &kmUpgradeParams, &key_blob);

    if (rc == KM_ERROR_OK) {
        // on success convert the result to wire format
        resultKeyBlob = kmBlob2hidlVec(key_blob);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultKeyBlob);

    if (key_blob.key_material) free(const_cast<uint8_t*>(key_blob.key_material));

    return Void();
}

Return<ErrorCode> LegacyKeymasterDeviceWrapper::deleteKey(const hidl_vec<uint8_t>& keyBlob) {
    auto kmKeyBlob = hidlVec2KmKeyBlob(keyBlob);
    return legacy_enum_conversion(keymaster_device_->delete_key(keymaster_device_, &kmKeyBlob));
}

Return<ErrorCode> LegacyKeymasterDeviceWrapper::deleteAllKeys() {
    return legacy_enum_conversion(keymaster_device_->delete_all_keys(keymaster_device_));
}

Return<ErrorCode> LegacyKeymasterDeviceWrapper::destroyAttestationIds() {
    return ErrorCode::UNIMPLEMENTED;
}

Return<void> LegacyKeymasterDeviceWrapper::begin(KeyPurpose purpose, const hidl_vec<uint8_t>& key,
                                                 const hidl_vec<KeyParameter>& inParams,
                                                 begin_cb _hidl_cb) {

    // result variables for the wire
    hidl_vec<KeyParameter> resultParams;
    uint64_t resultOpHandle = 0;

    // result variables the backend understands
    keymaster_key_param_set_t out_params{nullptr, 0};
    keymaster_operation_handle_t& operation_handle = resultOpHandle;

    auto kmKey = hidlVec2KmKeyBlob(key);
    auto kmInParams = hidlParams2KmParamSet(inParams);

    auto rc = keymaster_device_->begin(keymaster_device_, legacy_enum_conversion(purpose), &kmKey,
                                       &kmInParams, &out_params, &operation_handle);

    if (rc == KM_ERROR_OK) resultParams = kmParamSet2Hidl(out_params);

    _hidl_cb(legacy_enum_conversion(rc), resultParams, resultOpHandle);

    keymaster_free_param_set(&out_params);

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::update(uint64_t operationHandle,
                                                  const hidl_vec<KeyParameter>& inParams,
                                                  const hidl_vec<uint8_t>& input,
                                                  update_cb _hidl_cb) {
    // result variables for the wire
    uint32_t resultConsumed = 0;
    hidl_vec<KeyParameter> resultParams;
    hidl_vec<uint8_t> resultBlob;

    // result variables the backend understands
    size_t consumed = 0;
    keymaster_key_param_set_t out_params = {};
    keymaster_blob_t out_blob = {};

    auto kmInParams = hidlParams2KmParamSet(inParams);
    auto kmInput = hidlVec2KmBlob(input);

    auto rc = keymaster_device_->update(keymaster_device_, operationHandle, &kmInParams, &kmInput,
                                        &consumed, &out_params, &out_blob);

    if (rc == KM_ERROR_OK) {
        resultConsumed = consumed;
        resultParams = kmParamSet2Hidl(out_params);
        resultBlob = kmBlob2hidlVec(out_blob);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultConsumed, resultParams, resultBlob);

    keymaster_free_param_set(&out_params);
    if (out_blob.data) free(const_cast<uint8_t*>(out_blob.data));

    return Void();
}

Return<void> LegacyKeymasterDeviceWrapper::finish(uint64_t operationHandle,
                                                  const hidl_vec<KeyParameter>& inParams,
                                                  const hidl_vec<uint8_t>& input,
                                                  const hidl_vec<uint8_t>& signature,
                                                  finish_cb _hidl_cb) {
    // result variables for the wire
    hidl_vec<KeyParameter> resultParams;
    hidl_vec<uint8_t> resultBlob;

    // result variables the backend understands
    keymaster_key_param_set_t out_params = {};
    keymaster_blob_t out_blob = {};

    auto kmInParams = hidlParams2KmParamSet(inParams);
    auto kmInput = hidlVec2KmBlob(input);
    auto kmSignature = hidlVec2KmBlob(signature);

    auto rc = keymaster_device_->finish(keymaster_device_, operationHandle, &kmInParams, &kmInput,
                                        &kmSignature, &out_params, &out_blob);

    if (rc == KM_ERROR_OK) {
        resultParams = kmParamSet2Hidl(out_params);
        resultBlob = kmBlob2hidlVec(out_blob);
    }

    _hidl_cb(legacy_enum_conversion(rc), resultParams, resultBlob);

    keymaster_free_param_set(&out_params);
    if (out_blob.data) free(const_cast<uint8_t*>(out_blob.data));

    return Void();
}

Return<ErrorCode> LegacyKeymasterDeviceWrapper::abort(uint64_t operationHandle) {
    return legacy_enum_conversion(keymaster_device_->abort(keymaster_device_, operationHandle));
}

sp<IKeymasterDevice> makeSoftwareKeymasterDevice() {
    keymaster2_device_t* dev = nullptr;
    dev = (new SoftKeymasterDevice)->keymaster2_device();

    auto kmrc = ::keymaster::ConfigureDevice(dev);
    if (kmrc != KM_ERROR_OK) {
        dev->common.close(&dev->common);
        return nullptr;
    }

    return new LegacyKeymasterDeviceWrapper(dev);
}

}  // namespace keystore
}  // namespace android
