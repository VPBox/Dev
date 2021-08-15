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

#include "DocComment.h"

#include <android-base/strings.h>
#include <hidl-util/StringHelper.h>

#include <cctype>
#include <sstream>

#include <iostream>

namespace android {

DocComment::DocComment(const std::string& comment) {
    std::vector<std::string> lines = base::Split(base::Trim(comment), "\n");

    bool foundFirstLine = false;

    std::ostringstream is;
    for (size_t l = 0; l < lines.size(); l++) {
        const std::string& line = lines[l];

        // Delete prefixes like "    * ", "   *", or "    ".
        size_t idx = 0;
        for (; idx < line.size() && isspace(line[idx]); idx++)
            ;
        if (idx < line.size() && line[idx] == '*') idx++;
        if (idx < line.size() && line[idx] == ' ') idx++;

        bool isEmptyLine = idx == line.size();

        foundFirstLine = foundFirstLine || !isEmptyLine;
        if (!foundFirstLine) continue;

        is << line.substr(idx) << "\n";
    }

    mComment = is.str();
}

void DocComment::merge(const DocComment* comment) {
    mComment = mComment + "\n\n" + comment->mComment;
}

void DocComment::emit(Formatter& out) const {
    out << "/**\n";
    out.setLinePrefix(" * ");
    out << mComment;
    out.unsetLinePrefix();
    out << " */\n";
}

}  // namespace android
