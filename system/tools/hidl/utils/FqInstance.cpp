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

#include "FqInstance.h"

#include <sstream>

namespace android {

static const char INSTANCE_SEP = '/';

const std::string& FqInstance::getPackage() const {
    return mFqName.package();
}

bool FqInstance::hasPackage() const {
    return !getPackage().empty();
}

size_t FqInstance::getMajorVersion() const {
    return hasVersion() ? mFqName.getPackageMajorVersion() : 0;
}

size_t FqInstance::getMinorVersion() const {
    return hasVersion() ? mFqName.getPackageMinorVersion() : 0;
}

std::pair<size_t, size_t> FqInstance::getVersion() const {
    return mFqName.getVersion();
}

bool FqInstance::hasVersion() const {
    return mFqName.hasVersion();
}

const std::string& FqInstance::getInterface() const {
    return mFqName.getInterfaceName();
}

bool FqInstance::hasInterface() const {
    return mFqName.isInterfaceName();
}

const std::string& FqInstance::getInstance() const {
    return mInstance;
}

bool FqInstance::hasInstance() const {
    return !mInstance.empty();
}

bool FqInstance::isValid() const {
    bool hasPkg = hasPackage();
    bool hasVer = hasVersion();
    bool hasIntf = hasInterface();
    bool hasInst = hasInstance();

    // android.hardware.foo@1.0::IFoo/default
    if (hasPkg && hasVer && hasIntf && hasInst) {
        return true;
    }

    // @1.0::IFoo/default
    if (!hasPkg && hasVer && hasIntf && hasInst) {
        return true;
    }

    // IFoo/default
    if (!hasPkg && !hasVer && hasIntf && hasInst) {
        return true;
    }

    // Other cases are covered by FQName::setTo, but instance name should be empty.
    return !hasInst;
}

bool FqInstance::setTo(const std::string& s) {
    auto pos = s.find(INSTANCE_SEP);
    if (!mFqName.setTo(s.substr(0, pos))) return false;
    mInstance = pos == std::string::npos ? std::string{} : s.substr(pos + 1);

    return isValid();
}

bool FqInstance::setTo(const std::string& package, size_t majorVer, size_t minorVer,
                       const std::string& interface, const std::string& instance) {
    if (!mFqName.setTo(package, majorVer, minorVer, interface)) return false;
    mInstance = instance;
    return isValid();
}

bool FqInstance::setTo(size_t majorVer, size_t minorVer, const std::string& interface,
                       const std::string& instance) {
    return setTo("", majorVer, minorVer, interface, instance);
}

bool FqInstance::setTo(const std::string& interface, const std::string& instance) {
    return setTo(0u, 0u, interface, instance);
}

std::string FqInstance::string() const {
    std::string ret = mFqName.string();
    if (hasInstance()) ret += INSTANCE_SEP + mInstance;
    return ret;
}

bool FqInstance::operator<(const FqInstance& other) const {
    return string() < other.string();
}

bool FqInstance::operator==(const FqInstance& other) const {
    return string() == other.string();
}

bool FqInstance::operator!=(const FqInstance& other) const {
    return !(*this == other);
}

bool FqInstance::inPackage(const std::string& package) const {
    return mFqName.inPackage(package);
}

}  // namespace android
