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

#ifndef NVRAM_MESSAGES_MESSAGE_CODEC_H_
#define NVRAM_MESSAGES_MESSAGE_CODEC_H_

extern "C" {
#include <stddef.h>
#include <stdint.h>
}

#include <nvram/messages/compiler.h>
#include <nvram/messages/io.h>

namespace nvram {
namespace proto {

// |FieldDescriptor| describes a single proto field in a struct. We compile a
// table of field descriptors for each struct that needs to be encoded or
// decoded into the binary. See proto.hpp for the code that initializes the
// static tables.
struct FieldDescriptor {
  // A function to encode a struct field in protobuf wire format. |object| is a
  // pointer to the struct to read the value from, |writer| receives the output.
  using EncodeFunction = bool(const void* object, ProtoWriter* writer);

  // A function to encode a protobuf struct field from |reader|. The decoded
  // data is stored in the struct instance pointed at by |object|.
  using DecodeFunction = bool(void* object, ProtoReader* reader);

  constexpr FieldDescriptor(uint32_t field_number,
                            WireType wire_type,
                            EncodeFunction* encode_function,
                            DecodeFunction* decode_function)
      : field_number(field_number),
        wire_type(wire_type),
        encode_function(encode_function),
        decode_function(decode_function) {}

  uint32_t field_number;
  WireType wire_type;
  EncodeFunction* encode_function;
  DecodeFunction* decode_function;
};

// A table-driven protobuf message encoder. Takes a pointer to a C++ object to
// encode and a corresponding table of field descriptors. Provides functions to
// encode the message data in protobuf wire format.
class NVRAM_EXPORT MessageEncoderBase {
 public:
  // Initialize the encoder to encode |object|, using the descriptor table
  // passed in |descriptors|.
  MessageEncoderBase(const void* object,
                     const FieldDescriptor* descriptors,
                     size_t num_descriptors);

  // Convenience helper that constructs an encoder instance and invokes
  // |Encode()|.
  static bool Encode(const void* object,
                     ProtoWriter* writer,
                     const FieldDescriptor* descriptors,
                     size_t num_descriptors);

  // Returns the encoded size of the object.
  size_t GetSize();

  // Encodes the object as a sequence of protobuf fields, wrapped in a
  // length-delimited container.
  bool Encode(ProtoWriter* writer);

  // Encodes the object as a sequence of protobuf fields without any wrapping.
  bool EncodeData(ProtoWriter* writer);

 private:
  // The pointer to the object to encode. This is a void pointer, so the encoder
  // logic can be generic and doesn't need to be instantiated for every struct
  // type. The encode function provided by the field descriptor will cast back
  // to the correct type.
  const void* object_;

  // Field descriptor table.
  const FieldDescriptor* descriptors_;
  size_t num_descriptors_;
};

// A protobuf message decoder, driven by a table of field descriptors. Consumes
// data from a |ProtoReader|, decodes fields per the descriptor table and stores
// it to a C++ object.
class NVRAM_EXPORT MessageDecoderBase {
 public:
  // Initialize a decoder to store field data according to the |descriptors|
  // table in |object|.
  MessageDecoderBase(void* object,
                     const FieldDescriptor* descriptors,
                     size_t num_descriptors);

  // Convenience helper that constructs a decoder and invokes |Decode()|.
  static bool Decode(void* object,
                     ProtoReader* reader,
                     const FieldDescriptor* descriptors,
                     size_t num_descriptors);

  // Decode a nested protobuf message wrapped in a length-delimited protobuf
  // field.
  bool Decode(ProtoReader* reader);

  // Decode a protobuf message from reader. This just reads the sequence of
  // fields, not taking into account any wrapping. This is suitable for the
  // topmost encoded message.
  bool DecodeData(ProtoReader* reader);

 private:
  // Looks up the |FieldDescriptor| for decoding the next field. The descriptor
  // must match the field number and wire type of the field. If no matching
  // descriptor is found, |nullptr| is returned.
  const FieldDescriptor* FindDescriptor(ProtoReader* reader) const;

  // The object to decode to. This is a void pointer to keep the decoder generic
  // and avoid type-specific code for each struct type. The decode function in
  // the field descriptor casts back to the struct type.
  void* object_;

  // Descriptor table.
  const FieldDescriptor* descriptors_;
  size_t num_descriptors_;
};

}  // namespace proto
}  // namespace nvram

#endif  // NVRAM_MESSAGES_MESSAGE_CODEC_H_
