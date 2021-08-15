//
// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <algorithm>
#include <string>
#include <vector>

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <gtest/gtest.h>
#include <hardware/nvram.h>
#include <openssl/sha.h>

#include "nvram/hal/tests/scoped_nvram_device.h"

namespace {

constexpr uint32_t kTestIndex1 = 0xDEAD0001;
constexpr uint32_t kTestIndex2 = 0xDEAD0002;
constexpr uint32_t kTestIndexNeverExists = 0xDEAD0003;
// Once we run a test that locks writing, that space is burned until reboot.
// This value is the base index from which to dynamically burn spaces.
constexpr uint32_t kTestIndexBurnBase = 0xDEAD0010;
constexpr uint32_t kTestIndexBurnMax = 0xDEAD00FF;
constexpr nvram_control_t kDefaultControls[] = {NV_CONTROL_BOOT_WRITE_LOCK,
                                                NV_CONTROL_BOOT_READ_LOCK};
constexpr char kNoAuth[] = "";
// If using authorization with an index returned by GetNextBurnSpace use this
// as the value so the space can be cleaned up later.
constexpr char kBurnSpaceAuth[] = "hal_test_burn";

// Returns true if |target| contains |value|.
template <typename T>
bool Contains(T value, const std::vector<T>& target) {
  return (std::find(target.begin(), target.end(), value) != target.end());
}

// Returns true if |target| contains all of |values|.
template <typename T>
bool ContainsAll(const std::vector<T>& values,
                 const std::vector<T>& target) {
  return std::all_of(values.begin(), values.end(),
                     [target](T value) { return Contains(value, target); });
}

// Adds a few safety checks so tests don't get hardware into a state where it
// needs factory reset.
class SafeScopedNvramDevice : public nvram::ScopedNvramDevice {
 public:
  nvram_result_t CreateSpace(uint32_t index,
                             uint64_t size_in_bytes,
                             const std::vector<nvram_control_t>& control_list,
                             const std::string& authorization_value) override {
    CHECK(!Contains(NV_CONTROL_PERSISTENT_WRITE_LOCK, control_list))
        << "Do not use NV_CONTROL_PERSISTENT_WRITE_LOCK in tests.";
    CHECK(!Contains(NV_CONTROL_BOOT_WRITE_LOCK, control_list) ||
          !Contains(NV_CONTROL_WRITE_AUTHORIZATION, control_list) ||
          authorization_value == kNoAuth ||
          authorization_value == kBurnSpaceAuth)
        << "Do not lock spaces with unknown authorization values.";
    return nvram::ScopedNvramDevice::CreateSpace(
        index, size_in_bytes, control_list, authorization_value);
  }

  nvram_result_t DisableCreate() override {
    LOG(FATAL) << "Do not use DisableCreate in tests.";
    return NV_RESULT_OPERATION_DISABLED;
  }
};

class ScopedNvramSpace {
 public:
  ScopedNvramSpace(SafeScopedNvramDevice* device, uint32_t index, uint32_t size)
      : ScopedNvramSpace(device,
                         index,
                         size,
                         std::vector<nvram_control_t>(
                             &kDefaultControls[0],
                             &kDefaultControls[arraysize(kDefaultControls)]),
                         kNoAuth) {}

  ScopedNvramSpace(SafeScopedNvramDevice* device,
                   uint32_t index,
                   uint32_t size,
                   const std::vector<nvram_control_t>& control_list)
      : ScopedNvramSpace(device,
                         index,
                         size,
                         control_list,
                         kNoAuth) {}

  ScopedNvramSpace(SafeScopedNvramDevice* device,
                   uint32_t index,
                   uint32_t size,
                   const std::vector<nvram_control_t>& control_list,
                   const std::string& authorization_value)
      : device_(device),
        index_(index),
        authorization_value_(authorization_value) {
    Create(size, control_list);
  }

  ~ScopedNvramSpace() { Delete(); }

 private:
  void Create(uint32_t size,
              const std::vector<nvram_control_t>& control_list) {
    ASSERT_EQ(
        NV_RESULT_SUCCESS,
        device_->CreateSpace(index_, size, control_list, authorization_value_));
  }

