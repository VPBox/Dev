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

#ifndef STRING_HELPER_H_

#define STRING_HELPER_H_

#include <string>
#include <vector>

namespace android {

struct StringHelper {

    enum Case {
        // return original string
        kNoCase = 0,
        // use camelCase
        kCamelCase = 1,
        // use PascalCase
        kPascalCase = 2,
        // use UP_SNAKE_CASE
        kUpperSnakeCase = 3,
        // use low_snake_case
        kLowerSnakeCase = 4
    };

    // methods for a single word, like device
    // UPPERCASE
    static std::string Uppercase(const std::string &in);
    // lowercase
    static std::string Lowercase(const std::string &in);
    // Capitalize
    static std::string Capitalize(const std::string &in);

    // methods for a multi-word identifier, like framebuffer_device
    static std::string ToCamelCase(const std::string &in);
    static std::string ToPascalCase(const std::string &in);
    static std::string ToUpperSnakeCase(const std::string &in);
    static std::string ToLowerSnakeCase(const std::string &in);
    static std::string ToCase(Case c, const std::string &in);

    static bool EndsWith(const std::string &in, const std::string &suffix);
    static bool StartsWith(const std::string &in, const std::string &prefix);

    /* removes suffix once from in if in ends with suffix */
    static std::string RTrim(const std::string &in, const std::string &suffix);

    /* removes prefix once from in if in starts with prefix */
    static std::string LTrim(const std::string &in, const std::string &prefix);

    /* removes suffix repeatedly from in if in ends with suffix */
    static std::string RTrimAll(const std::string &in, const std::string &suffix);

    /* removes prefix repeatedly from in if in starts with prefix */
    static std::string LTrimAll(const std::string &in, const std::string &prefix);


    static void SplitString(
        const std::string &s,
        char c,
        std::vector<std::string> *components);

    static std::string JoinStrings(
        const std::vector<std::string> &components,
        const std::string &separator);

private:
    StringHelper() = delete;

    static void Tokenize(const std::string &in,
        std::vector<std::string> *vec);
};

}  // namespace android

#endif  // STRING_HELPER_H_

