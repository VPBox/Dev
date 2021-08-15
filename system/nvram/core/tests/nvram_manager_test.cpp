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

#if defined(HAS_GTEST)
#include <gtest/gtest.h>
#else
#include "gtest_stubs.h"
#endif

#include <string.h>

#include <nvram/core/nvram_manager.h>
#include <nvram/core/persistence.h>

#include "fake_storage.h"

namespace nvram {
namespace {

class NvramManagerTest : public testing::Test {
 protected:
  NvramManagerTest() {
    storage::Clear();
  }

  static void SetupHeader(uint32_t header_version, uint32_t index) {
    NvramHeader header;
    header.version = header_version;
    ASSERT_TRUE(header.allocated_indices.Resize(1));
    header.allocated_indices[0] = index;
    ASSERT_EQ(storage::Status::kSuccess, persistence::StoreHeader(header));
  }

  static void ReadAndCompareSpaceData(NvramManager* nvram,
                                      uint32_t index,
                                      const void* expected_contents,
                                      size_t expected_size) {
    ReadSpaceRequest read_space_request;
    read_space_request.index = index;
    ReadSpaceResponse read_space_response;
    EXPECT_EQ(NV_RESULT_SUCCESS,
              nvram->ReadSpace(read_space_request, &read_space_response));
    ASSERT_EQ(expected_size, read_space_response.buffer.size());
    EXPECT_EQ(0, memcmp(read_space_response.buffer.data(), expected_contents,
                        expected_size));
  }

  static uint32_t GetControlsMask(const Vector<nvram_control_t>& controls) {
    uint32_t mask = 0;
    for (nvram_control_t control : controls) {
      mask |= (1 << control);
    }
    return mask;
  }
};

TEST_F(NvramManagerTest, Init_FromScratch) {
  NvramManager nvram;

  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response));
}

TEST_F(NvramManagerTest, Init_TrailingStorageBytes) {
  // Set up a pre-existing space and add some trailing bytes.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));
  Blob space_blob;
  ASSERT_EQ(storage::Status::kSuccess, storage::LoadSpace(1, &space_blob));
  ASSERT_TRUE(space_blob.Resize(space_blob.size() + 10));
  ASSERT_EQ(storage::Status::kSuccess, storage::StoreSpace(1, space_blob));

  // Produce a matching header and append some trailing bytes.
  NvramHeader header;
  header.version = NvramHeader::kVersion;
  ASSERT_TRUE(header.allocated_indices.Resize(1));
  header.allocated_indices[0] = 1;
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreHeader(header));
  Blob header_blob;
  ASSERT_EQ(storage::Status::kSuccess, storage::LoadHeader(&header_blob));
  ASSERT_TRUE(header_blob.Resize(header_blob.size() + 10));
  ASSERT_EQ(storage::Status::kSuccess, storage::StoreHeader(header_blob));

  // Initialize the |NvramManager| and check that the header and space blobs get
  // loaded successfully.
  NvramManager nvram;

  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  ASSERT_EQ(1U, get_info_response.space_list.size());
  EXPECT_EQ(1U, get_info_response.space_list[0]);

  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10U, get_space_info_response.size);
}

TEST_F(NvramManagerTest, Init_SpacesPresent) {
  // Set up two pre-existing spaces.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));
  ASSERT_TRUE(space.contents.Resize(20));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(2, space));

  // Indicate 3 present spaces in the header, including one that doesn't have
  // space data in storage.
  NvramHeader header;
  header.version = NvramHeader::kVersion;
  ASSERT_TRUE(header.allocated_indices.Resize(3));
  header.allocated_indices[0] = 1;
  header.allocated_indices[1] = 2;
  header.allocated_indices[2] = 3;
  header.provisional_index.Activate() = 4;
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreHeader(header));

  NvramManager nvram;

  // Check that the spaces are correctly recovered.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10u, get_space_info_response.size);

  get_space_info_request.index = 2;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(20u, get_space_info_response.size);

  get_space_info_request.index = 3;
  EXPECT_EQ(
      NV_RESULT_INTERNAL_ERROR,
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response));

  get_space_info_request.index = 4;
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response));
}

