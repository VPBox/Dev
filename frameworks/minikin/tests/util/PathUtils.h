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

#ifndef TEST_UTILS_PATH_UTILS_H
#define TEST_UTILS_PATH_UTILS_H

#include <string>

namespace minikin {

std::string getDirname(const std::string& path);
std::string getBasename(const std::string& path);

// Returns test data directory.
std::string getTestDataDir();

inline std::string getTestFontPath(const std::string& fontFilePath) {
    return getTestDataDir() + fontFilePath;
}

}  // namespace minikin

#endif  // TEST_UTILS_PATH_UTILS_H
