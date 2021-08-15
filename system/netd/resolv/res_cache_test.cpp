/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <netdb.h>

#include <cutils/properties.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "netd_resolv/stats.h"
#include "resolv_cache.h"

constexpr int TEST_NETID = 30;

namespace {

constexpr int EAI_OK = 0;
constexpr char DNS_EVENT_SUBSAMPLING_MAP_FLAG[] =
        "persist.device_config.netd_native.dns_event_subsample_map";

class ScopedCacheCreate {
  public:
    explicit ScopedCacheCreate(unsigned netid, const char* subsampling_map,
                               const char* property = DNS_EVENT_SUBSAMPLING_MAP_FLAG)
        : mStoredNetId(netid), mStoredProperty(property) {
        property_get(property, mStoredMap, "");
        property_set(property, subsampling_map);
        EXPECT_EQ(0, resolv_create_cache_for_net(netid));
    }
    ~ScopedCacheCreate() {
        resolv_delete_cache_for_net(mStoredNetId);
        property_set(mStoredProperty, mStoredMap);
    }

  private:
    unsigned mStoredNetId;
    const char* mStoredProperty;
    char mStoredMap[PROPERTY_VALUE_MAX]{};
};

}  // namespace

TEST(ResolvCacheTest, DnsEventSubsampling) {
    // Test defaults, default flag is "default:1 0:100 7:10" if no experiment flag is set
    {
        ScopedCacheCreate scopedCacheCreate(TEST_NETID, "");
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_NODATA), 10U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_OK), 100U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_BADFLAGS),
                  1U);  // default
        EXPECT_THAT(resolv_cache_dump_subsampling_map(TEST_NETID),
                    testing::UnorderedElementsAreArray({"default:1", "0:100", "7:10"}));
    }
    // Now change the experiment flag to "0:42 default:666"
    {
        ScopedCacheCreate scopedCacheCreate(TEST_NETID, "0:42 default:666");
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_OK), 42U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_NODATA),
                  666U);  // default
        EXPECT_THAT(resolv_cache_dump_subsampling_map(TEST_NETID),
                    testing::UnorderedElementsAreArray({"default:666", "0:42"}));
    }
    // Now change the experiment flag to something illegal
    {
        ScopedCacheCreate scopedCacheCreate(TEST_NETID, "asvaxx");
        // 0(disable log) is the default value if experiment flag is invalid.
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_OK), 0U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_NODATA), 0U);
        EXPECT_TRUE(resolv_cache_dump_subsampling_map(TEST_NETID).empty());
    }
    // Test negative and zero denom
    {
        ScopedCacheCreate scopedCacheCreate(TEST_NETID, "0:-42 default:-666 7:10 10:0");
        // 0(disable log) is the default value if no valid denom is set
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_OK), 0U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_BADFLAGS), 0U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_NODATA), 10U);
        EXPECT_EQ(resolv_cache_get_subsampling_denom(TEST_NETID, EAI_SOCKTYPE), 0U);
        EXPECT_THAT(resolv_cache_dump_subsampling_map(TEST_NETID),
                    testing::UnorderedElementsAreArray({"7:10", "10:0"}));
    }
}
