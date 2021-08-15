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

#include "MatrixInstance.h"

#include <utility>

#include "Regex.h"

namespace android {
namespace vintf {

MatrixInstance::MatrixInstance() = default;

MatrixInstance::MatrixInstance(const MatrixInstance&) = default;

MatrixInstance::MatrixInstance(MatrixInstance&&) noexcept = default;

MatrixInstance& MatrixInstance::operator=(const MatrixInstance&) = default;

MatrixInstance& MatrixInstance::operator=(MatrixInstance&&) noexcept = default;

MatrixInstance::MatrixInstance(FqInstance&& fqInstance, VersionRange&& range, bool optional,
                               bool isRegex)
    : mFqInstance(std::move(fqInstance)),
      mRange(std::move(range)),
      mOptional(optional),
      mIsRegex(isRegex) {}

MatrixInstance::MatrixInstance(const FqInstance fqInstance, const VersionRange& range,
                               bool optional, bool isRegex)
    : mFqInstance(fqInstance), mRange(range), mOptional(optional), mIsRegex(isRegex) {}

const std::string& MatrixInstance::package() const {
    return mFqInstance.getPackage();
}

const VersionRange& MatrixInstance::versionRange() const {
    return mRange;
}

const std::string& MatrixInstance::interface() const {
    return mFqInstance.getInterface();
}

bool MatrixInstance::optional() const {
    return mOptional;
}

bool MatrixInstance::isSatisfiedBy(const FqInstance& provided) const {
    return package() == provided.getPackage() &&
           versionRange().supportedBy(provided.getVersion()) &&
           interface() == provided.getInterface() && matchInstance(provided.getInstance());
}

bool MatrixInstance::matchInstance(const std::string& e) const {
    if (!isRegex()) {
        return exactInstance() == e;
    }
    details::Regex regex;
    if (!regex.compile(regexPattern())) {
        return false;
    }
    return regex.matches(e);
}

const std::string& MatrixInstance::regexPattern() const {
    static const std::string kEmptyString;
    return isRegex() ? mFqInstance.getInstance() : kEmptyString;
}

const std::string& MatrixInstance::exactInstance() const {
    static const std::string kEmptyString;
    return isRegex() ? kEmptyString : mFqInstance.getInstance();
}

bool MatrixInstance::isRegex() const {
    return mIsRegex;
}

}  // namespace vintf
}  // namespace android