  void Delete() {
    ASSERT_EQ(NV_RESULT_SUCCESS,
              device_->DeleteSpace(index_, authorization_value_));
  }

  SafeScopedNvramDevice* device_;
  uint32_t index_;
  std::string authorization_value_;
};

// Remove all unlocked burn spaces. Returns false on failure.
bool CleanBurnSpaces(SafeScopedNvramDevice* device) {
  // Burned spaces will only be available for cleanup after reboot so there's no
  // sense in attempting cleanup more than once.
  static bool cleaned = false;
  if (cleaned) {
    return true;
  }
  bool success = true;
  cleaned = true;
  std::vector<uint32_t> space_index_list;
  if (device->GetSpaceList(&space_index_list) != NV_RESULT_SUCCESS) {
    return false;
  }
  for (uint32_t index : space_index_list) {
    if (index >= kTestIndexBurnBase && index <= kTestIndexBurnMax) {
      int write_lock, read_lock;
      if (device->IsSpaceLocked(index, &write_lock, &read_lock) !=
          NV_RESULT_SUCCESS) {
        success = false;
        continue;
      }
      if (!write_lock) {
        nvram_result_t result = device->DeleteSpace(index, kNoAuth);
        if (result == NV_RESULT_ACCESS_DENIED) {
          result = device->DeleteSpace(index, kBurnSpaceAuth);
        }
        if (result != NV_RESULT_SUCCESS) {
          success = false;
          continue;
        }
      }
    }
  }
  return success;
}

// Returns the next available burn space index. If using authorization, the
// value MUST be kBurnSpaceAuth.
bool GetNextBurnSpace(SafeScopedNvramDevice* device, uint32_t* index) {
  if (!CleanBurnSpaces(device)) {
    return false;
  }
  std::vector<uint32_t> space_index_list;
  if (device->GetSpaceList(&space_index_list) != NV_RESULT_SUCCESS) {
    return false;
  }
  *index = kTestIndexBurnBase;
  while (Contains(*index, space_index_list)) {
    (*index)++;
  }
  if (*index >= kTestIndexBurnMax) {
    return false;
  }
  return true;
}

std::string SHA256HashString(const std::string& input) {
  uint8_t hash[SHA256_DIGEST_LENGTH];
  SHA256(reinterpret_cast<const uint8_t*>(input.data()), input.size(), hash);
  return std::string(reinterpret_cast<const char*>(hash), SHA256_DIGEST_LENGTH);
}

}  // namespace

namespace nvram {

TEST(NVRAMModuleTest, TotalSize) {
  SafeScopedNvramDevice device;
  uint64_t total_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetTotalSizeInBytes(&total_size));
  EXPECT_LE(2048u, total_size);
};

TEST(NVRAMModuleTest, AvailableSize) {
  SafeScopedNvramDevice device;
  uint64_t available_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetAvailableSizeInBytes(&available_size));
  uint64_t total_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetTotalSizeInBytes(&total_size));
  EXPECT_LE(available_size, total_size);
}

TEST(NVRAMModuleTest, MaxSpaceSize) {
  SafeScopedNvramDevice device;
  uint64_t max_space_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetMaxSpaceSizeInBytes(&max_space_size));
  uint64_t total_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetTotalSizeInBytes(&total_size));
  EXPECT_LE(max_space_size, total_size);
  EXPECT_GE(max_space_size, 32u);
}

TEST(NVRAMModuleTest, MaxSpaces) {
  SafeScopedNvramDevice device;
  uint32_t num_spaces = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetMaxSpaces(&num_spaces));
  EXPECT_LE(8u, num_spaces);
}