TEST_F(NvramManagerTest, Init_BadSpacePresent) {
  // Set up a good and a bad NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));
  const uint8_t kBadSpaceData[] = {0xba, 0xad};
  Blob bad_space_blob;
  ASSERT_TRUE(bad_space_blob.Assign(kBadSpaceData, sizeof(kBadSpaceData)));
  ASSERT_EQ(storage::Status::kSuccess,
            storage::StoreSpace(2, bad_space_blob));

  NvramHeader header;
  header.version = NvramHeader::kVersion;
  ASSERT_TRUE(header.allocated_indices.Resize(2));
  header.allocated_indices[0] = 1;
  header.allocated_indices[1] = 2;
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreHeader(header));

  NvramManager nvram;

  // The bad index will fail requests.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 2;
  GetSpaceInfoResponse get_space_info_response;
  nvram_result_t result =
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response);
  EXPECT_NE(NV_RESULT_SUCCESS, result);
  EXPECT_NE(NV_RESULT_SPACE_DOES_NOT_EXIST, result);

  // A request to get info for the good index should succeed.
  get_space_info_request.index = 1;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10u, get_space_info_response.size);
}

TEST_F(NvramManagerTest, Init_NewerStorageVersion) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));

  SetupHeader(NvramHeader::kVersion + 1, 1);

  NvramManager nvram;

  // Requests should fail due to version mismatch.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(
      NV_RESULT_INTERNAL_ERROR,
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response));
}

TEST_F(NvramManagerTest, Init_StorageObjectTypeMismatch) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));

  // Copy the space blob to the header storage.
  Blob space_blob;
  ASSERT_EQ(storage::Status::kSuccess, storage::LoadSpace(1, &space_blob));
  ASSERT_EQ(storage::Status::kSuccess, storage::StoreHeader(space_blob));

  NvramManager nvram;

  // Initialization should detect that the header storage object doesn't look
  // like a header, so initialization should fail.
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.GetInfo(get_info_request, &get_info_response));
}

TEST_F(NvramManagerTest, CreateSpace_Success) {
  NvramManager nvram;

  // Make a call to CreateSpace, which should succeed.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 32;
  ASSERT_TRUE(create_space_request.controls.Resize(5));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  create_space_request.controls[1] = NV_CONTROL_BOOT_READ_LOCK;
  create_space_request.controls[2] = NV_CONTROL_WRITE_AUTHORIZATION;
  create_space_request.controls[3] = NV_CONTROL_READ_AUTHORIZATION;
  create_space_request.controls[4] = NV_CONTROL_WRITE_EXTEND;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.CreateSpace(create_space_request, &create_space_response));

  // GetSpaceInfo should reflect the space parameters set during creation.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));

  EXPECT_EQ(32u, get_space_info_response.size);
  EXPECT_EQ(GetControlsMask(create_space_request.controls),
            GetControlsMask(get_space_info_response.controls));
  EXPECT_EQ(false, get_space_info_response.read_locked);
  EXPECT_EQ(false, get_space_info_response.write_locked);
}

TEST_F(NvramManagerTest, CreateSpace_Existing) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));

  SetupHeader(NvramHeader::kVersion, 1);

  NvramManager nvram;

  // A request to create another space with the same index should fail.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_SPACE_ALREADY_EXISTS,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_TooLarge) {
  NvramManager nvram;

  // A request to create a space with a too large content size should fail.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16384;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_AuthTooLarge) {
  NvramManager nvram;

  // A request to create a space with a too large authorization value size
  // should fail.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  ASSERT_TRUE(create_space_request.authorization_value.Resize(256));

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_BadControl) {
  NvramManager nvram;

  // A request to create a space with an unknown control value should fail.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;
  ASSERT_TRUE(create_space_request.controls.Resize(2));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  create_space_request.controls[1] = 17;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_ControlWriteLockExclusive) {
  NvramManager nvram;

  // Spaces may not be created with conflicting write lock modes.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;
  ASSERT_TRUE(create_space_request.controls.Resize(2));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  create_space_request.controls[1] = NV_CONTROL_PERSISTENT_WRITE_LOCK;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_WriteExtendSpaceSize) {
  NvramManager nvram;

  // Write-extend spaces must match SHA256 hash size, i.e. 32 bytes.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;
  ASSERT_TRUE(create_space_request.controls.Resize(1));
  create_space_request.controls[0] = NV_CONTROL_WRITE_EXTEND;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, CreateSpace_HeaderWriteError) {
  // If the header fails to get written to storage, the creation request should
  // fail.
  storage::SetHeaderWriteError(true);

  NvramManager nvram;

  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.CreateSpace(create_space_request, &create_space_response));

  // The space shouldn't be present.
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  EXPECT_EQ(0U, get_info_response.space_list.size());

  // Creation of the space after clearing the error should work.
  storage::SetHeaderWriteError(false);
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.CreateSpace(create_space_request, &create_space_response));

  // The space should be reported as allocated now.
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  ASSERT_EQ(1U, get_info_response.space_list.size());
  EXPECT_EQ(1U, get_info_response.space_list[0]);
}

