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

#include <stdlib.h>
#include <string.h>

#include <gtest/gtest.h>

#include <nvram/messages/nvram_messages.h>

namespace nvram {

namespace {

template <typename Message>
void EncodeAndDecode(const Message& in, Message* out) {
  Blob blob;
  ASSERT_TRUE(Encode(in, &blob));
  ASSERT_TRUE(Decode(blob.data(), blob.size(), out));
}

}  // namespace

TEST(NvramMessagesTest, GetInfoRequest) {
  Request request;
  request.payload.Activate<COMMAND_GET_INFO>();

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_GET_INFO, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_GET_INFO>());
}

TEST(NvramMessagesTest, GetInfoResponse) {
  Response response;
  response.result = NV_RESULT_SUCCESS;
  response.payload.Activate<COMMAND_GET_INFO>();

  GetInfoResponse& response_payload =
      response.payload.Activate<COMMAND_GET_INFO>();
  response_payload.total_size = 32768;
  response_payload.available_size = 4096;
  response_payload.max_space_size = 512;
  response_payload.max_spaces = 32;
  ASSERT_TRUE(response_payload.space_list.Append(0x1234));
  ASSERT_TRUE(response_payload.space_list.Append(0xffffffff));
  ASSERT_TRUE(response_payload.space_list.Append(0x32));
  ASSERT_TRUE(response_payload.space_list.Append(0x0));
  ASSERT_TRUE(response_payload.space_list.Append(0x1));

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SUCCESS, response.result);
  EXPECT_EQ(COMMAND_GET_INFO, decoded.payload.which());
  const GetInfoResponse* decoded_payload =
      decoded.payload.get<COMMAND_GET_INFO>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(32768ULL, decoded_payload->total_size);
  EXPECT_EQ(4096ULL, decoded_payload->available_size);
  EXPECT_EQ(512ULL, decoded_payload->max_space_size);
  EXPECT_EQ(32U, decoded_payload->max_spaces);
  ASSERT_EQ(5U, decoded_payload->space_list.size());
  EXPECT_EQ(0x1234U, decoded_payload->space_list[0]);
  EXPECT_EQ(0xffffffffU, decoded_payload->space_list[1]);
  EXPECT_EQ(0x32U, decoded_payload->space_list[2]);
  EXPECT_EQ(0x0U, decoded_payload->space_list[3]);
  EXPECT_EQ(0x1U, decoded_payload->space_list[4]);
}

TEST(NvramMessagesTest, CreateSpaceRequest) {
  Request request;
  CreateSpaceRequest& request_payload =
      request.payload.Activate<COMMAND_CREATE_SPACE>();
  request_payload.index = 0x12345678;
  request_payload.size = 8;
  ASSERT_TRUE(request_payload.controls.Append(NV_CONTROL_BOOT_WRITE_LOCK));
  ASSERT_TRUE(request_payload.controls.Append(NV_CONTROL_WRITE_EXTEND));
  const uint8_t kAuthValue[] = {1, 2, 3, 4, 5};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_CREATE_SPACE, decoded.payload.which());
  const CreateSpaceRequest* decoded_payload =
      decoded.payload.get<COMMAND_CREATE_SPACE>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x12345678U, decoded_payload->index);
  EXPECT_EQ(8ULL, decoded_payload->size);
  ASSERT_EQ(2UL, decoded_payload->controls.size());
  EXPECT_EQ(NV_CONTROL_BOOT_WRITE_LOCK, decoded_payload->controls[0]);
  EXPECT_EQ(NV_CONTROL_WRITE_EXTEND, decoded_payload->controls[1]);
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, CreateSpaceResponse) {
  Response response;
  response.result = NV_RESULT_SPACE_ALREADY_EXISTS;
  response.payload.Activate<COMMAND_CREATE_SPACE>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SPACE_ALREADY_EXISTS, response.result);
  EXPECT_EQ(COMMAND_CREATE_SPACE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_CREATE_SPACE>());
}

TEST(NvramMessagesTest, GetSpaceInfoRequest) {
  Request request;
  GetSpaceInfoRequest& request_payload =
      request.payload.Activate<COMMAND_GET_SPACE_INFO>();
  request_payload.index = 0x1234;

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_GET_SPACE_INFO, decoded.payload.which());
  const GetSpaceInfoRequest* decoded_payload =
      decoded.payload.get<COMMAND_GET_SPACE_INFO>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
}

