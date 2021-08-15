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

#ifndef NETDUTILS_DUMPWRITER_H_
#define NETDUTILS_DUMPWRITER_H_

#include <string>

namespace android {
namespace netdutils {

class DumpWriter {
  public:
    DumpWriter(int fd);

    void incIndent();
    void decIndent();

    void println(const std::string& line);
    template <size_t n>
    void println(const char line[n]) {
        println(std::string(line));
    }
    // Hint to the compiler that it should apply printf validation of
    // arguments (beginning at position 3) of the format (specified in
    // position 2). Note that position 1 is the implicit "this" argument.
    void println(const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
    void blankline() { println(""); }

  private:
    uint8_t mIndentLevel;
    int mFd;
};

class ScopedIndent {
  public:
    ScopedIndent() = delete;
    ScopedIndent(const ScopedIndent&) = delete;
    ScopedIndent(ScopedIndent&&) = delete;
    explicit ScopedIndent(DumpWriter& dw) : mDw(dw) { mDw.incIndent(); }
    ~ScopedIndent() { mDw.decIndent(); }
    ScopedIndent& operator=(const ScopedIndent&) = delete;
    ScopedIndent& operator=(ScopedIndent&&) = delete;

    // TODO: consider additional {inc,dec}Indent methods and a counter that
    // can be used to unwind all pending increments on exit.

  private:
    DumpWriter& mDw;
};

}  // namespace netdutils
}  // namespace android

#endif  // NETDUTILS_DUMPWRITER_H_