TEST_F(NvramManagerTest, CreateSpace_SpaceWriteError) {
  storage::SetSpaceWriteError(1, true);
  NvramManager nvram;

  // A request to create another space with the same index should fail.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 16;

  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.CreateSpace(create_space_request, &create_space_response));

  // Reloading the state after a crash should not show any traces of the space.
  storage::SetSpaceWriteError(1, false);
  NvramManager nvram2;

  // The space shouldn't exist in the space list.
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram2.GetInfo(get_info_request, &get_info_response));

  EXPECT_EQ(0U, get_info_response.space_list.size());

  // The space info request should indicate the space doesn't exist.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 1;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram2.GetSpaceInfo(get_space_info_request, &get_space_info_response));
}

TEST_F(NvramManagerTest, DeleteSpace_SpaceAbsent) {
  NvramManager nvram;

  // Attempt to delete a non-existing space.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));
}

TEST_F(NvramManagerTest, DeleteSpace_Success) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(42, space));
  SetupHeader(NvramHeader::kVersion, 42);

  NvramManager nvram;

  // Successful deletion.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));
}

TEST_F(NvramManagerTest, DeleteSpace_AuthorizationFailure) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_WRITE_AUTHORIZATION);
  const char kAuthorizationValue[] = "secret";
  ASSERT_TRUE(space.authorization_value.Assign(kAuthorizationValue,
                                               sizeof(kAuthorizationValue)));
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(42, space));
  SetupHeader(NvramHeader::kVersion, 42);

  NvramManager nvram;

  // Deletion should fail if correct secret is not provided.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));
}

TEST_F(NvramManagerTest, DeleteSpace_HalfDeleted) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(42, space));
  SetupHeader(NvramHeader::kVersion, 42);

  // Hold on to the space data.
  Blob space_data;
  ASSERT_EQ(storage::Status::kSuccess, storage::LoadSpace(42, &space_data));

  NvramManager nvram;

  // Delete the space.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));

  // Put the space data back into place to simulate a half-completed deletion.
  ASSERT_EQ(storage::Status::kSuccess, storage::StoreSpace(42, space_data));

  // The space should remain deleted after re-initialization.
  NvramManager nvram2;

  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 42;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram2.GetSpaceInfo(get_space_info_request, &get_space_info_response));

  // Re-creation of a space with the same index should work.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 42;
  create_space_request.size = 32;
  ASSERT_TRUE(create_space_request.controls.Resize(1));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram2.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, DeleteSpace_SpaceDeleteError) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(42, space));
  SetupHeader(NvramHeader::kVersion, 42);

  // Make space deletion fail.
  storage::SetSpaceWriteError(42, true);

  NvramManager nvram;

  // Attempt to delete the space.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));

  // The space should remain present.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 42;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10U, get_space_info_response.size);

  // Starting up from scratch shouldn't destroy the space either.
  storage::SetSpaceWriteError(42, false);

  NvramManager nvram2;

  GetSpaceInfoResponse get_space_info_response_2;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response_2));
  EXPECT_EQ(10U, get_space_info_response_2.size);
}

TEST_F(NvramManagerTest, DeleteSpace_HeaderWriteError) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(42, space));
  SetupHeader(NvramHeader::kVersion, 42);

  // Header write on deletion will fail.
  storage::SetHeaderWriteError(true);

  NvramManager nvram;

  // Attempt to delete the space.
  DeleteSpaceRequest delete_space_request;
  delete_space_request.index = 42;
  DeleteSpaceResponse delete_space_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.DeleteSpace(delete_space_request, &delete_space_response));

  // The space should remain present.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 42;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10U, get_space_info_response.size);

  // Starting up from scratch shouldn't destroy the space either.
  storage::SetSpaceWriteError(42, false);

  NvramManager nvram2;

  GetSpaceInfoResponse get_space_info_response_2;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response_2));
  EXPECT_EQ(10U, get_space_info_response_2.size);
}

