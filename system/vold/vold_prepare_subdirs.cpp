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

/*
 * Tool to create a directory with the right SELinux context applied, or
 * apply the context if it's absent. Also fixes mode, uid, gid.
 */

#include <iostream>
#include <string>
#include <vector>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/logging.h>
#include <android-base/scopeguard.h>

#include <cutils/fs.h>
#include <selinux/android.h>

#include "Utils.h"
#include "android/os/IVold.h"

#include <private/android_filesystem_config.h>

static void usage(const char* progname) {
    std::cerr << "Usage: " << progname << " [ prepare | destroy ] <volume_uuid> <user_id> <flags>"
              << std::endl;
    exit(-1);
}

static bool small_int(const std::string& s) {
    return !s.empty() && s.size() < 7 && s.find_first_not_of("0123456789") == std::string::npos;
}

static bool valid_uuid(const std::string& s) {
    return s.size() < 40 && s.find_first_not_of("0123456789abcdefABCDEF-_") == std::string::npos;
}

static bool prepare_dir(struct selabel_handle* sehandle, mode_t mode, uid_t uid, gid_t gid,
                        const std::string& path) {
    auto clearfscreatecon = android::base::make_scope_guard([] { setfscreatecon(nullptr); });
    auto secontext = std::unique_ptr<char, void (*)(char*)>(nullptr, freecon);
    char* tmp_secontext;
    if (sehandle && selabel_lookup(sehandle, &tmp_secontext, path.c_str(), S_IFDIR) == 0) {
        secontext.reset(tmp_secontext);
    }
    LOG(DEBUG) << "Setting up mode " << std::oct << mode << std::dec << " uid " << uid << " gid "
               << gid << " context " << (secontext ? secontext.get() : "null")
               << " on path: " << path;
    if (secontext) {
        if (setfscreatecon(secontext.get()) != 0) {
            PLOG(ERROR) << "Unable to read setfscreatecon for: " << path;
            return false;
        }
    }
    if (fs_prepare_dir(path.c_str(), mode, uid, gid) != 0) {
        return false;
    }
    if (secontext) {
        char* tmp_oldsecontext = nullptr;
        if (lgetfilecon(path.c_str(), &tmp_oldsecontext) < 0) {
            PLOG(ERROR) << "Unable to read secontext for: " << path;
            return false;
        }
        auto oldsecontext = std::unique_ptr<char, void (*)(char*)>(tmp_oldsecontext, freecon);
        if (strcmp(secontext.get(), oldsecontext.get()) != 0) {
            LOG(INFO) << "Relabelling from " << ((char*)oldsecontext.get()) << " to "
                      << ((char*)secontext.get()) << ": " << path;
            if (lsetfilecon(path.c_str(), secontext.get()) != 0) {
                PLOG(ERROR) << "Relabelling failed for: " << path;
                return false;
            }
        }
    }
    return true;
}

static bool rmrf_contents(const std::string& path) {
    auto dirp = std::unique_ptr<DIR, int (*)(DIR*)>(opendir(path.c_str()), closedir);
    if (!dirp) {
        PLOG(ERROR) << "Unable to open directory: " << path;
        return false;
    }
    bool res = true;
    for (;;) {
        errno = 0;
        auto const entry = readdir(dirp.get());
        if (!entry) {
            if (errno) {
                PLOG(ERROR) << "readdir failed on: " << path;
                return false;
            }
            return res;
        }
        if (entry->d_name[0] == '.') continue;
        auto subdir = path + "/" + entry->d_name;
        if (0 !=
            android::vold::ForkExecvp(std::vector<std::string>{"/system/bin/rm", "-rf", subdir})) {
            LOG(ERROR) << "rm -rf failed on " << subdir;
            res = false;
        }
    }
}

static bool prepare_subdirs(const std::string& volume_uuid, int user_id, int flags) {
    struct selabel_handle* sehandle = selinux_android_file_context_handle();

    if (volume_uuid.empty()) {
        if (flags & android::os::IVold::STORAGE_FLAG_DE) {
            auto misc_de_path = android::vold::BuildDataMiscDePath(user_id);
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_de_path + "/vold")) return false;
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_de_path + "/storaged")) return false;
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_de_path + "/rollback")) return false;

            auto vendor_de_path = android::vold::BuildDataVendorDePath(user_id);
            if (!prepare_dir(sehandle, 0700, AID_SYSTEM, AID_SYSTEM, vendor_de_path + "/fpdata")) {
                return false;
            }
            auto facedata_path = vendor_de_path + "/facedata";
            if (!prepare_dir(sehandle, 0700, AID_SYSTEM, AID_SYSTEM, facedata_path)) {
                return false;
            }
        }
        if (flags & android::os::IVold::STORAGE_FLAG_CE) {
            auto misc_ce_path = android::vold::BuildDataMiscCePath(user_id);
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_ce_path + "/vold")) return false;
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_ce_path + "/storaged")) return false;
            if (!prepare_dir(sehandle, 0700, 0, 0, misc_ce_path + "/rollback")) return false;

            auto system_ce_path = android::vold::BuildDataSystemCePath(user_id);
            if (!prepare_dir(sehandle, 0700, AID_SYSTEM, AID_SYSTEM, system_ce_path + "/backup")) {
                return false;
            }
            if (!prepare_dir(sehandle, 0700, AID_SYSTEM, AID_SYSTEM,
                             system_ce_path + "/backup_stage")) {
                return false;
            }
            auto vendor_ce_path = android::vold::BuildDataVendorCePath(user_id);
            auto facedata_path = vendor_ce_path + "/facedata";
            if (!prepare_dir(sehandle, 0700, AID_SYSTEM, AID_SYSTEM, facedata_path)) {
                return false;
            }
        }
    }
    return true;
}

static bool destroy_subdirs(const std::string& volume_uuid, int user_id, int flags) {
    bool res = true;
    if (volume_uuid.empty()) {
        if (flags & android::os::IVold::STORAGE_FLAG_CE) {
            auto misc_ce_path = android::vold::BuildDataMiscCePath(user_id);
            res &= rmrf_contents(misc_ce_path);

            auto vendor_ce_path = android::vold::BuildDataVendorCePath(user_id);
            res &= rmrf_contents(vendor_ce_path);
        }
        if (flags & android::os::IVold::STORAGE_FLAG_DE) {
            auto misc_de_path = android::vold::BuildDataMiscDePath(user_id);
            res &= rmrf_contents(misc_de_path);

            auto vendor_de_path = android::vold::BuildDataVendorDePath(user_id);
            res &= rmrf_contents(vendor_de_path);
        }
    }
    return res;
}

int main(int argc, const char* const argv[]) {
    android::base::InitLogging(const_cast<char**>(argv));
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.size() != 4 || !valid_uuid(args[1]) || !small_int(args[2]) || !small_int(args[3])) {
        usage(argv[0]);
        return -1;
    }

    auto volume_uuid = args[1];
    int user_id = stoi(args[2]);
    int flags = stoi(args[3]);
    if (args[0] == "prepare") {
        if (!prepare_subdirs(volume_uuid, user_id, flags)) return -1;
    } else if (args[0] == "destroy") {
        if (!destroy_subdirs(volume_uuid, user_id, flags)) return -1;
    } else {
        usage(argv[0]);
        return -1;
    }
    return 0;
}
