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

#include <nvram/messages/message_codec.h>

namespace nvram {
namespace proto {

MessageEncoderBase::MessageEncoderBase(const void* object,
                                       const FieldDescriptor* descriptors,
                                       size_t num_descriptors)
    : object_(object),
      descriptors_(descriptors),
      num_descriptors_(num_descriptors) {}

bool MessageEncoderBase::Encode(const void* object,
                                ProtoWriter* writer,
                                const FieldDescriptor* descriptors,
                                size_t num_descriptors) {
  MessageEncoderBase encoder(object, descriptors, num_descriptors);
  return encoder.Encode(writer);
}

size_t MessageEncoderBase::GetSize() {
  CountingOutputStreamBuffer counting_stream;
  ProtoWriter writer(&counting_stream);
  return EncodeData(&writer) ? counting_stream.bytes_written() : 0;
}

bool MessageEncoderBase::Encode(ProtoWriter* writer) {
  // We need to compute the total size of all struct fields up front in order to
  // write a length delimiter that designates the end of the encoded nested
  // message. Note that computing the size of |object| requires a second
  // |EncodeData()| call in addition to the one that actually encodes the data.
  // When handling nested message structures, each level triggers its own size
  // computation, which are redundant with those performed by the levels above.
  //
  // For now, we just accept this inefficiency in the interest of keeping things
  // simple and correct. If this ever becomes a performance problem for deeply
  // nested structs here are some options:
  //  * Reserve bytes in |writer| for the encoded size. Once |Encode()|
  //    completes, it is known how many bytes were required, at which point the
  //    size field can be updated. The drawback with this solution is that
  //    varint encoding is variable length, so we'd have to write a degenerated
  //    varint that may occupy more bytes than actually required.
  //  * Cache encoded sizes in the struct. This is the solution implemented in
  //    the regular protobuf implementation. This is relatively straightforward,
  //    but at the expense of holding data in struct that doesn't really belong
  //    there.
  //  * Make a first pass over the struct tree, compute sizes and cache them in
  //    some auxiliary data structure held in the encoder. This is probably the
  //    cleanest solution, but comes at the expense of having to thread the size
  //    cache data structure through the encoding logic.
  return writer->WriteLengthHeader(GetSize()) && EncodeData(writer);
}

bool MessageEncoderBase::EncodeData(ProtoWriter* writer) {
  for (size_t i = 0; i < num_descriptors_; ++i) {
    const FieldDescriptor& desc = descriptors_[i];
    writer->set_field_number(desc.field_number);
    if (!desc.encode_function(object_, writer)) {
      return false;
    }
  }

  return true;
}

MessageDecoderBase::MessageDecoderBase(void* object,
                                       const FieldDescriptor* descriptors,
                                       size_t num_descriptors)
    : object_(object),
      descriptors_(descriptors),
      num_descriptors_(num_descriptors) {}

bool MessageDecoderBase::Decode(void* object,
                                ProtoReader* reader,
                                const FieldDescriptor* descriptors,
                                size_t num_descriptors) {
  MessageDecoderBase decoder(object, descriptors, num_descriptors);
  return decoder.Decode(reader);
}

bool MessageDecoderBase::Decode(ProtoReader* reader) {
  NestedInputStreamBuffer nested_stream_buffer(reader->stream_buffer(),
                                               reader->field_size());
  ProtoReader nested_reader(&nested_stream_buffer);
  return DecodeData(&nested_reader) && nested_reader.Done();
}

bool MessageDecoderBase::DecodeData(ProtoReader* reader) {
  while (!reader->Done()) {
    if (!reader->ReadWireTag()) {
      return false;
    }
    const FieldDescriptor* desc = FindDescriptor(reader);
    if (desc) {
      if (!desc->decode_function(object_, reader)) {
        return false;
      }
    } else {
      // Unknown field number or wire type mismatch. Skip field data.
      if (!reader->SkipField()) {
        return false;
      }
    }
  }

  return true;
}

const FieldDescriptor* MessageDecoderBase::FindDescriptor(
    ProtoReader* reader) const {
  for (size_t i = 0; i < num_descriptors_; ++i) {
    const FieldDescriptor& desc = descriptors_[i];
    if (reader->field_number() == desc.field_number &&
        reader->wire_type() == desc.wire_type) {
      return &desc;
    }
  }
  return nullptr;
}

}  // namespace proto
}  // namespace nvram