TEST_F(NvramManagerTest, DisableCreate_Success) {
  NvramManager nvram;

  // Issue a successful disable create request.
  DisableCreateRequest disable_create_request;
  DisableCreateResponse disable_create_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.DisableCreate(disable_create_request,
                                                   &disable_create_response));

  // Make sure space creation request fail afterwards.
  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 32;
  ASSERT_TRUE(create_space_request.controls.Resize(1));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram.CreateSpace(create_space_request, &create_space_response));

  // Redundant requests to disable creation are OK.
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.DisableCreate(disable_create_request,
                                                   &disable_create_response));

  // Space creation should remain disabled even after a reboot.
  NvramManager nvram2;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram2.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, DisableCreate_WriteError) {
  // Make header writes fail.
  storage::SetHeaderWriteError(true);

  NvramManager nvram;

  // The disable request should fail.
  DisableCreateRequest disable_create_request;
  DisableCreateResponse disable_create_response;
  EXPECT_EQ(
      NV_RESULT_INTERNAL_ERROR,
      nvram.DisableCreate(disable_create_request, &disable_create_response));

  // We should still be able to create spaces after clearing the error.
  storage::SetHeaderWriteError(false);

  CreateSpaceRequest create_space_request;
  create_space_request.index = 1;
  create_space_request.size = 32;
  ASSERT_TRUE(create_space_request.controls.Resize(1));
  create_space_request.controls[0] = NV_CONTROL_BOOT_WRITE_LOCK;
  CreateSpaceResponse create_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.CreateSpace(create_space_request, &create_space_response));
}

TEST_F(NvramManagerTest, WriteSpace_SpaceAbsent) {
  NvramManager nvram;

  // Attempt to write a non-existing space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("0123456789", 10));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            nvram.WriteSpace(write_space_request, &write_space_response));
}

TEST_F(NvramManagerTest, WriteSpace_Success) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("0123456789", 10));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.WriteSpace(write_space_request,
                                                &write_space_response));

  // Read back the space and compare contents.
  ReadAndCompareSpaceData(&nvram, 17, "0123456789", 10);

  // The data should persist even after a reboot.
  NvramManager nvram2;

  ReadAndCompareSpaceData(&nvram2, 17, "0123456789", 10);
}

TEST_F(NvramManagerTest, WriteSpace_ExcessData) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("0123456789abcdef", 16));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.WriteSpace(write_space_request, &write_space_response));
}

TEST_F(NvramManagerTest, WriteSpace_ShortData) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  memset(space.contents.data(), 'X', space.contents.size());
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("01234", 5));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // Read back the space data and verify that the missing content bytes have
  // been set to 0.
  const uint8_t kExpectedContents[] = {'0', '1', '2', '3', '4', 0, 0, 0, 0, 0};
  ReadAndCompareSpaceData(&nvram, 17, kExpectedContents, 10);
}

TEST_F(NvramManagerTest, WriteSpace_WriteExtend) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_WRITE_EXTEND);
  ASSERT_TRUE(space.contents.Resize(32));
  memset(space.contents.data(), 0, space.contents.size());
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // Read back the space data and verify the hash.
  const uint8_t kExpectedContents[] = {
      0xee, 0x84, 0x52, 0x88, 0xbb, 0x60, 0x7e, 0x02, 0xfd, 0xfb, 0x31,
      0x95, 0x3a, 0x77, 0x23, 0xcf, 0x67, 0xea, 0x6e, 0x2d, 0xd7, 0xdb,
      0x8c, 0xb4, 0xe4, 0xd2, 0xfd, 0xb4, 0x76, 0x7a, 0x67, 0x89,
  };
  ReadAndCompareSpaceData(&nvram, 17, kExpectedContents, 32);
}