TEST(NVRAMModuleTest, SpaceList) {
  SafeScopedNvramDevice device;
  uint32_t max_spaces = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetMaxSpaces(&max_spaces));
  std::vector<uint32_t> space_index_list;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetSpaceList(&space_index_list));
  ASSERT_LE(space_index_list.size(), max_spaces);

  // Add a test space and check it gets reported.
  {
    ScopedNvramSpace space(&device, kTestIndex1, 32);
    std::vector<uint32_t> space_index_list2;
    ASSERT_EQ(NV_RESULT_SUCCESS, device.GetSpaceList(&space_index_list2));
    ASSERT_EQ(space_index_list.size() + 1, space_index_list2.size());
    EXPECT_TRUE(ContainsAll(space_index_list, space_index_list2));
    EXPECT_TRUE(Contains(kTestIndex1, space_index_list2));
  }

  // Check we're back to the original list.
  std::vector<uint32_t> space_index_list3;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetSpaceList(&space_index_list3));
  ASSERT_EQ(space_index_list.size(), space_index_list3.size());
  EXPECT_TRUE(ContainsAll(space_index_list, space_index_list3));
  EXPECT_FALSE(Contains(kTestIndex1, space_index_list3));
}

TEST(NVRAMModuleTest, SpaceSize) {
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 17);
  ScopedNvramSpace space2(&device, kTestIndex2, 32);
  uint64_t size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetSpaceSize(kTestIndex1, &size));
  EXPECT_EQ(17u, size);
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetSpaceSize(kTestIndex2, &size));
  EXPECT_EQ(32u, size);
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.GetSpaceSize(kTestIndexNeverExists, &size));
}

TEST(NVRAMModuleTest, SpaceControls) {
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 32);
  std::vector<nvram_control_t> expected_control_list(
      &kDefaultControls[0], &kDefaultControls[arraysize(kDefaultControls)]);
  std::vector<nvram_control_t> control_list;
  ASSERT_EQ(NV_RESULT_SUCCESS,
            device.GetSpaceControls(kTestIndex1, &control_list));
  ASSERT_EQ(expected_control_list.size(), control_list.size());
  EXPECT_TRUE(ContainsAll(expected_control_list, control_list));
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.GetSpaceControls(kTestIndexNeverExists, &control_list));
}

TEST(NVRAMModuleTest, IsLocked) {
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 32);
  int write_lock, read_lock;
  ASSERT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(kTestIndex1, &write_lock, &read_lock));
  EXPECT_FALSE(read_lock);
  EXPECT_FALSE(write_lock);
  ASSERT_EQ(NV_RESULT_SUCCESS, device.EnableReadLock(kTestIndex1, kNoAuth));
  ASSERT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(kTestIndex1, &write_lock, &read_lock));
  EXPECT_TRUE(read_lock);
  EXPECT_FALSE(write_lock);
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      device.IsSpaceLocked(kTestIndexNeverExists, &write_lock, &read_lock));
}

TEST(NVRAMModuleTest, CreateSmall) {
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 1);
}

TEST(NVRAMModuleTest, CreateLarge) {
  SafeScopedNvramDevice device;
  uint64_t max_space_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetMaxSpaceSizeInBytes(&max_space_size));
  uint64_t available_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetAvailableSizeInBytes(&available_size));
  ScopedNvramSpace space(&device, kTestIndex1,
                         std::min(max_space_size, available_size));
}

TEST(NVRAMModuleTest, CreateWithCustomControls) {
  const std::vector<nvram_control_t> kControlList{
      NV_CONTROL_BOOT_WRITE_LOCK, NV_CONTROL_READ_AUTHORIZATION,
      NV_CONTROL_WRITE_EXTEND};
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 32, kControlList);
  std::vector<nvram_control_t> control_list;
  ASSERT_EQ(NV_RESULT_SUCCESS,
            device.GetSpaceControls(kTestIndex1, &control_list));
  ASSERT_EQ(kControlList.size(), control_list.size());
  EXPECT_TRUE(ContainsAll(control_list, kControlList));
  EXPECT_TRUE(ContainsAll(kControlList, control_list));
}

