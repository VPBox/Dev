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

#ifndef KEYSTORE_KEYSTORE_AIDL_HIDL_MARSHALLING_UTILS_H_
#define KEYSTORE_KEYSTORE_AIDL_HIDL_MARSHALLING_UTILS_H_

#include <utility>

#include <binder/Parcel.h>

#include <keystore/keymaster_types.h>

namespace keystore {

template <typename Fn, typename... Args>
inline auto nullable(Fn fn, const android::Parcel& in, Args&&... args)
    -> NullOr<decltype(fn(in, std::forward<Args>(args)...))> {
    if (in.readInt32() != 1) {
        return {};
    }

    return fn(in, std::forward<Args>(args)...);
}
template <typename Fn, typename Arg>
inline android::status_t nullable(Fn fn, const NullOr<Arg>& arg, android::Parcel* out) {
    if (!arg.isOk()) {
        return out->writeInt32(0);
    }
    auto rc = out->writeInt32(1);
    if (rc != ::android::OK) return rc;

    return fn(arg.value(), out);
}
template <typename Fn, typename Arg>
inline android::status_t nullable(Fn fn, Arg&& arg, android::Parcel* out) {
    auto rc = out->writeInt32(1);
    if (rc != ::android::OK) return rc;

    return fn(std::forward<Arg>(arg), out);
}

inline android::status_t nullable(android::Parcel* out) {
    return out->writeInt32(0);
}

/**
 * makes a copy only if inPlace is false
 */
hidl_vec<uint8_t> readKeymasterBlob(const android::Parcel& in);
android::status_t writeKeymasterBlob(const hidl_vec<uint8_t>& blob, android::Parcel* out);

NullOr<hidl_vec<uint8_t>> readBlobAsByteArray(const android::Parcel& in, bool inPlace = true);
android::status_t writeBlobAsByteArray(const NullOr<const hidl_vec<uint8_t>&>& blob,
                                       android::Parcel* out);

NullOr<KeyParameter> readKeyParameterFromParcel(const android::Parcel& in);
android::status_t writeKeyParameterToParcel(const KeyParameter& param, android::Parcel* out);

hidl_vec<KeyParameter> readParamSetFromParcel(const android::Parcel& in);
android::status_t writeParamSetToParcel(const hidl_vec<KeyParameter>& params, android::Parcel* out);

hidl_vec<hidl_vec<uint8_t>> readCertificateChainFromParcel(const android::Parcel& in);
}

#endif  // KEYSTORE_KEYSTORE_AIDL_HIDL_MARSHALLING_UTILS_H_