TEST_F(NvramManagerTest, WriteSpace_WriteExtendShortSpace) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_WRITE_EXTEND);
  ASSERT_TRUE(space.contents.Resize(16));
  memset(space.contents.data(), 0, space.contents.size());
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // Read back the space data and verify the truncated hash.
  const uint8_t kExpectedContents[] = {
      0x24, 0x2a, 0xbb, 0x36, 0x10, 0x37, 0x92, 0x3f,
      0x7d, 0x7d, 0x92, 0x3a, 0x16, 0x65, 0xd2, 0xa2,
  };
  ReadAndCompareSpaceData(&nvram, 17, kExpectedContents, 16);
}

TEST_F(NvramManagerTest, WriteSpace_WriteExtendLongSpace) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_WRITE_EXTEND);
  ASSERT_TRUE(space.contents.Resize(33));
  memset(space.contents.data(), 'X', space.contents.size());
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Write the space.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // Read back the space data and verify the hash and trailing 0 bytes.
  const uint8_t kExpectedContents[] = {
      0x99, 0xb8, 0x5f, 0xd0, 0xf7, 0x9b, 0x17, 0x2e, 0x0e, 0x58, 0x3d,
      0x3c, 0x9a, 0x29, 0xa3, 0xaf, 0x0a, 0x4c, 0x68, 0x97, 0x72, 0x8c,
      0x0c, 0xa4, 0x37, 0xad, 0x39, 0xf3, 0x8c, 0x6e, 0x64, 0xd7, 0x00,
  };
  ReadAndCompareSpaceData(&nvram, 17, kExpectedContents, 33);
}

TEST_F(NvramManagerTest, WriteSpace_AuthorizationFailure) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_WRITE_AUTHORIZATION);
  ASSERT_TRUE(space.contents.Resize(10));
  const char kAuthorizationValue[] = "secret";
  ASSERT_TRUE(space.authorization_value.Assign(kAuthorizationValue,
                                               sizeof(kAuthorizationValue)));
  ASSERT_TRUE(space.contents.Assign("0123456789", 10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt a write with the wrong authorization value.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // The previous data should remain effective.
  ReadAndCompareSpaceData(&nvram, 17, "0123456789", 10);
}

TEST_F(NvramManagerTest, WriteSpace_WriteError) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Assign("0123456789", 10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  storage::SetSpaceWriteError(17, true);

  // Attempt a write, which should fail.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // The previous data should remain effective.
  ReadAndCompareSpaceData(&nvram, 17, "0123456789", 10);
}

TEST_F(NvramManagerTest, ReadSpace_SpaceAbsent) {
  NvramManager nvram;

  // Attempt a read from a space that doesn't exist.
  ReadSpaceRequest read_space_request;
  read_space_request.index = 17;
  ReadSpaceResponse read_space_response;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            nvram.ReadSpace(read_space_request, &read_space_response));
}

TEST_F(NvramManagerTest, ReadSpace_AuthorizationFailure) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_READ_AUTHORIZATION);
  ASSERT_TRUE(space.contents.Resize(10));
  const char kAuthorizationValue[] = "secret";
  ASSERT_TRUE(space.authorization_value.Assign(kAuthorizationValue,
                                               sizeof(kAuthorizationValue)));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt a read from the space.
  ReadSpaceRequest read_space_request;
  read_space_request.index = 17;
  ReadSpaceResponse read_space_response;
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            nvram.ReadSpace(read_space_request, &read_space_response));
  EXPECT_EQ(0U, read_space_response.buffer.size());
}

TEST_F(NvramManagerTest, LockSpaceWrite_SpaceAbsent) {
  NvramManager nvram;

  // Attempt to lock a space that doesn't exist.
  LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = 17;
  LockSpaceWriteResponse lock_space_write_response;
  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST,
            nvram.LockSpaceWrite(lock_space_write_request,
                                 &lock_space_write_response));
}

TEST_F(NvramManagerTest, LockSpaceWrite_AuthorizationFailure) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_PERSISTENT_WRITE_LOCK) |
                   (1 << NV_CONTROL_WRITE_AUTHORIZATION);
  ASSERT_TRUE(space.contents.Resize(10));
  const char kAuthorizationValue[] = "secret";
  ASSERT_TRUE(space.authorization_value.Assign(kAuthorizationValue,
                                               sizeof(kAuthorizationValue)));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt to lock a space without valid authentication.
  LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = 17;
  LockSpaceWriteResponse lock_space_write_response;
  EXPECT_EQ(NV_RESULT_ACCESS_DENIED,
            nvram.LockSpaceWrite(lock_space_write_request,
                                 &lock_space_write_response));
}

