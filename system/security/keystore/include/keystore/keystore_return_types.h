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

#ifndef KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_RETURN_TYPES_H_
#define KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_RETURN_TYPES_H_

#include "keymaster_types.h"
#include "keystore.h"

namespace keystore {

using keymaster::ErrorCode;

class KeyStoreServiceReturnCode;
class KeyStoreNativeReturnCode;

/**
 * The keystore service return code is a bit tricky. It can return error codes from two name spaces:
 * ErrorCode, which has negative error codes and use 0 for ERROR_OK;
 * ResponseCode, which has positive error codes and uses 1 for NO_ERROR.
 * This class can be initialized by both. And when accessed through the operator int32_t () it
 * always returns ResponseCode::NO_ERROR (1) on success, even if it was initialized with
 * ErrorCode::OK (0), because this is what (java) clients expect.
 *
 * !!! Do not confuse this with KeyStoreNativeReturnCode which always converts to 0 on success. !!!
 */
class KeyStoreServiceReturnCode {
  public:
    KeyStoreServiceReturnCode() : errorCode_(0) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreServiceReturnCode(const ErrorCode& errorCode) : errorCode_(int32_t(errorCode)) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreServiceReturnCode(const ResponseCode& errorCode) : errorCode_(int32_t(errorCode)) {}
    KeyStoreServiceReturnCode(const KeyStoreServiceReturnCode& errorCode)
        : errorCode_(errorCode.errorCode_) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreServiceReturnCode(const KeyStoreNativeReturnCode& errorCode);
    explicit inline KeyStoreServiceReturnCode(const int32_t& errorCode) : errorCode_(errorCode) {}
    inline KeyStoreServiceReturnCode& operator=(const ErrorCode& errorCode) {
        errorCode_ = int32_t(errorCode);
        return *this;
    }
    inline KeyStoreServiceReturnCode& operator=(const ResponseCode& errorCode) {
        errorCode_ = int32_t(errorCode);
        return *this;
    }
    inline KeyStoreServiceReturnCode& operator=(const KeyStoreServiceReturnCode& errorCode) {
        errorCode_ = errorCode.errorCode_;
        return *this;
    }
    inline bool isOk() const {
        return errorCode_ == static_cast<int32_t>(ResponseCode::NO_ERROR) ||
               errorCode_ == static_cast<int32_t>(ErrorCode::OK);
    }

    inline int32_t getErrorCode() const {
        if (!errorCode_) return static_cast<int32_t>(ResponseCode::NO_ERROR /* 1 */);
        return errorCode_;
    }
    inline bool operator==(const ResponseCode& rhs) const {
        return (rhs == ResponseCode::NO_ERROR &&
                errorCode_ == static_cast<int32_t>(ErrorCode::OK)) ||
               errorCode_ == int32_t(rhs);
    }
    inline bool operator==(const ErrorCode& rhs) const {
        return (rhs == ErrorCode::OK &&
                errorCode_ == static_cast<int32_t>(ResponseCode::NO_ERROR)) ||
               errorCode_ == int32_t(rhs);
    }
    inline bool operator!=(const ResponseCode& rhs) const { return !(*this == rhs); }
    inline bool operator!=(const ErrorCode& rhs) const { return !(*this == rhs); }