TEST(NVRAMModuleTest, CreateWithAuthorization) {
  SafeScopedNvramDevice device;
  std::string password = "hunter2";
  ScopedNvramSpace space(
      &device, kTestIndex1, 32,
      {NV_CONTROL_WRITE_AUTHORIZATION, NV_CONTROL_READ_AUTHORIZATION},
      password);
  std::string data = "test";
  std::string bad_password = "*******";
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            device.WriteSpace(kTestIndex1, data, bad_password));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(kTestIndex1, data, password));
}

TEST(NVRAMModuleTest, CreateAlreadyExists) {
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, kTestIndex1, 32);
  EXPECT_EQ(NV_RESULT_SPACE_ALREADY_EXISTS,
            device.CreateSpace(kTestIndex1, 32, {}, kNoAuth));
}

TEST(NVRAMModuleTest, Delete) {
  SafeScopedNvramDevice device;
  {
    ScopedNvramSpace space(&device, kTestIndex1, 32);
    uint64_t size = 0;
    EXPECT_EQ(NV_RESULT_SUCCESS, device.GetSpaceSize(kTestIndex1, &size));
  }
  // ScopedNvramSpace will call Delete when it falls out of scope. Now we can
  // make sure that worked.
  uint64_t size = 0;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.GetSpaceSize(kTestIndex1, &size));
}

TEST(NVRAMModuleTest, WriteLock) {
  SafeScopedNvramDevice device;
  uint32_t index;
  ASSERT_TRUE(GetNextBurnSpace(&device, &index));
  ASSERT_EQ(
      NV_RESULT_SUCCESS,
      device.CreateSpace(index, 32, {NV_CONTROL_BOOT_WRITE_LOCK}, kNoAuth));
  int write_lock, read_lock;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(index, &write_lock, &read_lock));
  EXPECT_FALSE(write_lock);
  EXPECT_FALSE(read_lock);
  // It should be possible to delete if the space has not yet been locked.
  ASSERT_EQ(NV_RESULT_SUCCESS, device.DeleteSpace(index, kNoAuth));
  ASSERT_EQ(
      NV_RESULT_SUCCESS,
      device.CreateSpace(index, 32, {NV_CONTROL_BOOT_WRITE_LOCK}, kNoAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test", kNoAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.EnableWriteLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(index, &write_lock, &read_lock));
  EXPECT_TRUE(write_lock);
  EXPECT_FALSE(read_lock);
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            device.WriteSpace(index, "test2", kNoAuth));
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED, device.DeleteSpace(index, kNoAuth));
  std::string data;
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 4, kNoAuth, &data));
  EXPECT_EQ("test", data);
}

TEST(NVRAMModuleTest, ReadLock) {
  uint32_t index = kTestIndex1;
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32, {NV_CONTROL_BOOT_READ_LOCK});
  int write_lock, read_lock;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(index, &write_lock, &read_lock));
  EXPECT_FALSE(write_lock);
  EXPECT_FALSE(read_lock);
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test", kNoAuth));
  std::string data;
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 4, kNoAuth, &data));
  EXPECT_EQ("test", data);
  EXPECT_EQ(NV_RESULT_SUCCESS, device.EnableReadLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS,
            device.IsSpaceLocked(index, &write_lock, &read_lock));
  EXPECT_FALSE(write_lock);
  EXPECT_TRUE(read_lock);
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            device.ReadSpace(index, 4, kNoAuth, &data));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test2", kNoAuth));
}

TEST(NVRAMModuleTest, WriteAuthorization) {
  uint32_t index = kTestIndex1;
  std::string password = "hunter2";
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32, {NV_CONTROL_WRITE_AUTHORIZATION},
                         password);
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test", password));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            device.WriteSpace(index, "test2", kNoAuth));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            device.WriteSpace(index, "test3", "bad_password"));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.DeleteSpace(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.DeleteSpace(index, "bad"));
  std::string data;
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 4, kNoAuth, &data));
  EXPECT_EQ("test", data);
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 4, password, &data));
}

TEST(NVRAMModuleTest, ReadAuthorization) {
  uint32_t index = kTestIndex1;
  std::string password = "hunter2";
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32, {NV_CONTROL_READ_AUTHORIZATION},
                         password);
  ASSERT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test", password));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test2", kNoAuth));
  std::string data;
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 4, password, &data));
  EXPECT_EQ("test", data);
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            device.ReadSpace(index, 4, kNoAuth, &data));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            device.ReadSpace(index, 4, "bad_password", &data));
}

