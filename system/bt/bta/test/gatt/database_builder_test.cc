/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#include <gtest/gtest.h>

#include <base/logging.h>
#include <iterator>
#include <utility>

#include "gatt/database_builder.h"

using bluetooth::Uuid;

namespace gatt {

namespace {
/* make_pair doesn't work well with EXPECT_EQ, have own helper instead */
inline std::pair<uint16_t, uint16_t> make_pair_u16(uint16_t first,
                                                   uint16_t second) {
  return std::make_pair(first, second);
}

Uuid SERVICE_1_UUID = Uuid::FromString("00001800-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_2_UUID = Uuid::FromString("00001801-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_3_UUID = Uuid::FromString("0000180f-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_4_UUID = Uuid::FromString("0000fef5-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_UUID = Uuid::FromString("0000180a-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_1_CHAR_1_UUID =
    Uuid::FromString("00002a00-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_1_CHAR_1_DESC_1_UUID =
    Uuid::FromString("00002902-0000-1000-8000-00805f9b34fb");

}  // namespace

/* Verify adding empty service works ok */
TEST(DatabaseBuilderTest, EmptyServiceAddTest) {
  DatabaseBuilder builder;

  EXPECT_FALSE(builder.InProgress());

  // Simple database, just one empty
  builder.AddService(0x0001, 0x0001, SERVICE_1_UUID, true);
  EXPECT_FALSE(builder.StartNextServiceExploration());

  Database result = builder.Build();

  // verify that the returned database matches what was discovered
  auto service = result.Services().begin();
  EXPECT_EQ(service->handle, 0x0001);
  EXPECT_EQ(service->end_handle, 0x0001);
  EXPECT_EQ(service->is_primary, true);
  EXPECT_EQ(service->uuid, SERVICE_1_UUID);
}

/* Verify adding service, characteristic and descriptor work */
TEST(DatabaseBuilderTest, DescriptorAddTest) {
  DatabaseBuilder builder;

  EXPECT_FALSE(builder.InProgress());

  // Simple database, just one empty
  builder.AddService(0x0001, 0x000f, SERVICE_1_UUID, true);
  builder.AddCharacteristic(0x0002, 0x0003, SERVICE_1_CHAR_1_UUID, 0x02);
  builder.AddDescriptor(0x0004, SERVICE_1_CHAR_1_DESC_1_UUID);

  Database result = builder.Build();

  // verify that the returned database matches what was discovered
  auto service = result.Services().begin();
  EXPECT_EQ(service->handle, 0x0001);
  EXPECT_EQ(service->end_handle, 0x000f);
  EXPECT_EQ(service->is_primary, true);
  EXPECT_EQ(service->uuid, SERVICE_1_UUID);

  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_1_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[0].declaration_handle, 0x0002);
  EXPECT_EQ(service->characteristics[0].value_handle, 0x0003);
  EXPECT_EQ(service->characteristics[0].properties, 0x02);

  EXPECT_EQ(service->characteristics[0].descriptors[0].uuid,
            SERVICE_1_CHAR_1_DESC_1_UUID);
  EXPECT_EQ(service->characteristics[0].descriptors[0].handle, 0x0004);
}

/* This test verifies that DatabaseBuilder properly handle discovery of
 * secondary service, that is added to the discovery queue from included service
 * definition. Such service might come out of order.  */
TEST(DatabaseBuilderTest, SecondaryServiceOutOfOrderTest) {
  DatabaseBuilder builder;

  EXPECT_FALSE(builder.InProgress());

  // At start of discovery, builder will receive All services in order from
  // lower layers.
  builder.AddService(0x0001, 0x000f, SERVICE_1_UUID, true);
  builder.AddService(0x0030, 0x003f, SERVICE_3_UUID, true);
  builder.AddService(0x0050, 0x005f, SERVICE_5_UUID, true);

  // First service skipped, no place for handles
  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0001, 0x000f));

  // For this test, content of first service is irrevelant

  EXPECT_TRUE(builder.StartNextServiceExploration());
  // Grabbing first service, to start Included Service and Characteristic
  // discovery
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0030, 0x003f));

  builder.AddIncludedService(0x0031, SERVICE_4_UUID, 0x0040, 0x004f);
  builder.AddIncludedService(0x0032, SERVICE_2_UUID, 0x0020, 0x002f);

  /* Secondary service exploration */
  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0020, 0x002f));

  /* Secondary service exploration */
  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0040, 0x004f));

  /* Back to primary service exploration */
  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0050, 0x005f));

  Database result = builder.Build();

  // verify that the returned database matches what was discovered
  auto service = result.Services().begin();
  EXPECT_EQ(service->handle, 0x0001);
  EXPECT_EQ(service->is_primary, true);
  EXPECT_EQ(service->uuid, SERVICE_1_UUID);

  service++;
  EXPECT_EQ(service->handle, 0x0020);
  EXPECT_EQ(service->end_handle, 0x002f);
  EXPECT_EQ(service->uuid, SERVICE_2_UUID);
  EXPECT_EQ(service->is_primary, false);

  service++;
  EXPECT_EQ(service->handle, 0x0030);
  EXPECT_EQ(service->end_handle, 0x003f);
  EXPECT_EQ(service->uuid, SERVICE_3_UUID);
  EXPECT_EQ(service->is_primary, true);

  service++;
  EXPECT_EQ(service->handle, 0x0040);
  EXPECT_EQ(service->uuid, SERVICE_4_UUID);
  EXPECT_EQ(service->is_primary, false);

  service++;
  EXPECT_EQ(service->handle, 0x0050);
  EXPECT_EQ(service->uuid, SERVICE_5_UUID);
  EXPECT_EQ(service->is_primary, true);

  service++;
  ASSERT_EQ(service, result.Services().end());
}

}  // namespace gatt