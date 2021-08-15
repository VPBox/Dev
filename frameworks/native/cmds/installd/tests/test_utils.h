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

#pragma once

#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <selinux/android.h>

uint8_t kBase64Map[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
     52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
    255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
      7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
     19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
     37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255
};

uint8_t* DecodeBase64(const char* src, size_t* dst_size) {
    CHECK(dst_size != nullptr);
    std::vector<uint8_t> tmp;
    uint32_t t = 0, y = 0;
    int g = 3;
    for (size_t i = 0; src[i] != '\0'; ++i) {
        uint8_t c = kBase64Map[src[i] & 0xFF];
        if (c == 255) continue;
        // the final = symbols are read and used to trim the remaining bytes
        if (c == 254) {
            c = 0;
            // prevent g < 0 which would potentially allow an overflow later
            if (--g < 0) {
                *dst_size = 0;
                return nullptr;
            }
        } else if (g != 3) {
            // we only allow = to be at the end
            *dst_size = 0;
            return nullptr;
        }
        t = (t << 6) | c;
        if (++y == 4) {
            tmp.push_back((t >> 16) & 255);
            if (g > 1) {
                tmp.push_back((t >> 8) & 255);
            }
            if (g > 2) {
                tmp.push_back(t & 255);
            }
            y = t = 0;
        }
    }
    if (y != 0) {
        *dst_size = 0;
        return nullptr;
    }
    std::unique_ptr<uint8_t[]> dst(new uint8_t[tmp.size()]);
    *dst_size = tmp.size();
    std::copy(tmp.begin(), tmp.end(), dst.get());
    return dst.release();
}

bool WriteBase64ToFile(const char* base64, const std::string& file,
        uid_t uid, gid_t gid, int mode, std::string* error_msg) {
    CHECK(base64 != nullptr);
    size_t length;
    std::unique_ptr<uint8_t[]> bytes(DecodeBase64(base64, &length));
    CHECK(bytes != nullptr);


    int fd = open(file.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    using android::base::StringPrintf;

    if (fd < 0) {
        *error_msg = StringPrintf("Could not open file %s: %s", file.c_str(), strerror(errno));
        return false;
    }

    size_t wrote = 0;
    while (wrote < length) {
        ssize_t cur = write(fd, bytes.get() + wrote, length - wrote);
        if (cur == -1) {
            *error_msg = StringPrintf("Could not write file %s: %s", file.c_str(), strerror(errno));
            return false;
        }
        wrote += cur;
    }

    if (::chown(file.c_str(), uid, gid) != 0) {
        *error_msg = StringPrintf("Could not chown file %s: %s", file.c_str(), strerror(errno));
        return false;
    }
    if (::chmod(file.c_str(), mode) != 0) {
        *error_msg = StringPrintf("Could not chmod file %s: %s", file.c_str(), strerror(errno));
        return false;
    }
    return true;
}

// TODO(calin): fix dexopt drop_capabilities and move to general utils (b/69678790).
bool DropCapabilities(uid_t uid, gid_t gid) {
    if (setgid(gid) != 0) {
        PLOG(ERROR) << "setgid failed: " <<  gid;
        return false;
    }
    if (setuid(uid) != 0) {
        PLOG(ERROR) << "setuid failed: " <<  uid;
        return false;
    }
    // drop capabilities
    struct __user_cap_header_struct capheader;
    struct __user_cap_data_struct capdata[2];
    memset(&capheader, 0, sizeof(capheader));
    memset(&capdata, 0, sizeof(capdata));
    capheader.version = _LINUX_CAPABILITY_VERSION_3;
    if (capset(&capheader, &capdata[0]) < 0) {
        PLOG(ERROR) << "capset failed";
        return false;
    }

    return true;
}
