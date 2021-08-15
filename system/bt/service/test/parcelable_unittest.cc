//
//  Copyright 2016 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include <gtest/gtest.h>

#include "service/common/android/bluetooth/advertise_data.h"
#include "service/common/android/bluetooth/advertise_settings.h"
#include "service/common/android/bluetooth/bluetooth_gatt_characteristic.h"
#include "service/common/android/bluetooth/bluetooth_gatt_descriptor.h"
#include "service/common/android/bluetooth/bluetooth_gatt_service.h"
#include "service/common/android/bluetooth/scan_filter.h"
#include "service/common/android/bluetooth/scan_result.h"
#include "service/common/android/bluetooth/scan_settings.h"
#include "service/common/android/bluetooth/uuid.h"
#include "service/common/bluetooth/low_energy_constants.h"

using android::Parcel;

using bluetooth::AdvertiseData;
using bluetooth::AdvertiseSettings;
using bluetooth::Characteristic;
using bluetooth::Descriptor;
using bluetooth::ScanFilter;
using bluetooth::ScanResult;
using bluetooth::ScanSettings;
using bluetooth::Service;
using bluetooth::Uuid;

namespace bluetooth {
namespace {

template <class IN, class OUT>
bool TestData(IN& in) {
  Parcel parcel;

  parcel.writeParcelable((OUT)in);
  parcel.setDataPosition(0);
  OUT out;
  parcel.readParcelable(&out);
  // in case of error this display nice log message
  EXPECT_EQ(out, in);
  return in == out;
}

TEST(ParcelableTest, NonEmptyAdvertiseData) {
  std::vector<uint8_t> data{0x02, 0x02, 0x00};
  AdvertiseData adv0(data);
  bool result =
      TestData<AdvertiseData, android::bluetooth::AdvertiseData>(adv0);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, DefaultAdvertiseSettings) {
  AdvertiseSettings settings;
  bool result =
      TestData<AdvertiseSettings, android::bluetooth::AdvertiseSettings>(
          settings);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, NonEmptyAdvertiseSettings) {
  AdvertiseSettings settings(
      AdvertiseSettings::MODE_BALANCED, base::TimeDelta::FromMilliseconds(150),
      AdvertiseSettings::TX_POWER_LEVEL_HIGH, false /* connectable */);

  bool result =
      TestData<AdvertiseSettings, android::bluetooth::AdvertiseSettings>(
          settings);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, Uuid) {
  // Try a whole bunch of Uuids.
  for (int i = 0; i < 10; i++) {
    Uuid uuid = Uuid::GetRandom();
    TestData<Uuid, android::bluetooth::UUID>(uuid);
  }
}

TEST(ParcelableTest, ScanSettings) {
  ScanSettings settings0;
  ScanSettings settings1(
      ScanSettings::MODE_BALANCED, ScanSettings::CALLBACK_TYPE_FIRST_MATCH,
      ScanSettings::RESULT_TYPE_ABBREVIATED,
      base::TimeDelta::FromMilliseconds(150), ScanSettings::MATCH_MODE_STICKY,
      ScanSettings::MATCH_COUNT_FEW_ADVERTISEMENTS);

  bool result =
      TestData<ScanSettings, android::bluetooth::ScanSettings>(settings0);
  EXPECT_TRUE(result);

  result = TestData<ScanSettings, android::bluetooth::ScanSettings>(settings0);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, ScanFilter) {
  ScanFilter filter;

  filter.set_device_name("Test Device Name");
  ASSERT_TRUE(filter.SetDeviceAddress("01:02:04:AB:CD:EF"));

  bool result = TestData<ScanFilter, android::bluetooth::ScanFilter>(filter);
  EXPECT_TRUE(result);

  Uuid uuid = Uuid::GetRandom();
  filter.SetServiceUuid(uuid);

  result = TestData<ScanFilter, android::bluetooth::ScanFilter>(filter);
  EXPECT_TRUE(result);

  Uuid mask = Uuid::GetRandom();
  filter.SetServiceUuidWithMask(uuid, mask);
  result = TestData<ScanFilter, android::bluetooth::ScanFilter>(filter);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, ScanResult) {
  const char kTestAddress[] = "01:02:03:AB:CD:EF";

  const std::vector<uint8_t> kEmptyBytes;
  const std::vector<uint8_t> kTestBytes{0x01, 0x02, 0x03};

  const int kTestRssi = 127;

  ScanResult result0(kTestAddress, kEmptyBytes, kTestRssi);
  ScanResult result1(kTestAddress, kTestBytes, kTestRssi);

  bool result = TestData<ScanResult, android::bluetooth::ScanResult>(result0);
  EXPECT_TRUE(result);

  result = TestData<ScanResult, android::bluetooth::ScanResult>(result1);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, GattDescriptor) {
  Descriptor s = Descriptor(0x0000, Uuid::GetRandom(),
                            bluetooth::kAttributePermissionRead);
  Descriptor s2 = Descriptor(0xFFFE, Uuid::GetRandom(),
                             bluetooth::kAttributePermissionWrite);
  Descriptor s3 = Descriptor(0x003D, Uuid::GetRandom(),
                             bluetooth::kAttributePermissionReadEncryptedMITM |
                                 bluetooth::kAttributePermissionRead);

  bool result =
      TestData<Descriptor, android::bluetooth::BluetoothGattDescriptor>(s);
  EXPECT_TRUE(result);

  result =
      TestData<Descriptor, android::bluetooth::BluetoothGattDescriptor>(s2);
  EXPECT_TRUE(result);

  result =
      TestData<Descriptor, android::bluetooth::BluetoothGattDescriptor>(s3);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, GattCharacteristic) {
  Characteristic c = Characteristic(0x0004, Uuid::GetRandom(), 0, 0,
                                    {Descriptor(0x0005, Uuid::GetRandom(), 0),
                                     Descriptor(0x0007, Uuid::GetRandom(), 0),
                                     Descriptor(0x00A1, Uuid::GetRandom(), 0)});

  bool result =
      TestData<Characteristic, android::bluetooth::BluetoothGattCharacteristic>(
          c);
  EXPECT_TRUE(result);
}

TEST(ParcelableTest, GattService) {
  Service s =
      Service(0x0001, true, Uuid::FromString("CAFE", nullptr),
              {Characteristic(0x0004, Uuid::GetRandom(),
                              bluetooth::kCharacteristicPropertyNotify,
                              bluetooth::kAttributePermissionRead,
                              {Descriptor(0x0005, Uuid::GetRandom(), 0),
                               Descriptor(0x0007, Uuid::GetRandom(), 0),
                               Descriptor(0x0009, Uuid::GetRandom(), 0)}),
               Characteristic(0x000D, Uuid::GetRandom(),
                              bluetooth::kCharacteristicPropertyWrite,
                              bluetooth::kAttributePermissionWrite,
                              {Descriptor(0x0010, Uuid::GetRandom(), 0),
                               Descriptor(0x0012, Uuid::GetRandom(), 0)}),
               Characteristic(0x0015, Uuid::GetRandom(), 0, 0, {})},
              {});

  Parcel parcel;

  parcel.writeParcelable((android::bluetooth::BluetoothGattService)s);
  parcel.setDataPosition(0);
  android::bluetooth::BluetoothGattService out;
  parcel.readParcelable(&out);

  bool result = TestData<Service, android::bluetooth::BluetoothGattService>(s);
  EXPECT_TRUE(result);
}

}  // namespace
}  // namespace bluetooth
