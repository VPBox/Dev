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

#include "PathUtils.h"

#include <cutils/log.h>
#include <libgen.h>
#include <unistd.h>

namespace minikin {

const char* SELF_EXE_PATH = "/proc/self/exe";

std::string getDirname(const std::string& path) {
    const char* result = dirname(path.c_str());
    LOG_ALWAYS_FATAL_IF(result == nullptr, "dirname failed.");
    return std::string(result);
}

std::string getBasename(const std::string& path) {
    const char* result = basename(path.c_str());
    LOG_ALWAYS_FATAL_IF(result == nullptr, "basename failed.");
    return std::string(result);
}

std::string getTestDataDir() {
    char buf[PATH_MAX] = {};
    LOG_ALWAYS_FATAL_IF(readlink(SELF_EXE_PATH, buf, PATH_MAX) == -1, "readlink failed.");
    return getDirname(buf) + "/data/";
}

}  // namespace minikin
