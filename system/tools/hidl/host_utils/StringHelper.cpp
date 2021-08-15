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

#include "StringHelper.h"

#include <sstream>
#include <regex>

#include <android-base/macros.h>
#include <android-base/logging.h>

#define UPPERCASE  "[A-Z0-9]+"
#define LOWERCASE "[a-z0-9]+"
#define CAPCASE "[A-Z0-9][a-z0-9]*"
static const std::regex kStartUppercase("^" UPPERCASE);
static const std::regex kStartLowercase("^" LOWERCASE);
static const std::regex kStartCapcase("^" CAPCASE);

namespace android {

std::string StringHelper::Uppercase(const std::string &in) {
    std::string out{in};

    for (auto &ch : out) {
        ch = toupper(ch);
    }

    return out;
}

std::string StringHelper::Lowercase(const std::string &in) {
    std::string out{in};

    for (auto &ch : out) {
        ch = tolower(ch);
    }

    return out;
}

std::string StringHelper::Capitalize(const std::string &in) {
    std::string out{in};

    if(!out.empty()) {
        out[0] = toupper(out[0]);
    }

    return out;
}

void StringHelper::Tokenize(const std::string &in,
        std::vector<std::string> *vec) {

    std::smatch match;
    if (in.empty()) {
        vec->clear();
        return;
    }
    std::string copy(in);
    vec->clear();
    std::vector<std::string> matches;

    copy = RTrimAll(copy, "_");
    while(!copy.empty()) {
        copy = LTrimAll(copy, "_");
        if (std::regex_search(copy, match, kStartLowercase))
            matches.push_back(match.str(0));
        if (std::regex_search(copy, match, kStartCapcase))
            matches.push_back(match.str(0));
        if (std::regex_search(copy, match, kStartUppercase))
            matches.push_back(match.str(0));
        if (!matches.empty()) {
            std::string &maxmatch = matches[0];
            for (std::string &match : matches)
                if(match.length() > maxmatch.length())
                    maxmatch = match;
            vec->push_back(maxmatch);
            copy = copy.substr(maxmatch.length());
            matches.clear();
            continue;
        }
        LOG(WARNING) << "Could not stylize \"" << in << "\"";
        // don't know what to do, so push back the rest of the string.
        vec->push_back(copy);
    }
}

std::string StringHelper::ToCamelCase(const std::string &in) {
    std::vector<std::string> components;
    Tokenize(in, &components);
    if (components.empty()) {
        if (!in.empty())
            LOG(WARNING) << "Could not stylize \"" << in << "\"";
        return in;
    }
    components[0] = Lowercase(components[0]);
    for (size_t i = 1; i < components.size(); i++) {
        components[i] = Capitalize(components[i]);
    }
    return JoinStrings(components, "");
}

std::string StringHelper::ToPascalCase(const std::string &in) {
    std::vector<std::string> components;
    Tokenize(in, &components);
    for (size_t i = 0; i < components.size(); i++) {
        components[i] = Capitalize(components[i]);
    }
    return JoinStrings(components, "");
}

std::string StringHelper::ToUpperSnakeCase(const std::string &in) {
    std::vector<std::string> components;
    Tokenize(in, &components);
    for (size_t i = 0; i < components.size(); i++) {
        components[i] = Uppercase(components[i]);
    }
    return JoinStrings(components, "_");
}

std::string StringHelper::ToLowerSnakeCase(const std::string &in) {
    std::vector<std::string> components;
    Tokenize(in, &components);
    for (size_t i = 0; i < components.size(); i++) {
        components[i] = Lowercase(components[i]);
    }
    return JoinStrings(components, "_");
}

std::string StringHelper::ToCase(StringHelper::Case c, const std::string &in) {
    switch(c) {
    case kCamelCase:
        return ToCamelCase(in);
    case kPascalCase:
        return ToPascalCase(in);
    case kUpperSnakeCase:
        return ToUpperSnakeCase(in);
    case kLowerSnakeCase:
        return ToLowerSnakeCase(in);
    case kNoCase:
        return in;
    }
    LOG(FATAL) << "Should not reach here.";
    return in;
}

bool StringHelper::EndsWith(const std::string &in, const std::string &suffix) {
    return in.size() >= suffix.size() &&
           in.substr(in.size() - suffix.size()) == suffix;
}

bool StringHelper::StartsWith(const std::string &in, const std::string &prefix) {
    return in.size() >= prefix.size() &&
           in.substr(0, prefix.size()) == prefix;
}

std::string StringHelper::RTrim(const std::string &in, const std::string &suffix) {
    if (EndsWith(in, suffix)) {
        return in.substr(0, in.size() - suffix.size());
    }

    return in;
}

std::string StringHelper::LTrim(const std::string &in, const std::string &prefix) {
    if (StartsWith(in, prefix)) {
        return in.substr(prefix.size());
    }

    return in;
}

std::string StringHelper::RTrimAll(const std::string &in, const std::string &suffix) {
    if (suffix.empty()) {
        return in;
    }

    std::string copy(in);
    while (EndsWith(copy, suffix)) {
        copy = copy.substr(0, copy.size() - suffix.size());
    }

    return copy;
}

std::string StringHelper::LTrimAll(const std::string &in, const std::string &prefix) {
    if (prefix.empty()) {
        return in;
    }

    std::string copy(in);
    while (StartsWith(copy, prefix)) {
        copy = copy.substr(prefix.size());
    }

    return copy;
}

void StringHelper::SplitString(
        const std::string &s, char c, std::vector<std::string> *components) {
    components->clear();

    size_t startPos = 0;
    size_t matchPos;
    while ((matchPos = s.find(c, startPos)) != std::string::npos) {
        components->push_back(s.substr(startPos, matchPos - startPos));
        startPos = matchPos + 1;
    }

    if (startPos <= s.length()) {
        components->push_back(s.substr(startPos));
    }
}

std::string StringHelper::JoinStrings(
        const std::vector<std::string> &components,
        const std::string &separator) {
    std::string out;
    bool first = true;
    for (const auto &component : components) {
        if (!first) {
            out += separator;
        }
        out += component;

        first = false;
    }

    return out;
}

}  // namespace android