  private:
    int32_t errorCode_;
};

inline bool operator==(const ResponseCode& lhs, const KeyStoreServiceReturnCode& rhs) {
    return rhs == lhs;
}
inline bool operator==(const ErrorCode& lhs, const KeyStoreServiceReturnCode& rhs) {
    return rhs == lhs;
}
inline bool operator!=(const ResponseCode& lhs, const KeyStoreServiceReturnCode& rhs) {
    return rhs != lhs;
}
inline bool operator!=(const ErrorCode& lhs, const KeyStoreServiceReturnCode& rhs) {
    return rhs != lhs;
}

inline std::ostream& operator<<(std::ostream& out, const KeyStoreServiceReturnCode& error) {
    return out << error.getErrorCode();
}

/**
 * The keystore native return code is a bit tricky. It can return error codes from two name spaces:
 * ErrorCode, which has negative error codes and use 0 for ERROR_OK;
 * ResponseCode, which has positive error codes and uses 1 for NO_ERROR.
 * This class can be initialized by both. And when accessed through the operator int32_t () it
 * always returns ErrorCode::OK (0) on success, even if it was initialized with
 * ResponseCode::NO_ERROR (1), because this is what (native) clients expect.
 *
 * !!! Do not this confuse with KeyStoreServiceReturnCode which always converts to 1 on success. !!!
 */
class KeyStoreNativeReturnCode {
  public:
    KeyStoreNativeReturnCode() : errorCode_(0) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreNativeReturnCode(const ErrorCode& errorCode) : errorCode_(int32_t(errorCode)) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreNativeReturnCode(const ResponseCode& errorCode) : errorCode_(int32_t(errorCode)) {}
    KeyStoreNativeReturnCode(const KeyStoreNativeReturnCode& errorCode)
        : errorCode_(errorCode.errorCode_) {}
    explicit inline KeyStoreNativeReturnCode(const int32_t& errorCode) : errorCode_(errorCode) {}
    // NOLINTNEXTLINE(google-explicit-constructor)
    KeyStoreNativeReturnCode(const KeyStoreServiceReturnCode& errorcode);
    inline KeyStoreNativeReturnCode& operator=(const ErrorCode& errorCode) {
        errorCode_ = int32_t(errorCode);
        return *this;
    }
    inline KeyStoreNativeReturnCode& operator=(const ResponseCode& errorCode) {
        errorCode_ = int32_t(errorCode);
        return *this;
    }
    inline KeyStoreNativeReturnCode& operator=(const KeyStoreNativeReturnCode& errorCode) {
        errorCode_ = errorCode.errorCode_;
        return *this;
    }
    inline bool isOk() const {
        return errorCode_ == static_cast<int32_t>(ResponseCode::NO_ERROR) ||
               errorCode_ == static_cast<int32_t>(ErrorCode::OK);
    }
    inline int32_t getErrorCode() const {
        if (errorCode_ == static_cast<int32_t>(ResponseCode::NO_ERROR) /* 1 */) {
            return static_cast<int32_t>(ErrorCode::OK) /* 0 */;
        }
        return errorCode_;
    }
    inline bool operator==(const ResponseCode& rhs) const {
        return (rhs == ResponseCode::NO_ERROR &&
                errorCode_ == static_cast<int32_t>(ErrorCode::OK)) ||
               errorCode_ == int32_t(rhs);
    }
    inline bool operator==(const ErrorCode& rhs) const {
        return (rhs == ErrorCode::OK &&
                errorCode_ == static_cast<int32_t>(ResponseCode::NO_ERROR)) ||
               errorCode_ == int32_t(rhs);
    }
    inline bool operator!=(const ResponseCode& rhs) const { return !(*this == rhs); }
    inline bool operator!=(const ErrorCode& rhs) const { return !(*this == rhs); }

  private:
    int32_t errorCode_;
};

inline bool operator==(const ResponseCode& lhs, const KeyStoreNativeReturnCode& rhs) {
    return rhs == lhs;
}
inline bool operator==(const ErrorCode& lhs, const KeyStoreNativeReturnCode& rhs) {
    return rhs == lhs;
}
inline bool operator!=(const ResponseCode& lhs, const KeyStoreNativeReturnCode& rhs) {
    return rhs != lhs;
}
inline bool operator!=(const ErrorCode& lhs, const KeyStoreNativeReturnCode& rhs) {
    return rhs != lhs;
}

inline KeyStoreNativeReturnCode::KeyStoreNativeReturnCode(
    const KeyStoreServiceReturnCode& errorCode)
    : errorCode_(errorCode.getErrorCode()) {}
inline KeyStoreServiceReturnCode::KeyStoreServiceReturnCode(
    const KeyStoreNativeReturnCode& errorCode)
    : errorCode_(errorCode.getErrorCode()) {}

inline std::ostream& operator<<(std::ostream& out, const KeyStoreNativeReturnCode& error) {
    return out << error.getErrorCode();
}

}  // namespace keystore

#endif  // KEYSTORE_INCLUDE_KEYSTORE_KEYSTORE_RETURN_TYPES_H_
