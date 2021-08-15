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

#include <algorithm>
#include <cstdint>
#include <utility>

#include <gtest/gtest.h>

#include "netdutils/MemBlock.h"
#include "netdutils/Slice.h"

namespace android {
namespace netdutils {

namespace {

constexpr unsigned DNS_PACKET_SIZE = 512;
constexpr int ARBITRARY_VALUE = 0x55;

MemBlock makeArbitraryMemBlock(size_t len) {
    MemBlock result(len);
    // Do some fictional work before returning.
    for (Slice slice = result.get(); !slice.empty(); slice = drop(slice, 1)) {
        slice.base()[0] = ARBITRARY_VALUE;
    }
    return result;
}

void checkAllZeros(Slice slice) {
    for (; !slice.empty(); slice = drop(slice, 1)) {
        EXPECT_EQ(0U, slice.base()[0]);
    }
}

void checkArbitraryMemBlock(const MemBlock& block, size_t expectedSize) {
    Slice slice = block.get();
    EXPECT_EQ(expectedSize, slice.size());
    EXPECT_NE(nullptr, slice.base());
    for (; !slice.empty(); slice = drop(slice, 1)) {
        EXPECT_EQ(ARBITRARY_VALUE, slice.base()[0]);
    }
}

void checkHelloMello(Slice dest, Slice src) {
    EXPECT_EQ('h', dest.base()[0]);
    EXPECT_EQ('e', dest.base()[1]);
    EXPECT_EQ('l', dest.base()[2]);
    EXPECT_EQ('l', dest.base()[3]);
    EXPECT_EQ('o', dest.base()[4]);

    src.base()[0] = 'm';
    EXPECT_EQ('h', dest.base()[0]);
}

}  // namespace

TEST(MemBlockTest, Empty) {
    MemBlock empty;
    EXPECT_TRUE(empty.get().empty());
    EXPECT_EQ(nullptr, empty.get().base());
}

TEST(MemBlockTest, ExplicitZero) {
    MemBlock zero(0);
    EXPECT_TRUE(zero.get().empty());
    EXPECT_EQ(nullptr, zero.get().base());
}

TEST(MemBlockTest, BasicAllocation) {
    MemBlock dnsPacket(DNS_PACKET_SIZE);
    Slice slice = dnsPacket.get();
    EXPECT_EQ(DNS_PACKET_SIZE, slice.size());
    // Verify the space is '\0'-initialized.
    ASSERT_NO_FATAL_FAILURE(checkAllZeros(slice));
    EXPECT_NE(nullptr, slice.base());
}

TEST(MemBlockTest, MoveConstruction) {
    MemBlock block(makeArbitraryMemBlock(DNS_PACKET_SIZE));
    ASSERT_NO_FATAL_FAILURE(checkArbitraryMemBlock(block, DNS_PACKET_SIZE));
}

TEST(MemBlockTest, MoveAssignmentOrConstruction) {
    MemBlock block = makeArbitraryMemBlock(DNS_PACKET_SIZE);
    ASSERT_NO_FATAL_FAILURE(checkArbitraryMemBlock(block, DNS_PACKET_SIZE));
}

TEST(MemBlockTest, StdMoveAssignment) {
    constexpr unsigned SIZE = 10;

    MemBlock block;
    EXPECT_TRUE(block.get().empty());
    EXPECT_EQ(nullptr, block.get().base());

    {
        MemBlock block2 = makeArbitraryMemBlock(SIZE);
        EXPECT_EQ(SIZE, block2.get().size());
        // More fictional work.
        for (unsigned i = 0; i < SIZE; i++) {
            block2.get().base()[i] = i;
        }
        block = std::move(block2);
    }

    EXPECT_EQ(SIZE, block.get().size());
    for (unsigned i = 0; i < SIZE; i++) {
        EXPECT_EQ(i, block.get().base()[i]);
    }
}

TEST(MemBlockTest, ConstructionFromSlice) {
    uint8_t data[] = {'h', 'e', 'l', 'l', 'o'};
    Slice dataSlice(Slice(data, sizeof(data) / sizeof(data[0])));

    MemBlock dataCopy(dataSlice);
    ASSERT_NO_FATAL_FAILURE(checkHelloMello(dataCopy.get(), dataSlice));
}

TEST(MemBlockTest, ImplicitCastToSlice) {
    uint8_t data[] = {'h', 'e', 'l', 'l', 'o'};
    Slice dataSlice(Slice(data, sizeof(data) / sizeof(data[0])));

    MemBlock dataCopy(dataSlice.size());
    // NOTE: no explicit MemBlock::get().
    // Verify the space is '\0'-initialized.
    ASSERT_NO_FATAL_FAILURE(checkAllZeros(dataCopy));
    copy(dataCopy, dataSlice);
    ASSERT_NO_FATAL_FAILURE(checkHelloMello(dataCopy, dataSlice));
}

}  // namespace netdutils
}  // namespace android
