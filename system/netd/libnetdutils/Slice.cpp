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

#include <sstream>

#include "netdutils/Slice.h"

namespace android {
namespace netdutils {
namespace {

// Convert one byte to a two character hexadecimal string
const std::string toHex(uint8_t byte) {
    const std::array<char, 16> kLookup = {
        {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'}};
    return {kLookup[byte >> 4], kLookup[byte & 0xf]};
}

}  // namespace

std::string toString(const Slice s) {
    return std::string(reinterpret_cast<char*>(s.base()), s.size());
}

std::string toHex(const Slice s, int wrap) {
    Slice tail = s;
    int count = 0;
    std::stringstream ss;
    while (!tail.empty()) {
        uint8_t byte = 0;
        extract(tail, byte);
        ss << toHex(byte);
        if ((++count % wrap) == 0) {
            ss << "\n";
        }
        tail = drop(tail, 1);
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Slice& slice) {
    return os << std::hex << "Slice[base: " << reinterpret_cast<void*>(slice.base())
              << ", limit: " << reinterpret_cast<void*>(slice.limit()) << ", size: 0x"
              << slice.size() << "]" << std::dec;
}

}  // namespace netdutils
}  // namespace android
