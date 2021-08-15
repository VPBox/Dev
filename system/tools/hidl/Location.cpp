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

#include "Location.h"

#include <android-base/logging.h>

namespace android {

Position::Position(std::string filename, size_t line, size_t column)
    : mFilename(filename), mLine(line), mColumn(column) {}

const std::string& Position::filename() const {
    return mFilename;
}

size_t Position::line() const {
    return mLine;
}

size_t Position::column() const {
    return mColumn;
}

bool Position::inSameFile(const Position& lhs, const Position& rhs) {
    return lhs.mFilename == rhs.mFilename;
}

bool Position::operator<(const Position& pos) const {
    CHECK(inSameFile(*this, pos)) << "Cannot compare positions in different files";
    if (mLine == pos.mLine) {
        return mColumn < pos.mColumn;
    }
    return mLine < pos.mLine;
}

std::ostream& operator<<(std::ostream& ostr, const Position& pos) {
    if (!pos.filename().empty()) {
        ostr << pos.filename() << ":";
    }
    return ostr << pos.line() << "." << pos.column();
}

////////////////////////////////////////

Location::Location(const Position& begin, const Position& end)
    : mIsValid(true), mBegin(begin), mEnd(end) {}

void Location::setLocation(const Position& begin, const Position& end) {
    mIsValid = true;
    mBegin = begin;
    mEnd = end;
}

bool Location::isValid() const {
    return mIsValid;
}

const Position& Location::begin() const {
    return mBegin;
}

const Position& Location::end() const {
    return mEnd;
}

Location Location::startOf(const std::string& path) {
    return Location(Position(path, 1, 1), Position(path, 1, 1));
}

bool Location::inSameFile(const Location& lhs, const Location& rhs) {
    return Position::inSameFile(lhs.mBegin, rhs.mBegin);
}

bool Location::intersect(const Location& lhs, const Location& rhs) {
    if (!inSameFile(lhs, rhs)) return false;
    return !(lhs.mEnd < rhs.mBegin || rhs.mEnd < lhs.mBegin);
}

bool Location::operator<(const Location& loc) const {
    CHECK(inSameFile(*this, loc)) << "Cannot compare locations in different files";
    CHECK(!intersect(*this, loc));
    return mEnd < loc.mBegin;
}

std::ostream& operator<<(std::ostream& ostr, const Location& loc) {
    Position last = Position(loc.end().filename(), loc.end().line(),
                             std::max<size_t>(1u, loc.end().column() - 1));
    ostr << loc.begin();
    if (loc.begin().filename() != last.filename()) {
        ostr << "-" << last;
    } else if (loc.begin().line() != last.line()) {
        ostr << "-" << last.line() << "." << last.column();
    } else if (loc.begin().column() != last.column()) {
        ostr << "-" << last.column();
    }
    return ostr;
}

}  // namespace android
