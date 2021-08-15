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

#include "Regex.h"

namespace android {
namespace vintf {
namespace details {

Regex::~Regex() {
    clear();
}

void Regex::clear() {
    if (mImpl != nullptr) {
        regfree(mImpl.get());
        mImpl = nullptr;
    }
}

bool Regex::compile(const std::string& pattern) {
    clear();
    mImpl = std::make_unique<regex_t>();
    int status = regcomp(mImpl.get(), pattern.c_str(), REG_EXTENDED | REG_NEWLINE);
    return status == 0;
}

bool Regex::matches(const std::string& s) const {
    regmatch_t match;
    int status =
        regexec(mImpl.get(), s.c_str(), 1 /* nmatch */, &match /* pmatch */, 0 /* flags */);
    return status == 0 && match.rm_so == 0 && match.rm_eo >= 0 &&
           static_cast<size_t>(match.rm_eo) == s.length();
}

}  // namespace details
}  // namespace vintf
}  // namespace android
