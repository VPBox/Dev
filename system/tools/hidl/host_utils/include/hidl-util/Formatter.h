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

#ifndef FORMATTER_H_

#define FORMATTER_H_

#include <functional>
#include <string>

namespace android {

// Two styles to use a Formatter.
// One is with .indent() calls and operator<<.
//     out << "if (good) {\n"; out.indent(); out << "blah\nblah\n"; out.unindent(); out << "}\n";
// The other is with chain calls and lambda functions
//     out.sIf("good", [&] { out("blah").endl()("blah").endl(); }).endl();
struct Formatter {
    static Formatter invalid() { return Formatter(); }

    // Assumes ownership of file. Directed to stdout if file == NULL.
    Formatter(FILE* file, size_t spacesPerIndent = 4);
    Formatter(Formatter&&) = default;
    ~Formatter();

    void indent(size_t level = 1);
    void unindent(size_t level = 1);

    // Note that The last \n after the last line is NOT added automatically.
    // out.indent(2, [&] {
    //     out << "Meow\n";
    // });
    Formatter& indent(size_t level, const std::function<void(void)>& func);

    // Note that The last \n after the last line is NOT added automatically.
    // out.indent([&] {
    //     out << "Meow\n";
    // });
    Formatter& indent(const std::function<void(void)>& func);

    // A block inside braces.
    // * No space will be added before the opening brace.
    // * The last \n before the closing brace is added automatically.
    // * There will NOT be a \n after the closing brace.
    // out.block([&] {
    //     out << "one();\n"
    //         << "two();\n";
    // });
    // is equivalent to
    // out << "{\n"
    //     << "one();\ntwo();\n" // func()
    //     << "}";
    Formatter& block(const std::function<void(void)>& func);

    // A synonym to (*this) << "\n";
    Formatter &endl();

    // out.sIf("z == 1", [&] {
    //     out << "doGoodStuff();\n";
    // }).sElseIf("z == 2", [&] {
    //     out << "doBadStuff();\n";
    // }).sElse([&] {
    //     out << "logFatal();\n";
    // }).endl();
    // note that there will be a space before the "else"-s.
    Formatter& sIf(const std::string& cond, const std::function<void(void)>& block);
    Formatter& sElseIf(const std::string& cond, const std::function<void(void)>& block);
    Formatter& sElse(const std::function<void(void)>& block);

    // out.sFor("int i = 0; i < 10; i++", [&] {
    //     out << "printf(\"%d\", i);\n";
    // }).endl();
    Formatter& sFor(const std::string& stmts, const std::function<void(void)>& block);

    // out.sTry([&] {
    //     out << "throw RemoteException();\n"
    // }).sCatch("RemoteException ex", [&] {
    //     out << "ex.printStackTrace();\n"
    // }).sFinally([&] {
    //     // cleanup
    // }).endl();
    // note that there will be a space before the "catch"-s.
    Formatter& sTry(const std::function<void(void)>& block);
    Formatter& sCatch(const std::string& exception, const std::function<void(void)>& block);
    Formatter& sFinally(const std::function<void(void)>& block);

    // out.sWhile("z < 10", [&] {
    //     out << "z++;\n";
    // }).endl();
    Formatter& sWhile(const std::string& cond, const std::function<void(void)>& block);

    // out.join(v.begin(), v.end(), ",", [&](const auto &e) {
    //     out << toString(e);
    // });
    template <typename I>
    Formatter& join(
        const I begin, const I end, const std::string& separator,
        const std::function<void(const typename std::iterator_traits<I>::value_type&)>& func);

    Formatter &operator<<(const std::string &out);

    Formatter &operator<<(char c);
    Formatter &operator<<(signed char c);
    Formatter &operator<<(unsigned char c);

    Formatter &operator<<(short c);
    Formatter &operator<<(unsigned short c);
    Formatter &operator<<(int c);
    Formatter &operator<<(unsigned int c);
    Formatter &operator<<(long c);
    Formatter &operator<<(unsigned long c);
    Formatter &operator<<(long long c);
    Formatter &operator<<(unsigned long long c);
    Formatter &operator<<(float c);
    Formatter &operator<<(double c);
    Formatter &operator<<(long double c);

    // Any substrings matching "space" will be stripped out of the output.
    void setNamespace(const std::string &space);

    // Puts a prefix before each line. This is useful if
    // you want to start a // comment block, for example.
    // The prefix will be put before the indentation.
    // Will be effective the next time cursor is at the start of line.
    void setLinePrefix(const std::string& prefix);
    // Remove the line prefix.
    void unsetLinePrefix();

    bool isValid() const;

   private:
    // Creates an invalid formatter object.
    Formatter();

    FILE* mFile;  // invalid if nullptr
    size_t mIndentDepth;
    size_t mSpacesPerIndent;
    bool mAtStartOfLine;

    std::string mSpace;
    std::string mLinePrefix;

    void output(const std::string &text) const;

    Formatter(const Formatter&) = delete;
    void operator=(const Formatter&) = delete;
};

template <typename I>
Formatter& Formatter::join(
    const I begin, const I end, const std::string& separator,
    const std::function<void(const typename std::iterator_traits<I>::value_type&)>& func) {
    for (I iter = begin; iter != end; ++iter) {
        if (iter != begin) {
            (*this) << separator;
        }
        func(*iter);
    }
    return (*this);
}

}  // namespace android

#endif  // FORMATTER_H_

