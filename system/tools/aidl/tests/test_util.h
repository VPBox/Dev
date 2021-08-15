/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AIDL_TESTS_TEST_UTIL_H_
#define AIDL_TESTS_TEST_UTIL_H_

#include <string>

namespace android {
namespace aidl {
namespace test {

std::string CanonicalNameToPath(const char* package_class,
                                const char* extension);

void SplitPackageClass(const std::string& package_class,
                       std::string* rel_path,
                       std::string* package,
                       std::string* class_name);

void PrintDiff(const std::string& a, const std::string& b);

}  // namespace test
}  // namespace android
}  // namespace aidl

#endif // AIDL_TESTS_TEST_UTIL_H_
