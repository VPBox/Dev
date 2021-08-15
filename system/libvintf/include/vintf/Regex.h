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

#ifndef ANDROID_VINTF_REGEX_H_
#define ANDROID_VINTF_REGEX_H_

#include <regex.h>
#include <string>

namespace android {
namespace vintf {
namespace details {

// A wrapper class around regex.h. This is used instead of C++ <regex> library because
// C++ regex library throws exceptions when an invalid regular expression is compiled.
// Use Extended Regular Expression (ERE) syntax.
class Regex {
   public:
    Regex() = default;
    ~Regex();

    Regex& operator=(const Regex&) = delete;
    Regex(const Regex&) = delete;

    __attribute__((warn_unused_result)) bool compile(const std::string& pattern);

    bool matches(const std::string& s) const;

    /**
     * Return nullptr if not a valid regex pattern, else the Regex object.
     */
    static const Regex* Get(const std::string& pattern);

   private:
    std::unique_ptr<regex_t> mImpl;

    void clear();
};

}  // namespace details
}  // namespace vintf
}  // namespace android

#endif  // ANDROID_VINTF_REGEX_H_
