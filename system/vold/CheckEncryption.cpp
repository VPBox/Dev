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

#include "CheckEncryption.h"
#include "FileDeviceUtils.h"
#include "Utils.h"
#include "VolumeManager.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/unique_fd.h>
#include <cutils/iosched_policy.h>
#include <private/android_filesystem_config.h>

#include <sstream>

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include <assert.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

using android::base::unique_fd;

using android::base::ReadFileToString;
using android::base::WriteStringToFile;

namespace android {
namespace vold {

constexpr uint32_t max_extents = 32;
constexpr size_t bytecount = 8;
constexpr int repeats = 256;

bool check_file(const std::string& needle) {
    LOG(DEBUG) << "checkEncryption check_file: " << needle;
    auto haystack = android::vold::BlockDeviceForPath(needle);
    if (haystack.empty()) {
        LOG(ERROR) << "Failed to find device for path: " << needle;
        return false;
    }

    std::string randombytes;
    if (ReadRandomBytes(bytecount, randombytes) != 0) {
        LOG(ERROR) << "Failed to read random bytes";
        return false;
    }
    std::string randomhex;
    StrToHex(randombytes, randomhex);
    std::ostringstream os;
    for (int i = 0; i < repeats; i++) os << randomhex;
    auto towrite = os.str();

    if (access(needle.c_str(), F_OK) == 0) {
        if (unlink(needle.c_str()) != 0) {
            PLOG(ERROR) << "Failed to unlink " << needle;
            return false;
        }
    }
    LOG(DEBUG) << "Writing to " << needle;
    if (!WriteStringToFile(towrite, needle)) {
        PLOG(ERROR) << "Failed to write " << needle;
        return false;
    }
    sync();

    unique_fd haystack_fd(open(haystack.c_str(), O_RDONLY | O_CLOEXEC));
    if (haystack_fd.get() == -1) {
        PLOG(ERROR) << "Failed to open " << haystack;
        return false;
    }

    auto fiemap = PathFiemap(needle, max_extents);

    std::string area;
    for (uint32_t i = 0; i < fiemap->fm_mapped_extents; i++) {
        auto xt = &(fiemap->fm_extents[i]);
        LOG(DEBUG) << "Extent " << i << " at " << xt->fe_physical << " length " << xt->fe_length;
        if (lseek64(haystack_fd.get(), xt->fe_physical, SEEK_SET) == -1) {
            PLOG(ERROR) << "Failed lseek";
            return false;
        }
        auto toread = xt->fe_length;
        while (toread > 0) {
            char buf[BUFSIZ];
            size_t wlen =
                static_cast<size_t>(std::min(static_cast<typeof(toread)>(sizeof(buf)), toread));
            auto l = read(haystack_fd.get(), buf, wlen);
            if (l < 1) {
                PLOG(ERROR) << "Failed read";
                if (errno != EINTR) {
                    return false;
                }
            }
            area.append(buf, l);
            toread -= l;
        }
    }

    LOG(DEBUG) << "Searching " << area.size() << " bytes of " << needle;
    LOG(DEBUG) << "First position of blob: " << area.find(randomhex);
    return true;
}

int CheckEncryption(const std::string& path) {
    auto deNeedle(path);
    deNeedle += "/misc";
    if (android::vold::PrepareDir(deNeedle, 01771, AID_SYSTEM, AID_MISC)) {
        return -1;
    }
    deNeedle += "/vold";
    if (android::vold::PrepareDir(deNeedle, 0700, AID_ROOT, AID_ROOT)) {
        return -1;
    }
    deNeedle += "/checkEncryption";

    auto neNeedle(path);
    neNeedle += "/unencrypted/checkEncryption";

    check_file(deNeedle);
    check_file(neNeedle);

    return 0;
}

}  // namespace vold
}  // namespace android
