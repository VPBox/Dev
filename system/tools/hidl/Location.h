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

#ifndef LOCATION_H_
#define LOCATION_H_

#include <stdint.h>
#include <ostream>
#include <string>

// Mimics for yy::location and yy::position
namespace android {

struct Position {
    Position() = default;
    Position(std::string filename, size_t line, size_t column);

    const std::string& filename() const;

    size_t line() const;
    size_t column() const;

    static bool inSameFile(const Position& lhs, const Position& rhs);

    // Precondition: inSameFile()
    bool operator<(const Position& pos) const;

   private:
    // File name to which this position refers.
    std::string mFilename;
    // Current line number.
    size_t mLine;
    // Current column number.
    size_t mColumn;
};

std::ostream& operator<<(std::ostream& ostr, const Position& pos);

struct Location {
    Location() = default;
    Location(const Position& begin, const Position& end);

    void setLocation(const Position& begin, const Position& end);

    bool isValid() const;
    const Position& begin() const;
    const Position& end() const;

    static Location startOf(const std::string& path);

    static bool inSameFile(const Location& lhs, const Location& rhs);
    static bool intersect(const Location& lhs, const Location& rhs);

    // Precondition: inSameFile() && !intersect()
    bool operator<(const Location& loc) const;

   private:
    bool mIsValid = false;

    // Beginning of the located region.
    Position mBegin;
    // End of the located region.
    Position mEnd;
};

std::ostream& operator<<(std::ostream& ostr, const Location& loc);

} // namespace android

#endif  // LOCATION_H_
