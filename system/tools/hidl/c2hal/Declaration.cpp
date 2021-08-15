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

#include "Declaration.h"

#include <hidl-util/StringHelper.h>
#include <regex>

namespace android {

static const std::regex RE_LEADING_SPACES("\n +");

Declaration::Declaration(const std::string &name)
    : mName(name)
    {}

Declaration::~Declaration() {}

const std::string& Declaration::getName() const {
    return mName;
}
void Declaration::setName(const std::string &name) {
    mName = name;
}

void Declaration::forceCamelCase() {
    mName = StringHelper::RTrim(mName, "_t");
    mName = StringHelper::ToCamelCase(mName);
}

void Declaration::forcePascalCase() {
    mName = StringHelper::RTrim(mName, "_t");
    mName = StringHelper::ToPascalCase(mName);
}

void Declaration::forceUpperSnakeCase() {
    mName = StringHelper::RTrim(mName, "_t");
    mName = StringHelper::ToUpperSnakeCase(mName);
}

const std::string& Declaration::getComment() const {
    return mComment;
}
void Declaration::setComment(const std::string &comment) {
    // remove excess leading whitespace
    mComment = regex_replace(comment, RE_LEADING_SPACES, "\n ");
}

void Declaration::generateCommentText(Formatter &out) const {
    if (!mComment.empty()) {
        out << mComment << "\n";
    }
}

void Declaration::generateParameterSource(Formatter &out) const {
    out << "/* UNKNOWN PARAMTER */" << "\n";
}

} //namespace android