TEST_F(NvramManagerTest, LockSpaceWrite_SuccessPersistent) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_PERSISTENT_WRITE_LOCK);
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Lock the space.
  LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = 17;
  LockSpaceWriteResponse lock_space_write_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.LockSpaceWrite(lock_space_write_request,
                                 &lock_space_write_response));

  // Writing should fail now.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("data", 4));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // The lock should be persistent, so writing should fail after reboot.
  NvramManager nvram2;

  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram2.WriteSpace(write_space_request, &write_space_response));
}

TEST_F(NvramManagerTest, LockSpaceWrite_SuccessBoot) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_BOOT_WRITE_LOCK);
  ASSERT_TRUE(space.contents.Assign("01234567890", 10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Lock the space.
  LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = 17;
  LockSpaceWriteResponse lock_space_write_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.LockSpaceWrite(lock_space_write_request,
                                 &lock_space_write_response));

  // Writing should fail now.
  WriteSpaceRequest write_space_request;
  write_space_request.index = 17;
  ASSERT_TRUE(write_space_request.buffer.Assign("newcontent", 10));
  WriteSpaceResponse write_space_response;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram.WriteSpace(write_space_request, &write_space_response));

  // We configured a per-boot lock, so writing should succeed after reboot.
  NvramManager nvram2;

  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram2.WriteSpace(write_space_request, &write_space_response));
  ReadAndCompareSpaceData(&nvram2, 17, "newcontent", 10);
}

TEST_F(NvramManagerTest, LockSpaceWrite_NotLockable) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt to lock a space without valid authentication.
  LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = 17;
  LockSpaceWriteResponse lock_space_write_response;
  EXPECT_EQ(NV_RESULT_INVALID_PARAMETER,
            nvram.LockSpaceWrite(lock_space_write_request,
                                 &lock_space_write_response));
}

TEST_F(NvramManagerTest, LockSpaceRead_SpaceAbsent) {
  NvramManager nvram;

  // Attempt to lock a non-existing space.
  LockSpaceReadRequest lock_space_read_request;
  lock_space_read_request.index = 17;
  LockSpaceReadResponse lock_space_read_response;
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram.LockSpaceRead(lock_space_read_request, &lock_space_read_response));
}

TEST_F(NvramManagerTest, LockSpaceRead_AuthorizationFailure) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls =
      (1 << NV_CONTROL_BOOT_READ_LOCK) | (1 << NV_CONTROL_READ_AUTHORIZATION);
  ASSERT_TRUE(space.contents.Resize(10));
  const char kAuthorizationValue[] = "secret";
  ASSERT_TRUE(space.authorization_value.Assign(kAuthorizationValue,
                                               sizeof(kAuthorizationValue)));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt to lock a space without valid authorization.
  LockSpaceReadRequest lock_space_read_request;
  lock_space_read_request.index = 17;
  LockSpaceReadResponse lock_space_read_response;
  EXPECT_EQ(
      NV_RESULT_ACCESS_DENIED,
      nvram.LockSpaceRead(lock_space_read_request, &lock_space_read_response));
}

TEST_F(NvramManagerTest, LockSpaceRead_Success) {
  // Set up an NVRAM space.
  NvramSpace space;
  space.controls = (1 << NV_CONTROL_BOOT_READ_LOCK);
  ASSERT_TRUE(space.contents.Assign("0123456789", 10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Lock the space.
  LockSpaceReadRequest lock_space_read_request;
  lock_space_read_request.index = 17;
  LockSpaceReadResponse lock_space_read_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.LockSpaceRead(lock_space_read_request,
                                                   &lock_space_read_response));

  // Read requests should fail now.
  ReadSpaceRequest read_space_request;
  read_space_request.index = 17;
  ReadSpaceResponse read_space_response;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram.ReadSpace(read_space_request, &read_space_response));
  EXPECT_EQ(0U, read_space_response.buffer.size());

  // This is a non-persistent lock, so reads should work again after a reboot.
  NvramManager nvram2;

  ReadAndCompareSpaceData(&nvram2, 17, "0123456789", 10);
}

