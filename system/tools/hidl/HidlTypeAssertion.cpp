/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "HidlTypeAssertion.h"

#include <hidl-util/Formatter.h>

#include <string>
#include <vector>

namespace android {

typedef std::vector<std::pair<std::string, size_t>> Registry;
static Registry &registry() {
    static Registry sRegistry;
    return sRegistry;
}

HidlTypeAssertion::HidlTypeAssertion(const char *name, size_t size)
    : mSize(size) {
    registry().push_back(std::make_pair(name, size));
}

size_t HidlTypeAssertion::size() const {
    return mSize;
}

void HidlTypeAssertion::EmitAll(Formatter &out) {
    std::sort(
            registry().begin(),
            registry().end(),
            [](const auto &a, const auto &b) {
                return a.first < b.first;
            });

    for (const auto& entry : registry()) {
        out << "static_assert(sizeof(::android::hardware::"
            << entry.first
            << ") == "
            << entry.second
            << ", \"wrong size\");\n";
    }
}

}  // namespace android

