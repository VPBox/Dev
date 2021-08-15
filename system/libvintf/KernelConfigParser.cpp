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

#include "KernelConfigParser.h"

#include <regex>

#define KEY "(CONFIG[\\w_]+)"
#define COMMENT "(?:#.*)"

static const std::regex sKeyValuePattern("^\\s*" KEY "\\s*=\\s*([^#]+)" COMMENT "?$");
static const std::regex sNotSetPattern("^\\s*#\\s*" KEY " is not set\\s*$");
static const std::regex sCommentPattern("^\\s*" COMMENT "$");

namespace android {
namespace vintf {

KernelConfigParser::KernelConfigParser(bool processComments, bool relaxedFormat)
    : mProcessComments(processComments), mRelaxedFormat(relaxedFormat) {}

status_t KernelConfigParser::finish() {
    return process("\n", 1 /* sizeof "\n" */);
}

std::stringbuf* KernelConfigParser::error() const {
    return mError.rdbuf();
}

std::map<std::string, std::string>& KernelConfigParser::configs() {
    return mConfigs;
}

const std::map<std::string, std::string>& KernelConfigParser::configs() const {
    return mConfigs;
}

// trim spaces between value and #, value and end of line
std::string trimTrailingSpaces(const std::string& s) {
    auto r = s.rbegin();
    for (; r != s.rend() && std::isspace(*r); ++r)
        ;
    return std::string{s.begin(), r.base()};
}

status_t KernelConfigParser::processRemaining() {

    if (mRemaining.empty()) {
        return OK;
    }

    std::smatch match;

    if (mRelaxedFormat) {
        // Allow free format like "   CONFIG_FOO  = bar    #trailing comments"
        if (std::regex_match(mRemaining, match, sKeyValuePattern)) {
            if (mConfigs.emplace(match[1], trimTrailingSpaces(match[2])).second) {
                return OK;
            }
            mError << "Duplicated key in configs: " << match[1] << "\n";
            return UNKNOWN_ERROR;
        }
    } else {
        // No spaces. Strictly like "CONFIG_FOO=bar"
        size_t equalPos = mRemaining.find('=');
        if (equalPos != std::string::npos) {
            std::string key = mRemaining.substr(0, equalPos);
            std::string value = mRemaining.substr(equalPos + 1);
            if (mConfigs.emplace(std::move(key), std::move(value)).second) {
                return OK;
            }
            mError << "Duplicated key in configs: " << mRemaining.substr(0, equalPos) << "\n";
            return UNKNOWN_ERROR;
        }
    }

    if (mProcessComments && std::regex_match(mRemaining, match, sNotSetPattern)) {
        if (mConfigs.emplace(match[1], "n").second) {
            return OK;
        }
        mError << "Key " << match[1] << " is set but commented as not set"
               << "\n";
        return UNKNOWN_ERROR;
    }

    if (mRelaxedFormat) {
        // Allow free format like "   #comments here"
        if (std::regex_match(mRemaining, match, sCommentPattern)) {
            return OK;
        }
    } else {
        // No leading spaces before the comment
        if (mRemaining.at(0) == '#') {
            return OK;
        }
    }

    mError << "Unrecognized line in configs: " << mRemaining << "\n";
    return UNKNOWN_ERROR;
}

status_t KernelConfigParser::process(const char* buf, size_t len) {
    const char* begin = buf;
    const char* end = buf;
    const char* stop = buf + len;
    status_t err = OK;
    while (end < stop) {
        if (*end == '\n') {
            mRemaining.insert(mRemaining.size(), begin, end - begin);
            status_t newErr = processRemaining();
            if (newErr != OK && err == OK) {
                err = newErr;
                // but continue to get more
            }
            mRemaining.clear();
            begin = end + 1;
        }
        end++;
    }
    mRemaining.insert(mRemaining.size(), begin, end - begin);
    return err;
}

status_t KernelConfigParser::processAndFinish(const char* buf, size_t len) {
    status_t err = process(buf, len);
    if (err != OK) {
        return err;
    }
    return finish();
}

status_t KernelConfigParser::processAndFinish(const std::string& content) {
    return processAndFinish(content.c_str(), content.size());
}

}  // namespace vintf
}  // namespace android