TEST(NVRAMModuleTest, WriteLockAuthorization) {
  SafeScopedNvramDevice device;
  uint32_t index;
  ASSERT_TRUE(GetNextBurnSpace(&device, &index));
  ASSERT_EQ(NV_RESULT_SUCCESS,
            device.CreateSpace(index, 32, {NV_CONTROL_BOOT_WRITE_LOCK,
                                           NV_CONTROL_BOOT_READ_LOCK,
                                           NV_CONTROL_WRITE_AUTHORIZATION},
                               kBurnSpaceAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.EnableReadLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.EnableWriteLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.EnableWriteLock(index, "bad"));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.EnableWriteLock(index, kBurnSpaceAuth));
}

TEST(NVRAMModuleTest, ReadLockAuthorization) {
  uint32_t index = kTestIndex1;
  std::string password = "hunter2";
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32,
                         {NV_CONTROL_BOOT_WRITE_LOCK, NV_CONTROL_BOOT_READ_LOCK,
                          NV_CONTROL_READ_AUTHORIZATION},
                         password);
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.EnableReadLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, device.EnableReadLock(index, "bad"));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.EnableReadLock(index, password));
}

TEST(NVRAMModuleTest, WriteExtend) {
  uint32_t index = kTestIndex1;
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32, {NV_CONTROL_WRITE_EXTEND});
  ASSERT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test", kNoAuth));
  std::string data;
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 32, kNoAuth, &data));
  std::string hash1 = SHA256HashString(std::string(32, 0) + "test");
  EXPECT_EQ(hash1, data);
  EXPECT_EQ(NV_RESULT_SUCCESS, device.WriteSpace(index, "test2", kNoAuth));
  EXPECT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 32, kNoAuth, &data));
  std::string hash2 = SHA256HashString(hash1 + "test2");
  EXPECT_EQ(hash2, data);
}

TEST(NVRAMModuleTest, WriteExtendTooShort) {
  uint32_t index = kTestIndex1;
  SafeScopedNvramDevice device;
    // Only SHA-256 is supported. Try 20 which is SHA-1 output.
  EXPECT_EQ(
      NV_RESULT_INVALID_PARAMETER,
      device.CreateSpace(index, 20, {NV_CONTROL_WRITE_EXTEND}, kNoAuth));
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.WriteSpace(index, "test", kNoAuth));
}

TEST(NVRAMModuleTest, WriteExtendTooLong) {
  uint32_t index = kTestIndex1;
  SafeScopedNvramDevice device;
  uint64_t max_space_size = 0;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.GetMaxSpaceSizeInBytes(&max_space_size));
  if (max_space_size > 32) {
    // Only SHA-256 is supported. Try 64 which is SHA-512 output.
    EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
              device.CreateSpace(index, std::min<uint64_t>(max_space_size, 64),
                                 {NV_CONTROL_WRITE_EXTEND}, kNoAuth));
    EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
              device.WriteSpace(index, "test", kNoAuth));
  }
}

TEST(NVRAMModuleTest, InitialValue) {
  uint32_t index = kTestIndex1;
  SafeScopedNvramDevice device;
  ScopedNvramSpace space(&device, index, 32);
  std::string data;
  ASSERT_EQ(NV_RESULT_SUCCESS, device.ReadSpace(index, 32, kNoAuth, &data));
  EXPECT_EQ(std::string(32, 0), data);
}

TEST(NVRAMModuleTest, ReadWriteSpaceDoesNotExist) {
  uint32_t index = kTestIndexNeverExists;
  SafeScopedNvramDevice device;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.WriteSpace(index, "test", kNoAuth));
  std::string data;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.ReadSpace(index, 1, kNoAuth, &data));
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.EnableWriteLock(index, kNoAuth));
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            device.EnableReadLock(index, kNoAuth));
}

}  // namespace nvram
