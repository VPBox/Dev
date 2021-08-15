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

#include "keystore_aidl_hidl_marshalling_utils.h"

#include <keystore/ExportResult.h>
#include <keystore/KeyCharacteristics.h>
#include <keystore/KeymasterBlob.h>
#include <keystore/KeymasterCertificateChain.h>
#include <keystore/keymaster_types.h>
#include <keystore/keystore_hidl_support.h>

namespace keystore {

// reads byte[]
hidl_vec<uint8_t> readKeymasterBlob(const android::Parcel& in) {

    ssize_t length = in.readInt32();
    if (length <= 0) {
        return {};
    }

    const void* buf = in.readInplace(length);
    if (!buf) return {};

    return blob2hidlVec(reinterpret_cast<const uint8_t*>(buf), size_t(length));
}

android::status_t writeKeymasterBlob(const hidl_vec<uint8_t>& blob, android::Parcel* out) {
    int32_t size = int32_t(std::min<size_t>(blob.size(), std::numeric_limits<int32_t>::max()));

    auto rc = out->writeInt32(size);
    if (rc != ::android::OK) return rc;

    if (!size) return ::android::OK;

    return out->write(blob.data(), size);
}

android::status_t writeKeymasterBlob(const ::std::vector<int32_t>& blob, android::Parcel* out) {

    int32_t size = int32_t(std::min<size_t>(blob.size(), std::numeric_limits<int32_t>::max()));

    auto rc = out->writeInt32(size);
    if (rc != ::android::OK) return rc;

    if (!size) return ::android::OK;

    return out->write(blob.data(), size);
}

NullOr<KeyParameter> readKeyParameterFromParcel(const android::Parcel& in) {
    // Method must be in sync with KeymasterArgument.java
    if (in.readInt32() == 0) {
        return {};
    }
    KeyParameter result;

    Tag tag = static_cast<Tag>(in.readInt32());
    result.tag = tag;
    switch (typeFromTag(tag)) {
    case TagType::ENUM:
    case TagType::ENUM_REP:
    case TagType::UINT:
    case TagType::UINT_REP:
        result.f.integer = in.readInt32();
        break;
    case TagType::ULONG:
    case TagType::ULONG_REP:
    case TagType::DATE:
        result.f.longInteger = in.readInt64();
        break;
    case TagType::BOOL:
        result.f.boolValue = true;
        break;
    case TagType::BIGNUM:
    case TagType::BYTES:
        result.blob = readKeymasterBlob(in);  // byte array
        break;
    default:
        ALOGE("Unsupported KeyParameter tag %d", tag);
        return {};
    }
    return result;
}

android::status_t writeKeyParameterToParcel(const KeyParameter& param, android::Parcel* out) {
    // Method must be in sync with with KeymasterArgument.java
    // Presence flag must be written by caller.

    auto tag = param.tag;
    auto rc = out->writeInt32(uint32_t(tag));
    if (rc != ::android::OK) return rc;
    switch (typeFromTag(param.tag)) {
    case TagType::ENUM:
    case TagType::ENUM_REP:
    case TagType::UINT:
    case TagType::UINT_REP:
        rc = out->writeInt32(param.f.integer);
        break;
    case TagType::ULONG:
    case TagType::ULONG_REP:
    case TagType::DATE:
        rc = out->writeInt64(param.f.longInteger);
        break;
    case TagType::BOOL:
        // nothing to do here presence indicates true
        break;
    case TagType::BIGNUM:
    case TagType::BYTES:
        rc = writeKeymasterBlob(param.blob, out);
        break;
    default:
        ALOGE("Failed to write KeyParameter: Unsupported tag %d", param.tag);
        rc = android::BAD_VALUE;
        break;
    }
    return rc;
}

hidl_vec<KeyParameter> readParamSetFromParcel(const android::Parcel& in) {

    ssize_t length = in.readInt32();  // -1 for null
    size_t ulength = (size_t)length;
    if (length < 0) {
        ulength = 0;
    }
    hidl_vec<KeyParameter> result;
    result.resize(ulength);
    for (size_t i = 0; i < ulength; ++i) {
        auto param = readKeyParameterFromParcel(in);
        if (!param.isOk()) {
            ALOGE("Error reading KeyParameter from parcel");
            return {};
        }
        result[i] = param.value();
    }
    return result;
}

android::status_t writeParamSetToParcel(const hidl_vec<KeyParameter>& params,
                                        android::Parcel* out) {
    int32_t size = int32_t(std::min<size_t>(params.size(), std::numeric_limits<int32_t>::max()));

    auto rc = out->writeInt32(size);
    if (rc != ::android::OK) return rc;
    for (int32_t i = 0; i < size; ++i) {
        rc = out->writeInt32(1);  // writeTypedObject presence flag.
        if (rc != ::android::OK) return rc;
        rc = writeKeyParameterToParcel(params[i], out);
        if (rc != ::android::OK) return rc;
    }
    return rc;
}

hidl_vec<hidl_vec<uint8_t>> readCertificateChainFromParcel(const android::Parcel& in) {
    hidl_vec<hidl_vec<uint8_t>> result;

    ssize_t count = in.readInt32();
    size_t ucount = count;
    if (count <= 0) {
        return result;
    }

    result.resize(ucount);

    for (size_t i = 0; i < ucount; ++i) {
        result[i] = readKeymasterBlob(in);
    }
    return result;
};

android::status_t writeCertificateChainToParcel(const hidl_vec<hidl_vec<uint8_t>>& certs,
                                                android::Parcel* out) {
    int32_t count = int32_t(std::min<size_t>(certs.size(), std::numeric_limits<int32_t>::max()));
    auto rc = out->writeInt32(count);

    for (int32_t i = 0; i < count; ++i) {
        rc = writeKeymasterBlob(certs[i], out);
        if (rc != ::android::OK) return rc;
    }
    return rc;
}

};  // namespace keystore

// Implementation for  keystore parcelables.
// TODO: split implementation into separate classes
namespace android {
namespace security {
namespace keymaster {

using ::android::status_t;
using ::keystore::keymaster::ErrorCode;

ExportResult::ExportResult() : resultCode() {}

ExportResult::~ExportResult() {}

status_t ExportResult::readFromParcel(const Parcel* inn) {
    const Parcel& in = *inn;
    resultCode = ErrorCode(in.readInt32());
    exportData = keystore::readKeymasterBlob(in);
    return OK;
}

status_t ExportResult::writeToParcel(Parcel* out) const {
    out->writeInt32(resultCode.getErrorCode());
    return keystore::writeKeymasterBlob(exportData, out);
}

status_t KeyCharacteristics::readFromParcel(const Parcel* in) {
    softwareEnforced.readFromParcel(in);
    return hardwareEnforced.readFromParcel(in);
}

status_t KeyCharacteristics::writeToParcel(Parcel* out) const {
    softwareEnforced.writeToParcel(out);
    return hardwareEnforced.writeToParcel(out);
}

status_t KeymasterBlob::readFromParcel(const Parcel* in) {
    data_ = keystore::readKeymasterBlob(*in);
    return OK;
}

status_t KeymasterBlob::writeToParcel(Parcel* out) const {
    return keystore::writeKeymasterBlob(data_, out);
}

status_t KeymasterCertificateChain::readFromParcel(const Parcel* in) {
    chain = keystore::readCertificateChainFromParcel(*in);
    return OK;
}

status_t KeymasterCertificateChain::writeToParcel(Parcel* out) const {
    return keystore::writeCertificateChainToParcel(chain, out);
}

}  // namespace keymaster
}  // namespace security

}  // namespace android
