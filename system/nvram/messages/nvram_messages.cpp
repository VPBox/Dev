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

#include <nvram/messages/nvram_messages.h>

#include <nvram/messages/blob.h>
#include <nvram/messages/io.h>
#include <nvram/messages/proto.hpp>

namespace nvram {

// Descriptors for the message types.

// IMPORTANT: The field numbers specified here correspond to protocol buffer
// fields on the wire. While they are arbitrary, they should never be
// reordered, reassigned, or overloaded once defined.
template<> struct DescriptorForType<GetInfoRequest> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<GetInfoResponse> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &GetInfoResponse::total_size),
                    MakeField(2, &GetInfoResponse::available_size),
                    MakeField(3, &GetInfoResponse::max_spaces),
                    MakeField(4, &GetInfoResponse::space_list),
                    MakeField(5, &GetInfoResponse::max_space_size),
                    MakeField(6, &GetInfoResponse::wipe_disabled));
};

template<> struct DescriptorForType<CreateSpaceRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &CreateSpaceRequest::index),
                    MakeField(2, &CreateSpaceRequest::size),
                    MakeField(3, &CreateSpaceRequest::controls),
                    MakeField(4, &CreateSpaceRequest::authorization_value));
};

template<> struct DescriptorForType<CreateSpaceResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<GetSpaceInfoRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &GetSpaceInfoRequest::index));
};

template<> struct DescriptorForType<GetSpaceInfoResponse> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &GetSpaceInfoResponse::size),
                    MakeField(2, &GetSpaceInfoResponse::controls),
                    MakeField(3, &GetSpaceInfoResponse::read_locked),
                    MakeField(4, &GetSpaceInfoResponse::write_locked));
};

template<> struct DescriptorForType<DeleteSpaceRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &DeleteSpaceRequest::index),
                    MakeField(2, &DeleteSpaceRequest::authorization_value));
};

template<> struct DescriptorForType<DeleteSpaceResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<DisableCreateRequest> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<DisableCreateResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<WriteSpaceRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &WriteSpaceRequest::index),
                    MakeField(2, &WriteSpaceRequest::buffer),
                    MakeField(3, &WriteSpaceRequest::authorization_value));
};

template<> struct DescriptorForType<WriteSpaceResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<ReadSpaceRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &ReadSpaceRequest::index),
                    MakeField(2, &ReadSpaceRequest::authorization_value));
};

template<> struct DescriptorForType<ReadSpaceResponse> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &ReadSpaceResponse::buffer));
};

template<> struct DescriptorForType<LockSpaceWriteRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &LockSpaceWriteRequest::index),
                    MakeField(2, &LockSpaceWriteRequest::authorization_value));
};

template<> struct DescriptorForType<LockSpaceWriteResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<LockSpaceReadRequest> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &LockSpaceReadRequest::index),
                    MakeField(2, &LockSpaceReadRequest::authorization_value));
};

template<> struct DescriptorForType<LockSpaceReadResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<WipeStorageRequest> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<WipeStorageResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<DisableWipeRequest> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<DisableWipeResponse> {
  static constexpr auto kFields = MakeFieldList();
};

template<> struct DescriptorForType<Request> {
  static constexpr auto kFields = MakeFieldList(
      MakeOneOfField(1, &Request::payload, COMMAND_GET_INFO),
      MakeOneOfField(2, &Request::payload, COMMAND_CREATE_SPACE),
      MakeOneOfField(3, &Request::payload, COMMAND_GET_SPACE_INFO),
      MakeOneOfField(4, &Request::payload, COMMAND_DELETE_SPACE),
      MakeOneOfField(5, &Request::payload, COMMAND_DISABLE_CREATE),
      MakeOneOfField(6, &Request::payload, COMMAND_WRITE_SPACE),
      MakeOneOfField(7, &Request::payload, COMMAND_READ_SPACE),
      MakeOneOfField(8, &Request::payload, COMMAND_LOCK_SPACE_WRITE),
      MakeOneOfField(9, &Request::payload, COMMAND_LOCK_SPACE_READ),
      MakeOneOfField(10, &Request::payload, COMMAND_WIPE_STORAGE),
      MakeOneOfField(11, &Request::payload, COMMAND_DISABLE_WIPE));
};

template<> struct DescriptorForType<Response> {
  static constexpr auto kFields = MakeFieldList(
      MakeField(1, &Response::result),
      MakeOneOfField(2, &Response::payload, COMMAND_GET_INFO),
      MakeOneOfField(3, &Response::payload, COMMAND_CREATE_SPACE),
      MakeOneOfField(4, &Response::payload, COMMAND_GET_SPACE_INFO),
      MakeOneOfField(5, &Response::payload, COMMAND_DELETE_SPACE),
      MakeOneOfField(6, &Response::payload, COMMAND_DISABLE_CREATE),
      MakeOneOfField(7, &Response::payload, COMMAND_WRITE_SPACE),
      MakeOneOfField(8, &Response::payload, COMMAND_READ_SPACE),
      MakeOneOfField(9, &Response::payload, COMMAND_LOCK_SPACE_WRITE),
      MakeOneOfField(10, &Response::payload, COMMAND_LOCK_SPACE_READ),
      MakeOneOfField(11, &Response::payload, COMMAND_WIPE_STORAGE),
      MakeOneOfField(12, &Response::payload, COMMAND_DISABLE_WIPE));
};

template <typename Message>
bool Encode(const Message& msg, Blob* blob) {
  BlobOutputStreamBuffer stream(blob);
  return nvram::proto::Encode(msg, &stream) && stream.Truncate();
}

template <typename Message>
bool Encode(const Message& msg, void* buffer, size_t* size) {
  ArrayOutputStreamBuffer stream(buffer, *size);
  if (!nvram::proto::Encode(msg, &stream)) {
    return false;
  }
  *size = stream.bytes_written();
  return true;
}

template <typename Message>
bool Decode(const uint8_t* data, size_t size, Message* msg) {
  InputStreamBuffer stream(data, size);
  return nvram::proto::Decode(msg, &stream) && stream.Done();
}

// Instantiate the templates for the |Request| and |Response| message types.
template NVRAM_EXPORT bool Encode<Request>(const Request&, Blob*);
template NVRAM_EXPORT bool Encode<Request>(const Request&, void*, size_t*);
template NVRAM_EXPORT bool Decode<Request>(const uint8_t*, size_t, Request*);

template NVRAM_EXPORT bool Encode<Response>(const Response&, Blob*);
template NVRAM_EXPORT bool Encode<Response>(const Response&, void*, size_t*);
template NVRAM_EXPORT bool Decode<Response>(const uint8_t*, size_t, Response*);

}  // namespace nvram
