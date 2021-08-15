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

#include "FQName.h"

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <iostream>
#include <regex>
#include <sstream>

#define RE_COMPONENT    "[a-zA-Z_][a-zA-Z_0-9]*"
#define RE_PATH         RE_COMPONENT "(?:[.]" RE_COMPONENT ")*"
#define RE_MAJOR        "[0-9]+"
#define RE_MINOR        "[0-9]+"

namespace android {

FQName::FQName() : mIsIdentifier(false) {}

bool FQName::parse(const std::string& s, FQName* into) {
    return into->setTo(s);
}

FQName::FQName(const std::string& package, const std::string& version, const std::string& name,
               const std::string& valueName) {
    size_t majorVer, minorVer;
    CHECK(parseVersion(version, &majorVer, &minorVer));
    CHECK(setTo(package, majorVer, minorVer, name, valueName)) << string();
}

bool FQName::setTo(const std::string& package, size_t majorVer, size_t minorVer,
                   const std::string& name, const std::string& valueName) {
    mPackage = package;
    mMajor = majorVer;
    mMinor = minorVer;
    mName = name;
    mValueName = valueName;

    FQName other;
    if (!parse(string(), &other)) return false;
    if ((*this) != other) return false;
    mIsIdentifier = other.isIdentifier();
    return true;
}

FQName::FQName(const FQName& other)
    : mIsIdentifier(other.mIsIdentifier),
      mPackage(other.mPackage),
      mMajor(other.mMajor),
      mMinor(other.mMinor),
      mName(other.mName),
      mValueName(other.mValueName) {}

bool FQName::isIdentifier() const {
    return mIsIdentifier;
}

bool FQName::isFullyQualified() const {
    return !mPackage.empty() && !version().empty() && !mName.empty();
}

bool FQName::isValidValueName() const {
    return mIsIdentifier
        || (!mName.empty() && !mValueName.empty());
}

bool FQName::isInterfaceName() const {
    return !mName.empty() && mName[0] == 'I' && mName.find('.') == std::string::npos;
}

bool FQName::setTo(const std::string &s) {
    // android.hardware.foo@1.0::IFoo.Type
    static const std::regex kRE1("(" RE_PATH ")@(" RE_MAJOR ")[.](" RE_MINOR ")::(" RE_PATH ")");
    // @1.0::IFoo.Type
    static const std::regex kRE2("@(" RE_MAJOR ")[.](" RE_MINOR ")::(" RE_PATH ")");
    // android.hardware.foo@1.0 (for package declaration and whole package import)
    static const std::regex kRE3("(" RE_PATH ")@(" RE_MAJOR ")[.](" RE_MINOR ")");
    // IFoo.Type
    static const std::regex kRE4("(" RE_COMPONENT ")([.]" RE_COMPONENT ")+");
    // Type (a plain identifier)
    static const std::regex kRE5("(" RE_COMPONENT ")");

    // android.hardware.foo@1.0::IFoo.Type:MY_ENUM_VALUE
    static const std::regex kRE6("(" RE_PATH ")@(" RE_MAJOR ")[.](" RE_MINOR ")::(" RE_PATH
                                 "):(" RE_COMPONENT ")");
    // @1.0::IFoo.Type:MY_ENUM_VALUE
    static const std::regex kRE7("@(" RE_MAJOR ")[.](" RE_MINOR ")::(" RE_PATH "):(" RE_COMPONENT
                                 ")");
    // IFoo.Type:MY_ENUM_VALUE
    static const std::regex kRE8("(" RE_PATH "):(" RE_COMPONENT ")");

    bool invalid = false;
    clear();

    std::smatch match;
    if (std::regex_match(s, match, kRE1)) {
        CHECK_EQ(match.size(), 5u);

        mPackage = match.str(1);
        invalid |= !parseVersion(match.str(2), match.str(3));
        mName = match.str(4);
    } else if (std::regex_match(s, match, kRE2)) {
        CHECK_EQ(match.size(), 4u);

        invalid |= !parseVersion(match.str(1), match.str(2));
        mName = match.str(3);
    } else if (std::regex_match(s, match, kRE3)) {
        CHECK_EQ(match.size(), 4u);

        mPackage = match.str(1);
        invalid |= !parseVersion(match.str(2), match.str(3));
    } else if (std::regex_match(s, match, kRE4)) {
        mName = match.str(0);
    } else if (std::regex_match(s, match, kRE5)) {
        mIsIdentifier = true;
        mName = match.str(0);
    } else if (std::regex_match(s, match, kRE6)) {
        CHECK_EQ(match.size(), 6u);

        mPackage = match.str(1);
        invalid |= !parseVersion(match.str(2), match.str(3));
        mName = match.str(4);
        mValueName = match.str(5);
    } else if (std::regex_match(s, match, kRE7)) {
        CHECK_EQ(match.size(), 5u);

        invalid |= !parseVersion(match.str(1), match.str(2));
        mName = match.str(3);
        mValueName = match.str(4);
    } else if (std::regex_match(s, match, kRE8)) {
        CHECK_EQ(match.size(), 3u);

        mName = match.str(1);
        mValueName = match.str(2);
    } else {
        invalid = true;
    }

    // mValueName must go with mName.
    CHECK(mValueName.empty() || !mName.empty());

    // package without version is not allowed.
    CHECK(invalid || mPackage.empty() || !version().empty());

    return !invalid;
}

const std::string& FQName::package() const {
    return mPackage;
}

std::string FQName::version() const {
    if (!hasVersion()) {
        return "";
    }
    return std::to_string(mMajor) + "." + std::to_string(mMinor);
}

std::string FQName::sanitizedVersion() const {
    if (!hasVersion()) {
        return "";
    }
    return "V" + std::to_string(mMajor) + "_" + std::to_string(mMinor);
}

std::string FQName::atVersion() const {
    std::string v = version();
    return v.empty() ? "" : ("@" + v);
}

void FQName::clear() {
    mIsIdentifier = false;
    mPackage.clear();
    clearVersion();
    mName.clear();
    mValueName.clear();
}

void FQName::clearVersion(size_t* majorVer, size_t* minorVer) {
    *majorVer = *minorVer = 0;
}

bool FQName::parseVersion(const std::string& majorStr, const std::string& minorStr,
                          size_t* majorVer, size_t* minorVer) {
    bool versionParseSuccess = ::android::base::ParseUint(majorStr, majorVer) &&
                               ::android::base::ParseUint(minorStr, minorVer);
    if (!versionParseSuccess) {
        LOG(ERROR) << "numbers in " << majorStr << "." << minorStr << " are out of range.";
    }
    return versionParseSuccess;
}

bool FQName::parseVersion(const std::string& v, size_t* majorVer, size_t* minorVer) {
    static const std::regex kREVer("(" RE_MAJOR ")[.](" RE_MINOR ")");

    if (v.empty()) {
        clearVersion(majorVer, minorVer);
        return true;
    }

    std::smatch match;
    if (!std::regex_match(v, match, kREVer)) {
        return false;
    }
    CHECK_EQ(match.size(), 3u);

    return parseVersion(match.str(1), match.str(2), majorVer, minorVer);
}

bool FQName::setVersion(const std::string& v) {
    return parseVersion(v, &mMajor, &mMinor);
}

void FQName::clearVersion() {
    clearVersion(&mMajor, &mMinor);
}

bool FQName::parseVersion(const std::string& majorStr, const std::string& minorStr) {
    return parseVersion(majorStr, minorStr, &mMajor, &mMinor);
}

const std::string& FQName::name() const {
    return mName;
}

std::vector<std::string> FQName::names() const {
    std::vector<std::string> res {};
    std::istringstream ss(name());
    std::string s;
    while (std::getline(ss, s, '.')) {
        res.push_back(s);
    }
    return res;
}

const std::string& FQName::valueName() const {
    return mValueName;
}

FQName FQName::typeName() const {
    return FQName(mPackage, version(), mName);
}

void FQName::applyDefaults(
        const std::string &defaultPackage,
        const std::string &defaultVersion) {

    // package without version is not allowed.
    CHECK(mPackage.empty() || !version().empty());

    if (mPackage.empty()) {
        mPackage = defaultPackage;
    }

    if (version().empty()) {
        CHECK(setVersion(defaultVersion));
    }
}

std::string FQName::string() const {
    std::string out;
    out.append(mPackage);
    out.append(atVersion());
    if (!mName.empty()) {
        if (!mPackage.empty() || !version().empty()) {
            out.append("::");
        }
        out.append(mName);

        if (!mValueName.empty()) {
            out.append(":");
            out.append(mValueName);
        }
    }

    return out;
}

bool FQName::operator<(const FQName &other) const {
    return string() < other.string();
}

bool FQName::operator==(const FQName &other) const {
    return string() == other.string();
}

bool FQName::operator!=(const FQName &other) const {
    return !(*this == other);
}

const std::string& FQName::getInterfaceName() const {
    CHECK(isInterfaceName()) << mName;

    return mName;
}

std::string FQName::getInterfaceBaseName() const {
    // cut off the leading 'I'.
    return getInterfaceName().substr(1);
}

std::string FQName::getInterfaceAdapterName() const {
    return "A" + getInterfaceBaseName();
}

std::string FQName::getInterfaceHwName() const {
    return "IHw" + getInterfaceBaseName();
}

std::string FQName::getInterfaceProxyName() const {
    return "BpHw" + getInterfaceBaseName();
}

std::string FQName::getInterfaceStubName() const {
    return "BnHw" + getInterfaceBaseName();
}

std::string FQName::getInterfacePassthroughName() const {
    return "Bs" + getInterfaceBaseName();
}

FQName FQName::getInterfaceProxyFqName() const {
    return FQName(package(), version(), getInterfaceProxyName());
}

FQName FQName::getInterfaceAdapterFqName() const {
    return FQName(package(), version(), getInterfaceAdapterName());
}

FQName FQName::getInterfaceStubFqName() const {
    return FQName(package(), version(), getInterfaceStubName());
}

FQName FQName::getInterfacePassthroughFqName() const {
    return FQName(package(), version(), getInterfacePassthroughName());
}

FQName FQName::getTypesForPackage() const {
    return FQName(package(), version(), "types");
}

FQName FQName::getPackageAndVersion() const {
    return FQName(package(), version(), "");
}

FQName FQName::getTopLevelType() const {
    auto idx = mName.find('.');

    if (idx == std::string::npos) {
        return *this;
    }

    return FQName(mPackage, version(), mName.substr(0, idx));
}

std::string FQName::tokenName() const {
    std::vector<std::string> components;
    getPackageAndVersionComponents(&components, true /* cpp_compatible */);

    if (!mName.empty()) {
        std::vector<std::string> nameComponents = base::Split(mName, ".");

        components.insert(components.end(), nameComponents.begin(), nameComponents.end());
    }

    return base::Join(components, "_");
}

std::string FQName::cppNamespace() const {
    std::vector<std::string> components;
    getPackageAndVersionComponents(&components, true /* cpp_compatible */);

    std::string out = "::";
    out += base::Join(components, "::");

    return out;
}

std::string FQName::cppLocalName() const {
    std::vector<std::string> components = base::Split(mName, ".");

    return base::Join(components, "::")
            + (mValueName.empty() ? "" : ("::" + mValueName));
}

std::string FQName::cppName() const {
    std::string out = cppNamespace();

    std::vector<std::string> components = base::Split(name(), ".");
    out += "::";
    out += base::Join(components, "::");
    if (!mValueName.empty()) {
        out  += "::" + mValueName;
    }

    return out;
}

std::string FQName::javaPackage() const {
    std::vector<std::string> components;
    getPackageAndVersionComponents(&components, true /* cpp_compatible */);

    return base::Join(components, ".");
}

std::string FQName::javaName() const {
    return javaPackage() + "." + name()
            + (mValueName.empty() ? "" : ("." + mValueName));
}

void FQName::getPackageComponents(std::vector<std::string> *components) const {
    *components = base::Split(package(), ".");
}

void FQName::getPackageAndVersionComponents(
        std::vector<std::string> *components,
        bool cpp_compatible) const {
    getPackageComponents(components);

    if (!hasVersion()) {
        LOG(WARNING) << "FQName: getPackageAndVersionComponents expects version.";
        return;
    }

    if (!cpp_compatible) {
        components->push_back(std::to_string(getPackageMajorVersion()) +
                "." + std::to_string(getPackageMinorVersion()));
        return;
    }

    components->push_back(sanitizedVersion());
}

bool FQName::hasVersion() const {
    return mMajor > 0;
}

std::pair<size_t, size_t> FQName::getVersion() const {
    return {mMajor, mMinor};
}

FQName FQName::withVersion(size_t major, size_t minor) const {
    FQName ret(*this);
    ret.mMajor = major;
    ret.mMinor = minor;
    return ret;
}

size_t FQName::getPackageMajorVersion() const {
    CHECK(hasVersion()) << "FQName: No version exists at getPackageMajorVersion(). "
                        << "Did you check hasVersion()?";
    return mMajor;
}

size_t FQName::getPackageMinorVersion() const {
    CHECK(hasVersion()) << "FQName: No version exists at getPackageMinorVersion(). "
                        << "Did you check hasVersion()?";
    return mMinor;
}

bool FQName::endsWith(const FQName &other) const {
    std::string s1 = string();
    std::string s2 = other.string();

    size_t pos = s1.rfind(s2);
    if (pos == std::string::npos || pos + s2.size() != s1.size()) {
        return false;
    }

    // A match is only a match if it is preceded by a "boundary", i.e.
    // we perform a component-wise match from the end.
    // "az" is not a match for "android.hardware.foo@1.0::IFoo.bar.baz",
    // "baz", "bar.baz", "IFoo.bar.baz", "@1.0::IFoo.bar.baz" are.
    if (pos == 0) {
        // matches "android.hardware.foo@1.0::IFoo.bar.baz"
        return true;
    }

    if (s1[pos - 1] == '.') {
        // matches "baz" and "bar.baz"
        return true;
    }

    if (s1[pos - 1] == ':') {
        // matches "IFoo.bar.baz"
        return true;
    }

    if (s1[pos] == '@') {
        // matches "@1.0::IFoo.bar.baz"
        return true;
    }

    return false;
}

bool FQName::inPackage(const std::string &package) const {
    std::vector<std::string> components;
    getPackageComponents(&components);

    std::vector<std::string> inComponents = base::Split(package, ".");

    if (inComponents.size() > components.size()) {
        return false;
    }

    for (size_t i = 0; i < inComponents.size(); i++) {
        if (inComponents[i] != components[i]) {
            return false;
        }
    }

    return true;
}

FQName FQName::downRev() const {
    FQName ret(*this);
    CHECK(ret.mMinor > 0);
    ret.mMinor--;
    return ret;
}

const FQName gIBaseFqName = FQName("android.hidl.base", "1.0", "IBase");
const FQName gIManagerFqName = FQName("android.hidl.manager", "1.0", "IServiceManager");

}  // namespace android

