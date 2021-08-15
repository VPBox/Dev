
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

#include <vintf/FileSystem.h>

#include <dirent.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace android {
namespace vintf {
namespace details {

status_t FileSystemImpl::fetch(const std::string& path, std::string* fetched,
                               std::string* error) const {
    std::ifstream in;

    errno = 0;
    in.open(path);
    if (!in || errno != 0) {
        if (error) {
            *error = "Cannot open " + path + ": " + strerror(errno);
        }
        return -errno;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    *fetched = ss.str();

    return -errno;
}

status_t FileSystemImpl::listFiles(const std::string& path, std::vector<std::string>* out,
                                   std::string* error) const {
    errno = 0;
    DIR* dirp = opendir(path.c_str());
    if (!dirp || errno != 0) {
        if (error) {
            *error = "Cannot open " + path + ": " + strerror(errno);
        }
        return -errno;
    }

    std::unique_ptr<DIR, decltype(&closedir)> dir(dirp, closedir);
    dirent* dp;
    while ((dp = readdir(dir.get())) != nullptr) {
        if (dp->d_type != DT_DIR) {
            out->push_back(dp->d_name);
        }
    }
    return -errno;
}

status_t FileSystemNoOp::fetch(const std::string&, std::string*, std::string*) const {
    return NAME_NOT_FOUND;
}

status_t FileSystemNoOp::listFiles(const std::string&, std::vector<std::string>*,
                                   std::string*) const {
    return NAME_NOT_FOUND;
}

FileSystemUnderPath::FileSystemUnderPath(const std::string& rootdir) {
    mRootDir = rootdir;
    if (!mRootDir.empty() && mRootDir.back() != '/') {
        mRootDir.push_back('/');
    }
}

status_t FileSystemUnderPath::fetch(const std::string& path, std::string* fetched,
                                    std::string* error) const {
    return mImpl.fetch(mRootDir + path, fetched, error);
}

status_t FileSystemUnderPath::listFiles(const std::string& path, std::vector<std::string>* out,
                                        std::string* error) const {
    return mImpl.listFiles(mRootDir + path, out, error);
}

const std::string& FileSystemUnderPath::getRootDir() const {
    return mRootDir;
}

}  // namespace details
}  // namespace vintf
}  // namespace android
