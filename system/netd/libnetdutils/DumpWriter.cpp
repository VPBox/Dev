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

#include "netdutils/DumpWriter.h"

#include <unistd.h>
#include <limits>

#include <android-base/stringprintf.h>
#include <utils/String8.h>

using android::base::StringAppendV;

namespace android {
namespace netdutils {

namespace {

const char kIndentString[] = "  ";
const size_t kIndentStringLen = strlen(kIndentString);

}  // namespace

DumpWriter::DumpWriter(int fd) : mIndentLevel(0), mFd(fd) {}

void DumpWriter::incIndent() {
    if (mIndentLevel < std::numeric_limits<decltype(mIndentLevel)>::max()) {
        mIndentLevel++;
    }
}

void DumpWriter::decIndent() {
    if (mIndentLevel > std::numeric_limits<decltype(mIndentLevel)>::min()) {
        mIndentLevel--;
    }
}

void DumpWriter::println(const std::string& line) {
    if (!line.empty()) {
        for (int i = 0; i < mIndentLevel; i++) {
            ::write(mFd, kIndentString, kIndentStringLen);
        }
        ::write(mFd, line.c_str(), line.size());
    }
    ::write(mFd, "\n", 1);
}

// NOLINTNEXTLINE(cert-dcl50-cpp): Grandfathered C-style variadic function.
void DumpWriter::println(const char* fmt, ...) {
    std::string line;
    va_list ap;
    va_start(ap, fmt);
    StringAppendV(&line, fmt, ap);
    va_end(ap);
    println(line);
}

}  // namespace netdutils
}  // namespace android