TEST(NvramMessagesTest, GetSpaceInfoResponse) {
  Response response;
  response.result = NV_RESULT_SPACE_DOES_NOT_EXIST;
  GetSpaceInfoResponse& response_payload =
      response.payload.Activate<COMMAND_GET_SPACE_INFO>();
  response_payload.size = 256;
  ASSERT_TRUE(response_payload.controls.Append(NV_CONTROL_BOOT_WRITE_LOCK));
  ASSERT_TRUE(response_payload.controls.Append(NV_CONTROL_BOOT_READ_LOCK));
  ASSERT_TRUE(response_payload.controls.Append(NV_CONTROL_WRITE_EXTEND));
  response_payload.read_locked = false;
  response_payload.write_locked = true;

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SPACE_DOES_NOT_EXIST, response.result);
  EXPECT_EQ(COMMAND_GET_SPACE_INFO, decoded.payload.which());
  const GetSpaceInfoResponse* decoded_payload =
      decoded.payload.get<COMMAND_GET_SPACE_INFO>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(256ULL, decoded_payload->size);
  ASSERT_EQ(3U, decoded_payload->controls.size());
  EXPECT_EQ(NV_CONTROL_BOOT_WRITE_LOCK, decoded_payload->controls[0]);
  EXPECT_EQ(NV_CONTROL_BOOT_READ_LOCK, decoded_payload->controls[1]);
  EXPECT_EQ(NV_CONTROL_WRITE_EXTEND, decoded_payload->controls[2]);
  EXPECT_FALSE(decoded_payload->read_locked);
  EXPECT_TRUE(decoded_payload->write_locked);
}

TEST(NvramMessagesTest, DeleteSpaceRequest) {
  Request request;
  DeleteSpaceRequest& request_payload =
      request.payload.Activate<COMMAND_DELETE_SPACE>();
  request_payload.index = 0x1234;
  const uint8_t kAuthValue[] = {1, 2, 3};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_DELETE_SPACE, decoded.payload.which());
  const DeleteSpaceRequest* decoded_payload =
      decoded.payload.get<COMMAND_DELETE_SPACE>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, DeleteSpaceResponse) {
  Response response;
  response.result = NV_RESULT_ACCESS_DENIED;
  response.payload.Activate<COMMAND_DELETE_SPACE>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_ACCESS_DENIED, response.result);
  EXPECT_EQ(COMMAND_DELETE_SPACE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_DELETE_SPACE>());
}

TEST(NvramMessagesTest, DisableCreateRequest) {
  Request request;
  request.payload.Activate<COMMAND_DISABLE_CREATE>();

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_DISABLE_CREATE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_DISABLE_CREATE>());
}

TEST(NvramMessagesTest, DisableCreateResponse) {
  Response response;
  response.result = NV_RESULT_INTERNAL_ERROR;
  response.payload.Activate<COMMAND_DISABLE_CREATE>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_INTERNAL_ERROR, response.result);
  EXPECT_EQ(COMMAND_DISABLE_CREATE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_DISABLE_CREATE>());
}

TEST(NvramMessagesTest, WriteSpaceRequest) {
  Request request;
  WriteSpaceRequest& request_payload =
      request.payload.Activate<COMMAND_WRITE_SPACE>();
  request_payload.index = 0x1234;
  const uint8_t kData[] = {17, 29, 33};
  ASSERT_TRUE(request_payload.buffer.Assign(kData, sizeof(kData)));
  const uint8_t kAuthValue[] = {1, 2, 3};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_WRITE_SPACE, decoded.payload.which());
  const WriteSpaceRequest* decoded_payload =
      decoded.payload.get<COMMAND_WRITE_SPACE>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
  const Blob& decoded_buffer = decoded_payload->buffer;
  ASSERT_EQ(sizeof(kData), decoded_buffer.size());
  EXPECT_EQ(0, memcmp(kData, decoded_buffer.data(), sizeof(kData)));
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, WriteSpaceResponse) {
  Response response;
  response.result = NV_RESULT_OPERATION_DISABLED;
  response.payload.Activate<COMMAND_WRITE_SPACE>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_OPERATION_DISABLED, response.result);
  EXPECT_EQ(COMMAND_WRITE_SPACE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_WRITE_SPACE>());
}

