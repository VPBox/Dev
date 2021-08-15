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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <inttypes.h>
#include <linux/inet_diag.h>
#include <linux/sock_diag.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "bpf/BpfMap.h"
#include "bpf/BpfUtils.h"

using ::testing::Test;

namespace android {
namespace bpf {

using base::unique_fd;
using netdutils::StatusOr;

constexpr uint32_t TEST_MAP_SIZE = 10;
constexpr uint32_t TEST_KEY1 = 1;
constexpr uint32_t TEST_VALUE1 = 10;
constexpr const char PINNED_MAP_PATH[] = "/sys/fs/bpf/testMap";

class BpfMapTest : public testing::Test {
  protected:
    BpfMapTest() {}
    int mMapFd;

    void SetUp() {
        SKIP_IF_BPF_NOT_SUPPORTED;

        EXPECT_EQ(0, setrlimitForTest());
        if (!access(PINNED_MAP_PATH, R_OK)) {
            EXPECT_EQ(0, remove(PINNED_MAP_PATH));
        }
        mMapFd = createMap(BPF_MAP_TYPE_HASH, sizeof(uint32_t), sizeof(uint32_t), TEST_MAP_SIZE,
                           BPF_F_NO_PREALLOC);
        EXPECT_LE(0, mMapFd);
    }

    void TearDown() {
        SKIP_IF_BPF_NOT_SUPPORTED;

        if (!access(PINNED_MAP_PATH, R_OK)) {
            EXPECT_EQ(0, remove(PINNED_MAP_PATH));
        }
        close(mMapFd);
    }

    void checkMapInvalid(BpfMap<uint32_t, uint32_t>& map) {
        EXPECT_FALSE(map.isValid());
        EXPECT_EQ(-1, map.getMap().get());
    }

    void checkMapValid(BpfMap<uint32_t, uint32_t>& map) {
        EXPECT_LE(0, map.getMap().get());
        EXPECT_TRUE(map.isValid());
    }

    void writeToMapAndCheck(BpfMap<uint32_t, uint32_t>& map, uint32_t key, uint32_t value) {
        ASSERT_TRUE(isOk(map.writeValue(key, value, BPF_ANY)));
        uint32_t value_read;
        ASSERT_EQ(0, findMapEntry(map.getMap(), &key, &value_read));
        checkValueAndStatus(value, value_read);
    }

    void checkValueAndStatus(uint32_t refValue, StatusOr<uint32_t> value) {
        ASSERT_TRUE(isOk(value.status()));
        ASSERT_EQ(refValue, value.value());
    }

    void populateMap(uint32_t total, BpfMap<uint32_t, uint32_t>& map) {
        for (uint32_t key = 0; key < total; key++) {
            uint32_t value = key * 10;
            EXPECT_TRUE(isOk(map.writeValue(key, value, BPF_ANY)));
        }
    }

    void expectMapEmpty(BpfMap<uint32_t, uint32_t>& map) {
        auto isEmpty = map.isEmpty();
        ASSERT_TRUE(isOk(isEmpty));
        ASSERT_TRUE(isEmpty.value());
    }
};

TEST_F(BpfMapTest, constructor) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap1;
    checkMapInvalid(testMap1);

    BpfMap<uint32_t, uint32_t> testMap2(mMapFd);
    checkMapValid(testMap2);

    BpfMap<uint32_t, uint32_t> testMap3(BPF_MAP_TYPE_HASH, TEST_MAP_SIZE, BPF_F_NO_PREALLOC);
    checkMapValid(testMap3);
}

TEST_F(BpfMapTest, basicHelpers) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap(mMapFd);
    uint32_t key = TEST_KEY1;
    uint32_t value_write = TEST_VALUE1;
    writeToMapAndCheck(testMap, key, value_write);
    StatusOr<uint32_t> value_read = testMap.readValue(key);
    checkValueAndStatus(value_write, value_read);
    StatusOr<uint32_t> key_read = testMap.getFirstKey();
    checkValueAndStatus(key, key_read);
    ASSERT_TRUE(isOk(testMap.deleteValue(key)));
    ASSERT_GT(0, findMapEntry(testMap.getMap(), &key, &value_read));
    ASSERT_EQ(ENOENT, errno);
}

TEST_F(BpfMapTest, reset) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap;
    testMap.reset(mMapFd);
    uint32_t key = TEST_KEY1;
    uint32_t value_write = TEST_VALUE1;
    writeToMapAndCheck(testMap, key, value_write);
    testMap.reset();
    checkMapInvalid(testMap);
    unique_fd invalidFd(mMapFd);
    ASSERT_GT(0, findMapEntry(invalidFd, &key, &value_write));
    ASSERT_EQ(EBADF, errno);
}

TEST_F(BpfMapTest, moveConstructor) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap1(mMapFd);
    BpfMap<uint32_t, uint32_t> testMap2;
    testMap2 = std::move(testMap1);
    uint32_t key = TEST_KEY1;
    checkMapInvalid(testMap1);
    uint32_t value = TEST_VALUE1;
    writeToMapAndCheck(testMap2, key, value);
}

