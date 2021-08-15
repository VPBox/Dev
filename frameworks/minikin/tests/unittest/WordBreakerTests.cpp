/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "WordBreaker.h"

#include <cstdio>

#include <gtest/gtest.h>
#include <unicode/uclean.h>
#include <unicode/udata.h>

#include "UnicodeUtils.h"

#ifndef NELEM
#define NELEM(x) ((sizeof(x) / sizeof((x)[0])))
#endif

#define UTF16(codepoint) U16_LEAD(codepoint), U16_TRAIL(codepoint)

namespace minikin {

TEST(WordBreakerTest, basic) {
    uint16_t buf[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(6, breaker.followingWithLocale(Locale("en-US"), 0));  // after "hello "
    EXPECT_EQ(0, breaker.wordStart());                              // "hello"
    EXPECT_EQ(5, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ(6, breaker.current());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(6, breaker.wordStart());               // "world"
    EXPECT_EQ(11, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ(11, breaker.current());
}

TEST(WordBreakerTest, softHyphen) {
    uint16_t buf[] = {'h', 'e', 'l', 0x00AD, 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    // after "hel{SOFT HYPHEN}lo "
    EXPECT_EQ(7, breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());  // "hel{SOFT HYPHEN}lo"
    EXPECT_EQ(6, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(7, breaker.wordStart());               // "world"
    EXPECT_EQ(12, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, hardHyphen) {
    // Hyphens should not allow breaks anymore.
    uint16_t buf[] = {'s', 'u', 'g', 'a', 'r', '-', 'f', 'r', 'e', 'e'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, postfixAndPrefix) {
    uint16_t buf[] = {'U', 'S', 0x00A2, ' ', 'J', 'P', 0x00A5};  // US¢ JP¥
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());

    EXPECT_EQ(4, breaker.followingWithLocale(Locale("en-US"), 0));  // after CENT SIGN
    EXPECT_EQ(0, breaker.wordStart());                              // "US¢"
    EXPECT_EQ(3, breaker.wordEnd());

    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end of string
    EXPECT_EQ(4, breaker.wordStart());               // "JP¥"
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.wordEnd());
}

TEST(WordBreakerTest, myanmarKinzi) {
    uint16_t buf[] = {0x1004, 0x103A, 0x1039, 0x1000, 0x102C};  // NGA, ASAT, VIRAMA, KA, UU
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());

    // end of string
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.wordEnd());
}

TEST(WordBreakerTest, zwjEmojiSequences) {
    uint16_t buf[] = {
            // man + zwj + heart + zwj + man
            UTF16(0x1F468), 0x200D, 0x2764, 0x200D, UTF16(0x1F468),
            // woman + zwj + heart + zwj + kiss mark + zwj + woman
            UTF16(0x1F469), 0x200D, 0x2764, 0x200D, UTF16(0x1F48B), 0x200D, UTF16(0x1F469),
            // eye + zwj + left speech bubble
            UTF16(0x1F441), 0x200D, UTF16(0x1F5E8),
            // CAT FACE + zwj + BUST IN SILHOUETTE
            UTF16(0x1F431), 0x200D, UTF16(0x1F464),
    };
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    // after man + zwj + heart + zwj + man
    EXPECT_EQ(7, breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(7, breaker.wordEnd());
    EXPECT_EQ(17, breaker.next());  // after woman + zwj + heart + zwj + woman
    EXPECT_EQ(7, breaker.wordStart());
    EXPECT_EQ(17, breaker.wordEnd());
    EXPECT_EQ(22, breaker.next());  // after eye + zwj + left speech bubble
    EXPECT_EQ(17, breaker.wordStart());
    EXPECT_EQ(22, breaker.wordEnd());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(22, breaker.wordStart());
    EXPECT_EQ(27, breaker.wordEnd());
}

TEST(WordBreakerTest, emojiWithModifier) {
    uint16_t buf[] = {
            UTF16(0x1F466), UTF16(0x1F3FB),  // boy + type 1-2 fitzpatrick modifier
            0x270C, 0xFE0F,
            UTF16(0x1F3FF)  // victory hand + emoji style + type 6 fitzpatrick modifier
    };
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    // after boy + type 1-2 fitzpatrick modifier
    EXPECT_EQ(4, breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(4, breaker.wordEnd());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(4, breaker.wordStart());
    EXPECT_EQ(8, breaker.wordEnd());
}

TEST(WordBreakerTest, unicode10Emoji) {
    // Should break between emojis.
    uint16_t buf[] = {
            // SLED + SLED
            UTF16(0x1F6F7), UTF16(0x1F6F7),
            // SLED + VS15 + SLED
            UTF16(0x1F6F7), 0xFE0E, UTF16(0x1F6F7),
            // WHITE SMILING FACE + SLED
            0x263A, UTF16(0x1F6F7),
            // WHITE SMILING FACE + VS16 + SLED
            0x263A, 0xFE0F, UTF16(0x1F6F7),
    };
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(2, breaker.followingWithLocale(Locale("en"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(2, breaker.wordEnd());

    EXPECT_EQ(4, breaker.next());
    EXPECT_EQ(2, breaker.wordStart());
    EXPECT_EQ(4, breaker.wordEnd());

    EXPECT_EQ(7, breaker.next());
    EXPECT_EQ(4, breaker.wordStart());
    EXPECT_EQ(7, breaker.wordEnd());

    EXPECT_EQ(9, breaker.next());
    EXPECT_EQ(7, breaker.wordStart());
    EXPECT_EQ(9, breaker.wordEnd());

    EXPECT_EQ(10, breaker.next());
    EXPECT_EQ(9, breaker.wordStart());
    EXPECT_EQ(10, breaker.wordEnd());

    EXPECT_EQ(12, breaker.next());
    EXPECT_EQ(10, breaker.wordStart());
    EXPECT_EQ(12, breaker.wordEnd());

    EXPECT_EQ(14, breaker.next());
    EXPECT_EQ(12, breaker.wordStart());
    EXPECT_EQ(14, breaker.wordEnd());

    EXPECT_EQ(16, breaker.next());
    EXPECT_EQ(14, breaker.wordStart());
    EXPECT_EQ(16, breaker.wordEnd());
}

TEST(WordBreakerTest, flagsSequenceSingleFlag) {
    const std::string kFlag = "U+1F3F4";
    const std::string flags = kFlag + " " + kFlag;

    const int kFlagLength = 2;
    const size_t BUF_SIZE = kFlagLength * 2;

    uint16_t buf[BUF_SIZE];
    size_t size;
    ParseUnicode(buf, BUF_SIZE, flags.c_str(), &size, nullptr);

    WordBreaker breaker;
    breaker.setText(buf, size);
    EXPECT_EQ(0, breaker.current());
    // end of the first flag
    EXPECT_EQ(kFlagLength, breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(kFlagLength, breaker.wordEnd());
    EXPECT_EQ(static_cast<ssize_t>(size), breaker.next());
    EXPECT_EQ(kFlagLength, breaker.wordStart());
    EXPECT_EQ(kFlagLength * 2, breaker.wordEnd());
}

TEST(WordBreakerTest, flagsSequence) {
    // U+1F3F4 U+E0067 U+E0062 U+E0073 U+E0063 U+E0074 U+E007F is emoji tag sequence for the flag
    // of Scotland.
    const std::string kFlagSequence = "U+1F3F4 U+E0067 U+E0062 U+E0073 U+E0063 U+E0074 U+E007F";
    const std::string flagSequence = kFlagSequence + " " + kFlagSequence;

    const int kFlagLength = 14;
    const size_t BUF_SIZE = kFlagLength * 2;

    uint16_t buf[BUF_SIZE];
    size_t size;
    ParseUnicode(buf, BUF_SIZE, flagSequence.c_str(), &size, nullptr);

    WordBreaker breaker;
    breaker.setText(buf, size);
    EXPECT_EQ(0, breaker.current());
    // end of the first flag sequence
    EXPECT_EQ(kFlagLength, breaker.followingWithLocale(Locale("en-US"), 0));
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(kFlagLength, breaker.wordEnd());
    EXPECT_EQ(static_cast<ssize_t>(size), breaker.next());
    EXPECT_EQ(kFlagLength, breaker.wordStart());
    EXPECT_EQ(kFlagLength * 2, breaker.wordEnd());
}

TEST(WordBreakerTest, punct) {
    uint16_t buf[] = {0x00A1, 0x00A1, 'h', 'e', 'l', 'l', 'o', ',',
                      ' ',    'w',    'o', 'r', 'l', 'd', '!', '!'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(9, breaker.followingWithLocale(Locale("en-US"), 0));  // after "¡¡hello, "
    EXPECT_EQ(2, breaker.wordStart());                              // "hello"
    EXPECT_EQ(7, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(9, breaker.wordStart());               // "world"
    EXPECT_EQ(14, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, email) {
    uint16_t buf[] = {'f', 'o', 'o', '@', 'e', 'x', 'a', 'm', 'p',
                      'l', 'e', '.', 'c', 'o', 'm', ' ', 'x'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(11, breaker.followingWithLocale(Locale("en-US"), 0));  // after "foo@example"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(16, breaker.next());  // after ".com "
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(16, breaker.wordStart());              // "x"
    EXPECT_EQ(17, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, mailto) {
    uint16_t buf[] = {'m', 'a', 'i', 'l', 't', 'o', ':', 'f', 'o', 'o', '@', 'e',
                      'x', 'a', 'm', 'p', 'l', 'e', '.', 'c', 'o', 'm', ' ', 'x'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(7, breaker.followingWithLocale(Locale("en-US"), 0));  // after "mailto:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(18, breaker.next());  // after "foo@example"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(23, breaker.next());  // after ".com "
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(23, breaker.wordStart());              // "x"
    EXPECT_EQ(24, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

// The current logic always places a line break after a detected email address or URL
// and an immediately following non-ASCII character.
TEST(WordBreakerTest, emailNonAscii) {
    uint16_t buf[] = {'f', 'o', 'o', '@', 'e', 'x', 'a', 'm',
                      'p', 'l', 'e', '.', 'c', 'o', 'm', 0x4E00};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(11, breaker.followingWithLocale(Locale("en-US"), 0));  // after "foo@example"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(15, breaker.next());  // after ".com"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(15, breaker.wordStart());              // "一"
    EXPECT_EQ(16, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, emailCombining) {
    uint16_t buf[] = {'f', 'o', 'o', '@', 'e', 'x', 'a',    'm', 'p',
                      'l', 'e', '.', 'c', 'o', 'm', 0x0303, ' ', 'x'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(11, breaker.followingWithLocale(Locale("en-US"), 0));  // after "foo@example"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(17, breaker.next());  // after ".com̃ "
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(17, breaker.wordStart());              // "x"
    EXPECT_EQ(18, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, lonelyAt) {
    uint16_t buf[] = {'a', ' ', '@', ' ', 'b'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(2, breaker.followingWithLocale(Locale("en-US"), 0));  // after "a "
    EXPECT_EQ(0, breaker.wordStart());                              // "a"
    EXPECT_EQ(1, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ(4, breaker.next());  // after "@ "
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(4, breaker.wordStart());               // "b"
    EXPECT_EQ(5, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, url) {
    uint16_t buf[] = {'h', 't', 't', 'p', ':', '/', '/', 'e', 'x', 'a',
                      'm', 'p', 'l', 'e', '.', 'c', 'o', 'm', ' ', 'x'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(5, breaker.followingWithLocale(Locale("en-US"), 0));  // after "http:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(7, breaker.next());  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(14, breaker.next());  // after "example"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(19, breaker.next());  // after ".com "
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_EQ(19, breaker.wordStart());              // "x"
    EXPECT_EQ(20, breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

// Breaks according to section 14.12 of Chicago Manual of Style, *URLs or DOIs and line breaks*
TEST(WordBreakerTest, urlBreakChars) {
    uint16_t buf[] = {'h', 't', 't', 'p', ':', '/', '/', 'a', '.', 'b', '/',
                      '~', 'c', ',', 'd', '-', 'e', '?', 'f', '=', 'g', '&',
                      'h', '#', 'i', '%', 'j', '_', 'k', '/', 'l'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(5, breaker.followingWithLocale(Locale("en-US"), 0));  // after "http:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(7, breaker.next());  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(8, breaker.next());  // after "a"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(10, breaker.next());  // after ".b"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(11, breaker.next());  // after "/"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(13, breaker.next());  // after "~c"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(15, breaker.next());  // after ",d"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(17, breaker.next());  // after "-e"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(19, breaker.next());  // after "?f"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(20, breaker.next());  // after "="
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(21, breaker.next());  // after "g"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(22, breaker.next());  // after "&"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(23, breaker.next());  // after "h"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(25, breaker.next());  // after "#i"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(27, breaker.next());  // after "%j"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ(29, breaker.next());  // after "_k"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(1, breaker.breakBadness());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(0, breaker.breakBadness());
}

TEST(WordBreakerTest, urlNoHyphenBreak) {
    uint16_t buf[] = {'h', 't', 't', 'p', ':', '/', '/', 'a', '-', '/', 'b'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(5, breaker.followingWithLocale(Locale("en-US"), 0));  // after "http:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(7, breaker.next());  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(8, breaker.next());  // after "a"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
}

TEST(WordBreakerTest, urlEndsWithSlash) {
    uint16_t buf[] = {'h', 't', 't', 'p', ':', '/', '/', 'a', '/'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(5, breaker.followingWithLocale(Locale("en-US"), 0));  // after "http:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(7, breaker.next());  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(8, breaker.next());  // after "a"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.next());  // end
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
}

TEST(WordBreakerTest, emailStartsWithSlash) {
    uint16_t buf[] = {'/', 'a', '@', 'b'};
    WordBreaker breaker;
    breaker.setText(buf, NELEM(buf));
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ((ssize_t)NELEM(buf), breaker.followingWithLocale(Locale("en-US"), 0));  // end
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
}

TEST(WordBreakerTest, setLocaleInsideUrl) {
    std::vector<uint16_t> buf = utf8ToUtf16("Hello http://abc/d.html World");
    WordBreaker breaker;
    breaker.setText(buf.data(), buf.size());
    EXPECT_EQ(0, breaker.current());
    EXPECT_EQ(6, breaker.followingWithLocale(Locale("en-US"), 0));  // after "Hello "
    EXPECT_EQ(0, breaker.wordStart());
    EXPECT_EQ(5, breaker.wordEnd());

    EXPECT_EQ(6, breaker.current());
    EXPECT_EQ(11, breaker.next());  // after "http:"

    // Restart from middle point of the URL. It should return the same previous break point.
    EXPECT_EQ(11, breaker.followingWithLocale(Locale("en-US"), 6));  // after "http:"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());

    EXPECT_EQ(13, breaker.next());  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());

    // Restart from middle point of the URL. It should return the same previous break point.
    EXPECT_EQ(13, breaker.followingWithLocale(Locale("en-US"), 12));  // after "//"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(16, breaker.next());  // after "abc"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(18, breaker.next());  // after "/d"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());
    EXPECT_EQ(24, breaker.next());  // after ".html"
    EXPECT_TRUE(breaker.wordStart() >= breaker.wordEnd());

    EXPECT_EQ(29, breaker.next());  // after "World"
    EXPECT_EQ(24, breaker.wordStart());
    EXPECT_EQ(29, breaker.wordEnd());
}

// b/68669534
TEST(WordBreakerTest, spaceAfterSpace) {
    const std::vector<uint16_t> SPACES = {
            '\t',    // TAB
            0x1680,  // OGHAM SPACE MARK
            0x3000,  // IDEOGRAPHIC SPACE
    };

    constexpr uint16_t CHAR_SPACE = 0x0020;

    for (uint16_t sp : SPACES) {
        char msg[64] = {};
        snprintf(msg, sizeof(msg), "Test Space: U+%04X", sp);
        SCOPED_TRACE(msg);

        std::vector<uint16_t> buf = {'a', CHAR_SPACE, sp, 'b'};
        WordBreaker breaker;
        breaker.setText(buf.data(), buf.size());

        EXPECT_EQ(0, breaker.current());
        EXPECT_EQ(2, breaker.followingWithLocale(Locale("en-US"), 0));  // after "a "
        EXPECT_EQ(0, breaker.wordStart());
        EXPECT_EQ(1, breaker.wordEnd());

        EXPECT_EQ(2, breaker.current());
        EXPECT_EQ(3, breaker.next());  // after CHAR_SPACE character.
        EXPECT_EQ(2, breaker.wordStart());
        EXPECT_EQ(2, breaker.wordEnd());

        EXPECT_EQ(3, breaker.current());
        EXPECT_EQ(4, breaker.next());  // after sp character.
        EXPECT_EQ(3, breaker.wordStart());
        EXPECT_EQ(4, breaker.wordEnd());
    }
}

class TestableICULineBreakerPoolImpl : public ICULineBreakerPoolImpl {
public:
    TestableICULineBreakerPoolImpl() : ICULineBreakerPoolImpl() {}

    using ICULineBreakerPoolImpl::getPoolSize;
    using ICULineBreakerPoolImpl::MAX_POOL_SIZE;
};

TEST(WordBreakerTest, LineBreakerPool_acquire_without_release) {
    TestableICULineBreakerPoolImpl pool;

    const Locale enUS("en-Latn-US");
    const Locale frFR("fr-Latn-FR");

    // All following three breakers must be the different instances.
    ICULineBreakerPool::Slot enUSBreaker = pool.acquire(enUS);
    ICULineBreakerPool::Slot enUSBreaker2 = pool.acquire(enUS);
    ICULineBreakerPool::Slot frFRBreaker = pool.acquire(frFR);

    EXPECT_NE(nullptr, enUSBreaker.breaker.get());
    EXPECT_NE(nullptr, enUSBreaker2.breaker.get());
    EXPECT_NE(nullptr, frFRBreaker.breaker.get());

    EXPECT_NE(enUSBreaker.breaker.get(), enUSBreaker2.breaker.get());
    EXPECT_NE(enUSBreaker.breaker.get(), frFRBreaker.breaker.get());
    EXPECT_NE(enUSBreaker2.breaker.get(), frFRBreaker.breaker.get());

    EXPECT_EQ(enUSBreaker.localeId, enUSBreaker2.localeId);
    EXPECT_NE(enUSBreaker.localeId, frFRBreaker.localeId);
    EXPECT_NE(enUSBreaker2.localeId, frFRBreaker.localeId);
}

TEST(WordBreakerTest, LineBreakerPool_acquire_with_release) {
    TestableICULineBreakerPoolImpl pool;

    const Locale enUS("en-Latn-US");
    const Locale frFR("fr-Latn-FR");

    // All following three breakers must be the different instances.
    ICULineBreakerPool::Slot enUSBreaker = pool.acquire(enUS);

    uint64_t enUSBreakerLocaleId = enUSBreaker.localeId;
    UBreakIterator* enUSBreakerPtr = enUSBreaker.breaker.get();

    pool.release(std::move(enUSBreaker));
    EXPECT_EQ(nullptr, enUSBreaker.breaker.get());

    // acquire must return a different instance if the locale is different.
    ICULineBreakerPool::Slot frFRBreaker = pool.acquire(frFR);
    EXPECT_NE(enUSBreakerPtr, frFRBreaker.breaker.get());
    EXPECT_NE(enUSBreakerLocaleId, frFRBreaker.localeId);

    // acquire must return the same instance as released before if the locale is the same.
    ICULineBreakerPool::Slot enUSBreaker2 = pool.acquire(enUS);
    EXPECT_EQ(enUSBreakerPtr, enUSBreaker2.breaker.get());
    EXPECT_EQ(enUSBreakerLocaleId, enUSBreaker2.localeId);
}

TEST(WordBreakerTest, LineBreakerPool_exceeds_pool_size) {
    const size_t MAX_POOL_SIZE = TestableICULineBreakerPoolImpl::MAX_POOL_SIZE;
    TestableICULineBreakerPoolImpl pool;

    const Locale enUS("en-Latn-US");

    ICULineBreakerPool::Slot slots[MAX_POOL_SIZE * 2];

    // Make pool full.
    for (size_t i = 0; i < MAX_POOL_SIZE * 2; i++) {
        slots[i] = pool.acquire(enUS);
        EXPECT_EQ(0U, pool.getPoolSize());
    }

    for (size_t i = 0; i < MAX_POOL_SIZE; i++) {
        pool.release(std::move(slots[i]));
        EXPECT_EQ(i + 1, pool.getPoolSize());
    }

    for (size_t i = MAX_POOL_SIZE; i < MAX_POOL_SIZE * 2; i++) {
        pool.release(std::move(slots[i]));
        EXPECT_EQ(MAX_POOL_SIZE, pool.getPoolSize());
    }
}

}  // namespace minikin