TEST_F(NvramManagerTest, LockSpaceRead_NotLockable) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Attempt to lock a space without valid authorization.
  LockSpaceReadRequest lock_space_read_request;
  lock_space_read_request.index = 17;
  LockSpaceReadResponse lock_space_read_response;
  EXPECT_EQ(
      NV_RESULT_INVALID_PARAMETER,
      nvram.LockSpaceRead(lock_space_read_request, &lock_space_read_response));
}

TEST_F(NvramManagerTest, WipeStorage_Success) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  // Check that the space is visible.
  NvramManager nvram;
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 17;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10U, get_space_info_response.size);

  // Request a wipe.
  WipeStorageRequest wipe_storage_request;
  WipeStorageResponse wipe_storage_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.WipeStorage(wipe_storage_request, &wipe_storage_response));

  // The space should no longer be declared.
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  EXPECT_EQ(0U, get_info_response.space_list.size());

  // Accessing the space should fail.
  EXPECT_EQ(
      NV_RESULT_SPACE_DOES_NOT_EXIST,
      nvram.GetSpaceInfo(get_space_info_request, &get_space_info_response));
}

TEST_F(NvramManagerTest, WipeStorage_Abort) {
  // Set up two pre-existing spaces and a matching header.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(1, space));
  ASSERT_TRUE(space.contents.Resize(20));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(2, space));
  NvramHeader header;
  header.version = NvramHeader::kVersion;
  ASSERT_TRUE(header.allocated_indices.Resize(2));
  header.allocated_indices[0] = 1;
  header.allocated_indices[1] = 2;
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreHeader(header));

  // Check that the spaces are visible.
  NvramManager nvram;
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  EXPECT_EQ(2U, get_info_response.space_list.size());
  int space_mask = 0;
  for (size_t i = 0; i < get_info_response.space_list.size(); ++i) {
    space_mask |= (1 << get_info_response.space_list[i]);
  }
  EXPECT_EQ(0x6, space_mask);

  // Set things up so the deletion request for the second space fails.
  storage::SetSpaceWriteError(2, true);

  // The wipe request should fail now.
  WipeStorageRequest wipe_storage_request;
  WipeStorageResponse wipe_storage_response;
  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR,
            nvram.WipeStorage(wipe_storage_request, &wipe_storage_response));

  // New wipe attempt with a fresh instance after clearing the error.
  storage::SetSpaceWriteError(2, false);
  NvramManager nvram2;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram2.WipeStorage(wipe_storage_request, &wipe_storage_response));

  // No spaces should remain.
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram2.GetInfo(get_info_request, &get_info_response));
  EXPECT_EQ(0U, get_info_response.space_list.size());
}

TEST_F(NvramManagerTest, WipeStorage_Disable) {
  // Set up an NVRAM space.
  NvramSpace space;
  ASSERT_TRUE(space.contents.Resize(10));
  ASSERT_EQ(storage::Status::kSuccess, persistence::StoreSpace(17, space));
  SetupHeader(NvramHeader::kVersion, 17);

  NvramManager nvram;

  // Disable wiping.
  DisableWipeRequest disable_wipe_request;
  DisableWipeResponse disable_wipe_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.DisableWipe(disable_wipe_request, &disable_wipe_response));

  // A wipe request should fail.
  WipeStorageRequest wipe_storage_request;
  WipeStorageResponse wipe_storage_response;
  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED,
            nvram.WipeStorage(wipe_storage_request, &wipe_storage_response));

  // The space should remain declared.
  GetInfoRequest get_info_request;
  GetInfoResponse get_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS,
            nvram.GetInfo(get_info_request, &get_info_response));
  ASSERT_EQ(1U, get_info_response.space_list.size());
  EXPECT_EQ(17U, get_info_response.space_list[0]);

  // The space data should remain present.
  GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = 17;
  GetSpaceInfoResponse get_space_info_response;
  EXPECT_EQ(NV_RESULT_SUCCESS, nvram.GetSpaceInfo(get_space_info_request,
                                                  &get_space_info_response));
  EXPECT_EQ(10U, get_space_info_response.size);
}

}  // namespace
}  // namespace nvram
