/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_FQINSTANCE_H_

#define ANDROID_FQINSTANCE_H_

#include <string>
#include <utility>

#include <hidl-util/FQName.h>

namespace android {

// A wrapper around FQName to include instance name as well.
// FqInstance::setTo also recognizes all FQName formats, including enum names
// etc.
// Typical usage:
// FqInstance fqInstance;
// if (!fqInstance.setTo("...")) {
//    // error handling
// }
// LOG(WARNING) << fqInstance.string();
class FqInstance {
   public:
    const std::string& getPackage() const;
    size_t getMajorVersion() const;
    size_t getMinorVersion() const;
    std::pair<size_t, size_t> getVersion() const;
    const std::string& getInterface() const;
    const std::string& getInstance() const;

    bool hasPackage() const;
    bool hasVersion() const;
    bool hasInterface() const;
    bool hasInstance() const;

    // If this is android.hardware@1.0::IFoo
    // package = "and" -> false
    // package = "android" -> true
    // package = "android.hardware@1.0" -> false
    bool inPackage(const std::string& package) const;

    // Return true if valid:
    // android.hardware.foo@1.0::IFoo/instance
    // @1.0::IFoo/instance
    // IFoo/instance
    // android.hardware.foo@1.0::IFoo.Type
    // @1.0::IFoo.Type
    // android.hardware.foo@1.0
    // IFoo.Type
    // Type
    // android.hardware.foo@1.0::IFoo.Type:MY_ENUM_VALUE
    // @1.0::IFoo.Type:MY_ENUM_VALUE
    // IFoo.Type:MY_ENUM_VALUE
    //
    // If no "/instance", hasInstance() will return false afterwards.
    // TODO(b/73774955): deprecate this and use std::optional.
    __attribute__((warn_unused_result)) bool setTo(const std::string& s);

    // Convenience method for the following formats:
    // android.hardware.foo@1.0
    // android.hardware.foo@1.0::IFoo
    // android.hardware.foo@1.0::IFoo/default
    __attribute__((warn_unused_result)) bool setTo(const std::string& package, size_t majorVer,
                                                   size_t minorVer,
                                                   const std::string& interface = "",
                                                   const std::string& instance = "");
    // Convenience method for the following formats:
    // @1.0::IFoo
    // @1.0::IFoo/default
    __attribute__((warn_unused_result)) bool setTo(size_t majorVer, size_t minorVer,
                                                   const std::string& interface,
                                                   const std::string& instance = "");
    // Convenience method for the following formats:
    // IFoo/default
    __attribute__((warn_unused_result)) bool setTo(const std::string& interface,
                                                   const std::string& instance);

    // undefined behavior if:
    // - Default constructor is called without setTo();
    // - setTo is called but returned false.
    // Should only be called after setTo() returns true.
    std::string string() const;
    bool operator<(const FqInstance& other) const;
    bool operator==(const FqInstance& other) const;
    bool operator!=(const FqInstance& other) const;

   private:
    FQName mFqName;
    std::string mInstance;

    // helper to setTo() to determine that the FqInstance is actually valid.
    bool isValid() const;
};

}  // namespace android

#endif  // ANDROID_FQINSTANCE_H_
