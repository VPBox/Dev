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

#include "Include.h"

namespace android {

Include::Include(const std::string &name, bool isLibrary)
    : Declaration(name), mIsLibrary(isLibrary)
    {}

Include::~Include() {}

bool Include::isLibrary() const {
    return mIsLibrary;
}

void Include::generateSource(Formatter &out) const {
    out << "// import "
        << getName();

    if (isLibrary()) {
        out << "/* library file */";
    } else {
        out << "/* local file */";
    }

    out << "\n";
}

void Include::processContents(AST &) {
    // nothing to do
}

} //namespace android