TEST(NvramMessagesTest, ReadSpaceRequest) {
  Request request;
  ReadSpaceRequest& request_payload =
      request.payload.Activate<COMMAND_READ_SPACE>();
  request_payload.index = 0x1234;
  const uint8_t kAuthValue[] = {1, 2, 3};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_READ_SPACE, decoded.payload.which());
  const ReadSpaceRequest* decoded_payload =
      decoded.payload.get<COMMAND_READ_SPACE>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, ReadSpaceResponse) {
  Response response;
  response.result = NV_RESULT_SUCCESS;
  ReadSpaceResponse& response_payload =
      response.payload.Activate<COMMAND_READ_SPACE>();
  const uint8_t kData[] = {48, 0, 32, 1, 255};
  ASSERT_TRUE(response_payload.buffer.Assign(kData, sizeof(kData)));

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SUCCESS, response.result);
  EXPECT_EQ(COMMAND_READ_SPACE, decoded.payload.which());
  const ReadSpaceResponse* decoded_payload =
      decoded.payload.get<COMMAND_READ_SPACE>();
  ASSERT_TRUE(decoded_payload);
  const Blob& decoded_buffer = decoded_payload->buffer;
  ASSERT_EQ(sizeof(kData), decoded_buffer.size());
  EXPECT_EQ(0, memcmp(kData, decoded_buffer.data(), sizeof(kData)));
}

TEST(NvramMessagesTest, LockSpaceWriteRequest) {
  Request request;
  LockSpaceWriteRequest& request_payload =
      request.payload.Activate<COMMAND_LOCK_SPACE_WRITE>();
  request_payload.index = 0x1234;
  const uint8_t kAuthValue[] = {1, 2, 3};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_LOCK_SPACE_WRITE, decoded.payload.which());
  const LockSpaceWriteRequest* decoded_payload =
      decoded.payload.get<COMMAND_LOCK_SPACE_WRITE>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, LockSpaceWriteResponse) {
  Response response;
  response.result = NV_RESULT_SUCCESS;
  response.payload.Activate<COMMAND_LOCK_SPACE_WRITE>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SUCCESS, response.result);
  EXPECT_EQ(COMMAND_LOCK_SPACE_WRITE, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_LOCK_SPACE_WRITE>());
}

TEST(NvramMessagesTest, LockSpaceReadRequest) {
  Request request;
  LockSpaceReadRequest& request_payload =
      request.payload.Activate<COMMAND_LOCK_SPACE_READ>();
  request_payload.index = 0x1234;
  const uint8_t kAuthValue[] = {1, 2, 3};
  ASSERT_TRUE(request_payload.authorization_value.Assign(kAuthValue,
                                                         sizeof(kAuthValue)));

  Request decoded;
  EncodeAndDecode(request, &decoded);

  EXPECT_EQ(COMMAND_LOCK_SPACE_READ, decoded.payload.which());
  const LockSpaceReadRequest* decoded_payload =
      decoded.payload.get<COMMAND_LOCK_SPACE_READ>();
  ASSERT_TRUE(decoded_payload);

  EXPECT_EQ(0x1234U, decoded_payload->index);
  const Blob& decoded_auth_value = decoded_payload->authorization_value;
  ASSERT_EQ(sizeof(kAuthValue), decoded_auth_value.size());
  EXPECT_EQ(0,
            memcmp(kAuthValue, decoded_auth_value.data(), sizeof(kAuthValue)));
}

TEST(NvramMessagesTest, LockSpaceReadResponse) {
  Response response;
  response.result = NV_RESULT_SUCCESS;
  response.payload.Activate<COMMAND_LOCK_SPACE_READ>();

  Response decoded;
  EncodeAndDecode(response, &decoded);

  EXPECT_EQ(NV_RESULT_SUCCESS, response.result);
  EXPECT_EQ(COMMAND_LOCK_SPACE_READ, decoded.payload.which());
  EXPECT_TRUE(decoded.payload.get<COMMAND_LOCK_SPACE_READ>());
}

TEST(NvramMessagesTest, GarbageDecode) {
  srand(0);
  uint8_t random_data[1024];
  for (size_t i = 0; i < sizeof(random_data); ++i) {
    random_data[i] = rand() & 0xff;
  }

  // Feed the data to the decoder. The test succeeds if we don't crash.
  for (size_t pos = 0; pos < sizeof(random_data); ++pos) {
    for (size_t end = pos; end < sizeof(random_data); ++end) {
      Request request;
      Decode(random_data + pos, end - pos, &request);

      Response response;
      Decode(random_data + pos, end - pos, &response);
    }
  }
}

}  // namespace nvram
