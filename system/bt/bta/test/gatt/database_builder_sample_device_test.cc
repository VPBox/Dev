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
/* EXPECT_EQ doesn't work well with static constexpr fields, need a variable
 * with address */
constexpr std::pair<uint16_t, uint16_t> EXPLORE_END =
    DatabaseBuilder::EXPLORE_END;

/* make_pair doesn't work well with EXPECT_EQ, have own helper instead */
inline std::pair<uint16_t, uint16_t> make_pair_u16(uint16_t first,
                                                   uint16_t second) {
  return std::make_pair(first, second);
}

// clang-format off
Uuid SERVICE_1_UUID = Uuid::FromString("00001800-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_2_UUID = Uuid::FromString("00001801-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_3_UUID = Uuid::FromString("0000180f-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_4_UUID = Uuid::FromString("0000fef5-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_UUID = Uuid::FromString("0000180a-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_6_UUID = Uuid::FromString("0000fe55-0000-1000-8000-00805f9b34fb");

Uuid SERVICE_1_CHAR_1_UUID = Uuid::FromString("00002a00-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_1_CHAR_2_UUID = Uuid::FromString("00002a01-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_1_CHAR_3_UUID = Uuid::FromString("00002a04-0000-1000-8000-00805f9b34fb");

Uuid SERVICE_3_CHAR_1_UUID = Uuid::FromString("00002a19-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_3_CHAR_1_DESC_1_UUID = Uuid::FromString("00002902-0000-1000-8000-00805f9b34fb");

Uuid SERVICE_4_CHAR_1_UUID = Uuid::FromString("8082caa8-41a6-4021-91c6-56f9b954cc34");
Uuid SERVICE_4_CHAR_2_UUID = Uuid::FromString("724249f0-5ec3-4b5f-8804-42345af08651");
Uuid SERVICE_4_CHAR_3_UUID = Uuid::FromString("6c53db25-47a1-45fe-a022-7c92fb334fd4");
Uuid SERVICE_4_CHAR_4_UUID = Uuid::FromString("9d84b9a3-000c-49d8-9183-855b673fda31");
Uuid SERVICE_4_CHAR_5_UUID = Uuid::FromString("457871e8-d516-4ca1-9116-57d0b17b9cb2");
Uuid SERVICE_4_CHAR_6_UUID = Uuid::FromString("5f78df94-798c-46f5-990a-b3eb6a065c88");
Uuid SERVICE_4_CHAR_6_DESC_1_UUID = Uuid::FromString("00002902-0000-1000-8000-00805f9b34fb");

Uuid SERVICE_5_CHAR_1_UUID = Uuid::FromString("00002a29-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_2_UUID = Uuid::FromString("00002a24-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_3_UUID = Uuid::FromString("00002a25-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_4_UUID = Uuid::FromString("00002a27-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_5_UUID = Uuid::FromString("00002a26-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_6_UUID = Uuid::FromString("00002a28-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_5_CHAR_7_UUID = Uuid::FromString("00002a50-0000-1000-8000-00805f9b34fb");

Uuid SERVICE_6_CHAR_1_UUID = Uuid::FromString("00000001-1000-1000-8000-00805f9b34fb");
Uuid SERVICE_6_CHAR_1_DESC_1_UUID = Uuid::FromString("00002902-0000-1000-8000-00805f9b34fb");
Uuid SERVICE_6_CHAR_2_UUID = Uuid::FromString("00000002-1000-1000-8000-00805f9b34fb");
Uuid SERVICE_6_CHAR_3_UUID = Uuid::FromString("00000003-1000-1000-8000-00805f9b34fb");
// clang-format on

}  // namespace

// clang-format off
/* Content of sample database, comes from Daydream controller:
Service: handle=0x0001, end_handle=0x0007, uuid=00001800-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x0002, value_handle=0x0003, uuid=00002a00-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0004, value_handle=0x0005, uuid=00002a01-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0006, value_handle=0x0007, uuid=00002a04-0000-1000-8000-00805f9b34fb, prop=0x02
Service: handle=0x0008, end_handle=0x0008, uuid=00001801-0000-1000-8000-00805f9b34fb
Service: handle=0x0009, end_handle=0x000c, uuid=0000180f-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x000a, value_handle=0x000b, uuid=00002a19-0000-1000-8000-00805f9b34fb, prop=0x12
		 Descriptor: handle=0x000c, uuid=00002902-0000-1000-8000-00805f9b34fb
Service: handle=0x000d, end_handle=0x001a, uuid=0000fef5-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x000e, value_handle=0x000f, uuid=8082caa8-41a6-4021-91c6-56f9b954cc34, prop=0x0a
	 Characteristic: declaration_handle=0x0010, value_handle=0x0011, uuid=724249f0-5ec3-4b5f-8804-42345af08651, prop=0x0a
	 Characteristic: declaration_handle=0x0012, value_handle=0x0013, uuid=6c53db25-47a1-45fe-a022-7c92fb334fd4, prop=0x02
	 Characteristic: declaration_handle=0x0014, value_handle=0x0015, uuid=9d84b9a3-000c-49d8-9183-855b673fda31, prop=0x0a
	 Characteristic: declaration_handle=0x0016, value_handle=0x0017, uuid=457871e8-d516-4ca1-9116-57d0b17b9cb2, prop=0x0e
	 Characteristic: declaration_handle=0x0018, value_handle=0x0019, uuid=5f78df94-798c-46f5-990a-b3eb6a065c88, prop=0x12
		 Descriptor: handle=0x001a, uuid=00002902-0000-1000-8000-00805f9b34fb
Service: handle=0x001b, end_handle=0x0029, uuid=0000180a-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x001c, value_handle=0x001d, uuid=00002a29-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x001e, value_handle=0x001f, uuid=00002a24-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0020, value_handle=0x0021, uuid=00002a25-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0022, value_handle=0x0023, uuid=00002a27-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0024, value_handle=0x0025, uuid=00002a26-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0026, value_handle=0x0027, uuid=00002a28-0000-1000-8000-00805f9b34fb, prop=0x02
	 Characteristic: declaration_handle=0x0028, value_handle=0x0029, uuid=00002a50-0000-1000-8000-00805f9b34fb, prop=0x02
Service: handle=0x002a, end_handle=0x0031, uuid=0000fe55-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x002b, value_handle=0x002c, uuid=00000001-1000-1000-8000-00805f9b34fb, prop=0x10
		 Descriptor: handle=0x002d, uuid=00002902-0000-1000-8000-00805f9b34fb
	 Characteristic: declaration_handle=0x002e, value_handle=0x002f, uuid=00000002-1000-1000-8000-00805f9b34fb, prop=0x08
	 Characteristic: declaration_handle=0x0030, value_handle=0x0031, uuid=00000003-1000-1000-8000-00805f9b34fb, prop=0x02
*/
// clang-format on

/* This test verifies that DatabaseBuilder will properly discover database
 * content from a remote device. It also verify that after the discovery is
 * done, returned database is equal to the discovered one */
TEST(DatabaseBuilderSampleDeviceTest, DoDiscovery) {
  DatabaseBuilder builder;

  EXPECT_FALSE(builder.InProgress());

  // At start of discovery, builder will receive All services in order from
  // lower layers.
  builder.AddService(0x0001, 0x0007, SERVICE_1_UUID, true);

  // The moment we receive first service, we are in progress
  // TODO: we should be able to set InProgress state once we sent GATT request,
  // not when it's back and parsed
  EXPECT_TRUE(builder.InProgress());

  builder.AddService(0x0008, 0x0008, SERVICE_2_UUID, true);
  builder.AddService(0x0009, 0x000c, SERVICE_3_UUID, true);
  builder.AddService(0x000d, 0x001a, SERVICE_4_UUID, true);
  builder.AddService(0x001b, 0x0029, SERVICE_5_UUID, true);
  builder.AddService(0x002a, 0x0031, SERVICE_6_UUID, true);

  // At this moment, all services are received, stack will grab them one and one
  // to discover their content.
  EXPECT_TRUE(builder.StartNextServiceExploration());

  // Grabbing first service, to start Included Service and Characteristic
  // discovery
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0001, 0x0007));

  builder.AddCharacteristic(0x0002, 0x0003, SERVICE_1_CHAR_1_UUID, 0x02);
  builder.AddCharacteristic(0x0004, 0x0005, SERVICE_1_CHAR_2_UUID, 0x02);
  builder.AddCharacteristic(0x0006, 0x0007, SERVICE_1_CHAR_3_UUID, 0x02);

  // All characteristics were discovered, stack will try to look for
  // descriptors. Since there is no space for descriptors, builder should return
  // nothing more to discover
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(), EXPLORE_END);

  // Service with handles 0x0008, 0x0008 is skipped for exploration - we know
  // it's empty.
  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x0009, 0x000c));

  builder.AddCharacteristic(0x000a, 0x000b, SERVICE_3_CHAR_1_UUID, 0x12);

  EXPECT_EQ(builder.NextDescriptorRangeToExplore(),
            make_pair_u16(0x000c, 0x000c));

  builder.AddDescriptor(0x000c, SERVICE_3_CHAR_1_DESC_1_UUID);

  // All descriptors were explored
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(), EXPLORE_END);

  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x000d, 0x001a));

  builder.AddCharacteristic(0x000e, 0x000f, SERVICE_4_CHAR_1_UUID, 0x0a);
  builder.AddCharacteristic(0x0010, 0x0011, SERVICE_4_CHAR_2_UUID, 0x0a);
  builder.AddCharacteristic(0x0012, 0x0013, SERVICE_4_CHAR_3_UUID, 0x02);
  builder.AddCharacteristic(0x0014, 0x0015, SERVICE_4_CHAR_4_UUID, 0x0a);
  builder.AddCharacteristic(0x0016, 0x0017, SERVICE_4_CHAR_5_UUID, 0x0e);
  builder.AddCharacteristic(0x0018, 0x0019, SERVICE_4_CHAR_6_UUID, 0x12);

  // Just last Characteristic have space for descriptor
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(),
            make_pair_u16(0x001a, 0x001a));

  builder.AddDescriptor(0x001a, SERVICE_4_CHAR_6_DESC_1_UUID);

  // All descriptors were explored
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(), EXPLORE_END);

  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x001b, 0x0029));

  builder.AddCharacteristic(0x001c, 0x001d, SERVICE_5_CHAR_1_UUID, 0x02);
  builder.AddCharacteristic(0x001e, 0x001f, SERVICE_5_CHAR_2_UUID, 0x02);
  builder.AddCharacteristic(0x0020, 0x0021, SERVICE_5_CHAR_3_UUID, 0x02);
  builder.AddCharacteristic(0x0022, 0x0023, SERVICE_5_CHAR_4_UUID, 0x02);
  builder.AddCharacteristic(0x0024, 0x0025, SERVICE_5_CHAR_5_UUID, 0x02);
  builder.AddCharacteristic(0x0026, 0x0027, SERVICE_5_CHAR_6_UUID, 0x02);
  builder.AddCharacteristic(0x0028, 0x0029, SERVICE_5_CHAR_7_UUID, 0x02);

  // No space for descriptors
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(), EXPLORE_END);

  EXPECT_TRUE(builder.StartNextServiceExploration());
  EXPECT_EQ(builder.CurrentlyExploredService(), make_pair_u16(0x002a, 0x0031));

  builder.AddCharacteristic(0x002b, 0x002c, SERVICE_6_CHAR_1_UUID, 0x10);
  builder.AddCharacteristic(0x002e, 0x002f, SERVICE_6_CHAR_2_UUID, 0x08);
  builder.AddCharacteristic(0x0030, 0x0031, SERVICE_6_CHAR_3_UUID, 0x02);

  // Just one Characteristic have space for descriptor
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(),
            make_pair_u16(0x002d, 0x002d));

  builder.AddDescriptor(0x002d, SERVICE_6_CHAR_1_DESC_1_UUID);

  // All descriptors were explored
  EXPECT_EQ(builder.NextDescriptorRangeToExplore(), EXPLORE_END);

  EXPECT_FALSE(builder.StartNextServiceExploration());

  EXPECT_TRUE(builder.InProgress());
  Database result = builder.Build();
  EXPECT_FALSE(builder.InProgress());

  // verify that the returned database matches what was discovered
  auto service = result.Services().begin();
  EXPECT_EQ(service->handle, 0x0001);
  EXPECT_EQ(service->uuid, SERVICE_1_UUID);

  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_1_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[1].uuid, SERVICE_1_CHAR_2_UUID);
  EXPECT_EQ(service->characteristics[2].uuid, SERVICE_1_CHAR_3_UUID);

  service++;
  EXPECT_EQ(service->uuid, SERVICE_2_UUID);

  service++;
  EXPECT_EQ(service->uuid, SERVICE_3_UUID);
  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_3_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[0].descriptors[0].uuid,
            SERVICE_3_CHAR_1_DESC_1_UUID);

  service++;
  EXPECT_EQ(service->uuid, SERVICE_4_UUID);
  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_4_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[1].uuid, SERVICE_4_CHAR_2_UUID);
  EXPECT_EQ(service->characteristics[2].uuid, SERVICE_4_CHAR_3_UUID);
  EXPECT_EQ(service->characteristics[3].uuid, SERVICE_4_CHAR_4_UUID);
  EXPECT_EQ(service->characteristics[4].uuid, SERVICE_4_CHAR_5_UUID);
  EXPECT_EQ(service->characteristics[5].uuid, SERVICE_4_CHAR_6_UUID);
  EXPECT_EQ(service->characteristics[5].descriptors[0].uuid,
            SERVICE_4_CHAR_6_DESC_1_UUID);

  service++;
  EXPECT_EQ(service->uuid, SERVICE_5_UUID);
  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_5_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[1].uuid, SERVICE_5_CHAR_2_UUID);
  EXPECT_EQ(service->characteristics[2].uuid, SERVICE_5_CHAR_3_UUID);
  EXPECT_EQ(service->characteristics[3].uuid, SERVICE_5_CHAR_4_UUID);
  EXPECT_EQ(service->characteristics[4].uuid, SERVICE_5_CHAR_5_UUID);
  EXPECT_EQ(service->characteristics[5].uuid, SERVICE_5_CHAR_6_UUID);
  EXPECT_EQ(service->characteristics[6].uuid, SERVICE_5_CHAR_7_UUID);

  service++;
  EXPECT_EQ(service->uuid, SERVICE_6_UUID);
  EXPECT_EQ(service->characteristics[0].uuid, SERVICE_6_CHAR_1_UUID);
  EXPECT_EQ(service->characteristics[0].descriptors[0].uuid,
            SERVICE_6_CHAR_1_DESC_1_UUID);
  EXPECT_EQ(service->characteristics[1].uuid, SERVICE_6_CHAR_2_UUID);
  EXPECT_EQ(service->characteristics[2].uuid, SERVICE_6_CHAR_3_UUID);
}

}  // namespace gatt