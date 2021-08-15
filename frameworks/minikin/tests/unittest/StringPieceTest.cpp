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

#include "StringPiece.h"

#include <gtest/gtest.h>

namespace minikin {

TEST(StringPieceTest, basics) {
    {
        StringPiece s(nullptr);
        EXPECT_EQ(nullptr, s.data());
        EXPECT_EQ(0u, s.size());
        EXPECT_TRUE(s.empty());
    }
    {
        StringPiece s("");
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(0u, s.size());
        EXPECT_TRUE(s.empty());
    }
    {
        StringPiece s("", 0);
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(0u, s.size());
        EXPECT_TRUE(s.empty());
    }
    {
        StringPiece s("abcde");
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(5u, s.size());
        EXPECT_FALSE(s.empty());
        EXPECT_EQ("abcde", s);
        EXPECT_NE("abc", s);
    }
    {
        StringPiece s("abcde", 5);
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(5u, s.size());
        EXPECT_FALSE(s.empty());
        EXPECT_EQ("abcde", s);
        EXPECT_NE("abc", s);
    }
    {
        StringPiece s("abcde", 3);
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(3u, s.size());
        EXPECT_FALSE(s.empty());
        EXPECT_EQ("abc", s);
        EXPECT_NE("abcde", s);
    }
    {
        const char* kText = "abcde";
        StringPiece s(kText + 2, 3);
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(3u, s.size());
        EXPECT_FALSE(s.empty());
        EXPECT_EQ("cde", s);
        EXPECT_NE("abcde", s);
    }
    {
        const char* kText = "abcde";
        StringPiece s(kText + 2);
        EXPECT_NE(nullptr, s.data());
        EXPECT_EQ(3u, s.size());
        EXPECT_FALSE(s.empty());
        EXPECT_EQ("cde", s);
        EXPECT_NE("abcde", s);
    }
}

TEST(StringPieceTest, substr) {
    StringPiece s("abcde");
    EXPECT_EQ("", s.substr(0, 0));
    EXPECT_EQ("a", s.substr(0, 1));
    EXPECT_EQ("abc", s.substr(0, 3));
    EXPECT_EQ("cde", s.substr(2, 3));
    EXPECT_EQ("", s.substr(2, 0));
    EXPECT_EQ("", s.substr(5, 0));
}

TEST(StringPieceTest, find) {
    StringPiece s("mississippi");
    EXPECT_EQ(1u, s.find(0, 'i'));
    EXPECT_EQ(1u, s.find(1, 'i'));
    EXPECT_EQ(4u, s.find(2, 'i'));
    EXPECT_EQ(4u, s.find(3, 'i'));
    EXPECT_EQ(4u, s.find(4, 'i'));
    EXPECT_EQ(7u, s.find(5, 'i'));
    EXPECT_EQ(7u, s.find(6, 'i'));
    EXPECT_EQ(7u, s.find(7, 'i'));
    EXPECT_EQ(10u, s.find(8, 'i'));
    EXPECT_EQ(10u, s.find(9, 'i'));
    EXPECT_EQ(10u, s.find(10, 'i'));
    EXPECT_EQ(11u, s.find(11, 'i'));

    EXPECT_EQ(11u, s.find(12, 'i'));  // search index is out of bounds.
}

TEST(StringPieceTest, find_empty) {
    StringPiece s("");
    EXPECT_EQ(0u, s.find(0, 'a'));
}

TEST(SplitIteratorTest, split) {
    {
        StringPiece s("");
        SplitIterator it(s, ',');
        EXPECT_FALSE(it.hasNext());
    }
    {
        StringPiece s("abcde");
        SplitIterator it(s, ',');
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("abcde", it.next());
        EXPECT_FALSE(it.hasNext());
    }
    {
        StringPiece s("a,bb,ccc,dddd,eeeee");
        SplitIterator it(s, ',');
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("a", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("bb", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("ccc", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("dddd", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("eeeee", it.next());
        EXPECT_FALSE(it.hasNext());
    }
    {
        StringPiece s(",,,,");
        SplitIterator it(s, ',');
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_FALSE(it.hasNext());
    }
    {
        StringPiece s(",a,,b,");
        SplitIterator it(s, ',');
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("a", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("b", it.next());
        EXPECT_TRUE(it.hasNext());
        EXPECT_EQ("", it.next());
        EXPECT_FALSE(it.hasNext());
    }
}

}  // namespace minikin