TEST_F(BpfMapTest, SetUpMap) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    EXPECT_NE(0, access(PINNED_MAP_PATH, R_OK));
    BpfMap<uint32_t, uint32_t> testMap1(BPF_MAP_TYPE_HASH, TEST_MAP_SIZE, BPF_F_NO_PREALLOC);
    ASSERT_EQ(0, bpfFdPin(testMap1.getMap(), PINNED_MAP_PATH));
    EXPECT_EQ(0, access(PINNED_MAP_PATH, R_OK));
    checkMapValid(testMap1);
    BpfMap<uint32_t, uint32_t> testMap2;
    EXPECT_OK(testMap2.init(PINNED_MAP_PATH));
    checkMapValid(testMap2);
    uint32_t key = TEST_KEY1;
    uint32_t value = TEST_VALUE1;
    writeToMapAndCheck(testMap1, key, value);
    StatusOr<uint32_t> value_read = testMap2.readValue(key);
    checkValueAndStatus(value, value_read);
}

TEST_F(BpfMapTest, iterate) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap(mMapFd);
    populateMap(TEST_MAP_SIZE, testMap);
    int totalCount = 0;
    int totalSum = 0;
    const auto iterateWithDeletion = [&totalCount, &totalSum](const uint32_t& key,
                                                              BpfMap<uint32_t, uint32_t>& map) {
        EXPECT_GE((uint32_t)TEST_MAP_SIZE, key);
        totalCount++;
        totalSum += key;
        return map.deleteValue(key);
    };
    EXPECT_OK(testMap.iterate(iterateWithDeletion));
    EXPECT_EQ((int)TEST_MAP_SIZE, totalCount);
    EXPECT_EQ(((1 + TEST_MAP_SIZE - 1) * (TEST_MAP_SIZE - 1)) / 2, (uint32_t)totalSum);
    expectMapEmpty(testMap);
}

TEST_F(BpfMapTest, iterateWithValue) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap(mMapFd);
    populateMap(TEST_MAP_SIZE, testMap);
    int totalCount = 0;
    int totalSum = 0;
    const auto iterateWithDeletion = [&totalCount, &totalSum](const uint32_t& key,
                                                              const uint32_t& value,
                                                              BpfMap<uint32_t, uint32_t>& map) {
        EXPECT_GE((uint32_t)TEST_MAP_SIZE, key);
        EXPECT_EQ(value, key * 10);
        totalCount++;
        totalSum += value;
        return map.deleteValue(key);
    };
    EXPECT_OK(testMap.iterateWithValue(iterateWithDeletion));
    EXPECT_EQ((int)TEST_MAP_SIZE, totalCount);
    EXPECT_EQ(((1 + TEST_MAP_SIZE - 1) * (TEST_MAP_SIZE - 1)) * 5, (uint32_t)totalSum);
    expectMapEmpty(testMap);
}

TEST_F(BpfMapTest, mapIsEmpty) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap(mMapFd);
    expectMapEmpty(testMap);
    uint32_t key = TEST_KEY1;
    uint32_t value_write = TEST_VALUE1;
    writeToMapAndCheck(testMap, key, value_write);
    auto isEmpty = testMap.isEmpty();
    ASSERT_TRUE(isOk(isEmpty));
    ASSERT_FALSE(isEmpty.value());
    ASSERT_TRUE(isOk(testMap.deleteValue(key)));
    ASSERT_GT(0, findMapEntry(testMap.getMap(), &key, &value_write));
    ASSERT_EQ(ENOENT, errno);
    expectMapEmpty(testMap);
    int entriesSeen = 0;
    EXPECT_OK(testMap.iterate(
        [&entriesSeen](const unsigned int&,
                       const BpfMap<unsigned int, unsigned int>&) -> netdutils::Status {
            entriesSeen++;
            return netdutils::status::ok;
        }));
    EXPECT_EQ(0, entriesSeen);
    EXPECT_OK(testMap.iterateWithValue(
        [&entriesSeen](const unsigned int&, const unsigned int&,
                       const BpfMap<unsigned int, unsigned int>&) -> netdutils::Status {
            entriesSeen++;
            return netdutils::status::ok;
        }));
    EXPECT_EQ(0, entriesSeen);
}

TEST_F(BpfMapTest, mapClear) {
    SKIP_IF_BPF_NOT_SUPPORTED;

    BpfMap<uint32_t, uint32_t> testMap(mMapFd);
    populateMap(TEST_MAP_SIZE, testMap);
    auto isEmpty = testMap.isEmpty();
    ASSERT_TRUE(isOk(isEmpty));
    ASSERT_FALSE(isEmpty.value());
    ASSERT_TRUE(isOk(testMap.clear()));
    expectMapEmpty(testMap);
}

}  // namespace bpf
}  // namespace android
