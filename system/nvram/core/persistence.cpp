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

#include "nvram/core/persistence.h"

#include <nvram/messages/io.h>
#include <nvram/messages/proto.hpp>

#include <nvram/core/logger.h>

namespace nvram {

namespace {

// Magic constants that identify encoded |NvramHeader| vs. |NvramSpace| objects.
const uint32_t kHeaderMagic = 0x4e5648;  // "NVH" in hex
const uint32_t kSpaceMagic = 0x4e5653;   // "NVS" in hex

// Encodes an |object| as a protobuf message and writes it to |blob|. Note that
// standard protobuf encoding doesn't include information about the overall size
// of the encoded object. This is not good enough here, as encoding should
// gracefully handle trailing data on decode, e.g. to allow underlying storage
// systems that only provide block-granular I/O.
//
// Not that the code uses |proto::detail::MessageEncoder<Object>::Encode()|
// instead of the regular |proto::Encode()| to encode the message. This results
// in the message being wrapped in a length-delimited proto field record, so the
// length field can be used to determine the actual length of the message. Also,
// this gives us the opportunity to encode a magic constant in the field number
// bits of the wire tag, thus allowing us to detect situations where we're
// attempting to decode a message of wrong type.
template <uint32_t magic, typename Object>
storage::Status EncodeObject(const Object& object, Blob* blob) {
  BlobOutputStreamBuffer stream(blob);
  ProtoWriter writer(&stream);
  writer.set_field_number(magic);
  if (!proto::detail::MessageEncoder<Object>::Encode(object, &writer) ||
      !stream.Truncate()) {
    NVRAM_LOG_ERR("Failed to encode object.");
    return storage::Status::kStorageError;
  }
  return storage::Status::kSuccess;
}

// Decodes a protobuf-encoded |object| from |blob|. It is OK if the provided
// |blob| includes trailing data that doesn't belong to the encoded object.
//
// Note that the code below reads the wire tag to strip the wrapping proto field
// record produced by |EncodeObject|. It then checks the magic field number to
// make sure we're decoding a message of correct type. Finally,
// |proto::detail::MessageDecoder<Object>::Decode()| takes care of reading the
// message payload from the proto field record.
template <uint32_t magic, typename Object>
storage::Status DecodeObject(const Blob& blob, Object* object) {
  InputStreamBuffer stream(blob.data(), blob.size());
  ProtoReader reader(&stream);
  if (!reader.ReadWireTag() || reader.field_number() != magic ||
      reader.wire_type() != WireType::kLengthDelimited ||
      !proto::detail::MessageDecoder<Object>::Decode(*object, &reader)) {
    NVRAM_LOG_ERR("Failed to decode object of size %zu.", blob.size());
    return storage::Status::kStorageError;
  }
  return storage::Status::kSuccess;
}

}  // namespace

template <> struct DescriptorForType<NvramHeader> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &NvramHeader::version),
                    MakeField(2, &NvramHeader::flags),
                    MakeField(3, &NvramHeader::allocated_indices),
                    MakeField(4, &NvramHeader::provisional_index));
};

template <> struct DescriptorForType<NvramSpace> {
  static constexpr auto kFields =
      MakeFieldList(MakeField(1, &NvramSpace::flags),
                    MakeField(2, &NvramSpace::controls),
                    MakeField(3, &NvramSpace::authorization_value),
                    MakeField(4, &NvramSpace::contents));
};

namespace persistence {

storage::Status LoadHeader(NvramHeader* header) {
  Blob blob;
  storage::Status status = storage::LoadHeader(&blob);
  if (status != storage::Status::kSuccess) {
    return status;
  }
  return DecodeObject<kHeaderMagic>(blob, header);
}

storage::Status StoreHeader(const NvramHeader& header) {
  Blob blob;
  storage::Status status = EncodeObject<kHeaderMagic>(header, &blob);
  if (status != storage::Status::kSuccess) {
    return status;
  }
  return storage::StoreHeader(blob);
}

storage::Status LoadSpace(uint32_t index, NvramSpace* space) {
  Blob blob;
  storage::Status status = storage::LoadSpace(index, &blob);
  if (status != storage::Status::kSuccess) {
    return status;
  }
  return DecodeObject<kSpaceMagic>(blob, space);
}

storage::Status StoreSpace(uint32_t index, const NvramSpace& space) {
  Blob blob;
  storage::Status status = EncodeObject<kSpaceMagic>(space, &blob);
  if (status != storage::Status::kSuccess) {
    return status;
  }
  return storage::StoreSpace(index, blob);
}

storage::Status DeleteSpace(uint32_t index) {
  return storage::DeleteSpace(index);
}

}  // namespace persistence
}  // namespace nvram
