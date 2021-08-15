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

// This file implements a simple protobuf encoder and decoder. The high-level
// idea is to use C++ structs as data containers corresponding to protobuf
// messages. A descriptor must be provided for a struct via a
// |nvram::DescriptorForType| specialization that declares the protobuf fields
// to encode and decode.
//  * Encoding works by going through the declared fields, and encode the
//    corresponding struct members in protobuf wire format.
//  * Decoding scans through the binary encoded message. It looks at the wire
//    tag decoded form the message to recover the field number as well as
//    the protobuf wire type (i.e. kind of encoding). The field number is then
//    used to locate the struct field declaration so the appropriate decoding
//    logic for the corresponding struct member can be invoked.
//  * The main dispatch point that ties member types to decoding and encoding
//    logic is the |nvram::proto::detail::Codec| template. The idea is that
//    |Codec<Type>| provides encoding and decoding logic for |Type|.
//
// The API for encoding and decoding is straightforward. Consider the following
// Employee struct and its descriptor:
// type:
//
//   struct Employee {
//     uint32_t id;
//     std::string name;
//     std::vector<uint32_t> reports;
//   };
//
//   template <>
//   struct DescriptorForType<Employee> {
//     static constexpr auto kFields =
//         MakeFieldList(MakeField(1, &Employee::id),
//                       MakeField(2, &Employee::name),
//                       MakeField(3, &Employee::reports));
//   };
//
// Encoding is simple:
//
//   Employee employee;
//   uint8_t buffer[SIZE];
//   nvram::OutputStream stream(buffer, sizeof(buffer));
//   if (!nvram::proto::Encode(employee, &stream)) {
//     // Handle encoding failure.
//   }
//
// Note that |nvram::proto::GetSize()| can be used to determine a sufficient
// buffer size.
//
// Decoding is similar:
//
//   Employee employee;
//   nvram::InputStreamBuffer stream(buffer_start, buffer_size);
//   if (!nvram::proto::Decode(&employee, &stream)) {
//     // Handle decoding failure.
//   }
//
// Note that this file is not meant as a header to be included by all code that
// needs to encode or decode messages. Rather, this header should only be
// included by a .cpp file which can then instantiate the
// |nvram::proto::Encode()| and |nvram::proto::Decode()| templates to obtain
// encoders and decoders for the relevant message types. This approach results
// in decode and encode logic getting compiled in only one translation unit,
// which other code can link against.

#ifndef NVRAM_MESSAGES_PROTO_HPP_
#define NVRAM_MESSAGES_PROTO_HPP_

extern "C" {
#include <stdint.h>
}

#include <nvram/messages/blob.h>
#include <nvram/messages/compiler.h>
#include <nvram/messages/io.h>
#include <nvram/messages/message_codec.h>
#include <nvram/messages/optional.h>
#include <nvram/messages/struct.h>
#include <nvram/messages/tagged_union.h>
#include <nvram/messages/type_traits.h>
#include <nvram/messages/vector.h>

namespace nvram {
namespace proto {

namespace detail {

// A class template that performs encoding and decoding of a protobuf message
// field of the C++ type |Type|. The base template is left undefined here,
// specific implementations for relevant |Type|s are provided by template
// specializations. Each specialization needs to provide the following members:
//  * |static constexpr WireType kWireType| indicates the wire type used for
//    encoded field data.
//  * |static bool Encode(const Type& object, ProtoWriter* writer)| writes the
//    encoded form of |object| to |writer|.
//  * |static bool Decode(Type& object, ProtoReader* reader)| decodes a field
//    from |reader| and places recovered data in |object|.
//
// |Codec| specializations are provided below for commonly-used types such as
// integral and enum types, as well as structs with corresponding descriptors.
// Additional specializations can be added as needed.
template <typename Type, typename Enable = void>
struct Codec {
  // The assert below fails unconditionally, but must depend on the |Type|
  // parameter so it only triggers at instantiation time. If this assert fires,
  // then you are attempting to encode or decode a struct that contains a field
  // of a C++ type for which there exists no code that implements encoding and
  // decoding for that type. To add encoding/decoding support for a type, you
  // can provide a Codec specialization.
  static_assert(sizeof(Type) == 0,
                "A Codec specialization must be provided for types "
                "that are to be used with the protobuf encoder.");
};

namespace {

// Codec specific message field encoding function. Note that this is marked
// noinline to prevent the compiler from inlining |Codec::Encode| for every
// occurrence of a field of type |Type|.
template <typename Codec, typename Type>
NVRAM_NOINLINE bool EncodeField(const Type& value, ProtoWriter* writer) {
  return Codec::Encode(value, writer);
}

// Codec specific message field decoding function. Note that this is marked
// noinline to prevent the compiler from inlining |Codec::Decode| for every
// occurrence of a field of type |Type|.
template <typename Codec, typename Type>
NVRAM_NOINLINE bool DecodeField(Type& value, ProtoReader* reader) {
  return Codec::Decode(value, reader);
}

}  // namespace

// |Codec| specialization for Blob.
template <>
struct Codec<Blob> {
  static constexpr WireType kWireType = WireType::kLengthDelimited;

  static bool Encode(const Blob& blob, ProtoWriter* writer) {
    return writer->WriteLengthDelimited(blob.data(), blob.size());
  }

  static bool Decode(Blob& blob, ProtoReader* reader) {
    return blob.Resize(reader->field_size()) &&
           reader->ReadLengthDelimited(blob.data(), blob.size());
  }
};

// A helper to test whether a given |Type| should be handled by the Varint
// |Codec| specialization. The |Type| needs to allow conversion from and to
// |uint64_t|. This checks for static_cast conversion behavior instead of
// implicit conversion in order to also match scoped enums.
template <typename Type>
struct IsVarintCompatible {
  template <typename From, typename To>
  struct IsCastConvertible {
    template <typename T>
    static decltype(static_cast<T>(declval<From>()), true_type()) test(int);

    template <typename T>
    static false_type test(...);

    static constexpr bool value = decltype(test<To>(0))::value;
  };

  static constexpr bool value = IsCastConvertible<Type, uint64_t>::value &&
                                IsCastConvertible<uint64_t, Type>::value;
};

// |Codec| specialization for varint-encoded numeric fields.
template <typename Type>
struct Codec<Type, typename enable_if<IsVarintCompatible<Type>::value>::Type> {
  static constexpr WireType kWireType = WireType::kVarint;

  static bool Encode(const Type& value, ProtoWriter* writer) {
    return writer->WriteVarint(static_cast<uint64_t>(value));
  }

  static bool Decode(Type& value, ProtoReader* reader) {
    uint64_t raw_value;
    if (!reader->ReadVarint(&raw_value)) {
      return false;
    }
    value = static_cast<Type>(raw_value);
    return static_cast<uint64_t>(value) == raw_value;
  }
};

// |Codec| specialization for |Vector|.
template <typename ElementType>
struct Codec<Vector<ElementType>> {
  using ElementCodec = Codec<ElementType>;
  static constexpr WireType kWireType = ElementCodec::kWireType;

  static bool Encode(const Vector<ElementType>& vector, ProtoWriter* writer) {
    for (const ElementType& elem : vector) {
      if (!EncodeField<ElementCodec>(elem, writer)) {
        return false;
      }
    }
    return true;
  }

  static bool Decode(Vector<ElementType>& vector, ProtoReader* reader) {
    return vector.Resize(vector.size() + 1) &&
           DecodeField<ElementCodec>(vector[vector.size() - 1], reader);
  }
};

// |Codec| specialization for |Optional|.
template <typename ValueType>
struct Codec<Optional<ValueType>> {
  using ValueCodec = Codec<ValueType>;
  static constexpr WireType kWireType = ValueCodec::kWireType;

  static bool Encode(const Optional<ValueType>& value, ProtoWriter* writer) {
    return !value.valid() || EncodeField<ValueCodec>(value.value(), writer);
  }

  static bool Decode(Optional<ValueType>& value, ProtoReader* reader) {
    return DecodeField<ValueCodec>(value.Activate(), reader);
  }
};

namespace {

// |StructDescriptor| provides the |FieldDescriptor| table corresponding to
// |StructType|. The table contains information about each field in the protobuf
// encoding, e.g. field number and wire type.
//
// The |IndexSequence| template parameter is present purely for technical
// reasons. It provides a sequence of indices, one for each entry in the field
// declaration list for |StructType|. Having the index available simplifies
// generation of the descriptor table entries.
template <
    typename StructType,
    typename IndexSequence = decltype(
        make_index_sequence<DescriptorForType<StructType>::kFields.kSize>())>
struct StructDescriptor;

template <typename StructType, size_t... indices>
struct StructDescriptor<StructType, index_sequence<indices...>> {
 private:
  static constexpr auto kFieldSpecList =
      DescriptorForType<StructType>::kFields;
  using FieldSpecs = typename remove_const<decltype(kFieldSpecList)>::Type;

  // A helper function used to preform a compile-time sanity check on the
  // declared field numbers to ensure that they're positive, unique and in
  // ascending order.
  template <typename FieldSpecList>
  static constexpr bool CheckFieldNumbersAscending(
      FieldSpecList list,
      uint32_t previous_field_number) {
    return list.kFieldSpec.kFieldNumber > previous_field_number &&
           CheckFieldNumbersAscending(list.kTail, list.kFieldSpec.kFieldNumber);
  }
  static constexpr bool CheckFieldNumbersAscending(FieldSpecList<>, uint32_t) {
    return true;
  }

  // If this fails, check your struct field declarations for the following:
  //  * Field numbers must be positive.
  //  * Field numbers must be unique.
  //  * Fields must be declared in ascending field number order.
  static_assert(CheckFieldNumbersAscending(kFieldSpecList, 0),
                "Field numbers must be positive, unique and declared in "
                "ascending order.");

  // Provides the |FieldDescriptor| instance for the field specified by |index|.
  // Note that |index| is *not* the proto field number, but the zero-based index
  // in the field declaration list.
  template <size_t index>
  class FieldDescriptorBuilder {
    static constexpr auto kFieldSpec = kFieldSpecList.template Get<index>();
    using FieldSpecType = typename remove_const<decltype(kFieldSpec)>::Type;
    using MemberType = typename FieldSpecType::MemberType;

    // Determines the Codec type to use for the field. The default is to use
    // |Codec<MemberType>|, which is appropriate for simple fields declared via
    // |FieldSpec|.
    template <typename FieldSpec>
    struct MemberCodecLookup {
      using Type = Codec<MemberType>;
    };

    // |TaggedUnion| members require a special codec implementation that takes
    // into account the case, so encoding only takes place if the respective
    // union member is active and decoding activates the requested member before
    // decoding data.
    template <typename Struct, typename TagType, typename... Member>
    struct MemberCodecLookup<
        OneOfFieldSpec<Struct, TagType, Member...>> {
      static constexpr TagType kTag = kFieldSpec.kTag;

      struct Type {
        using TaggedUnionType = TaggedUnion<TagType, Member...>;
        using TaggedUnionMemberType =
            typename TaggedUnionType::template MemberLookup<kTag>::Type::Type;
        using TaggedUnionMemberCodec = Codec<TaggedUnionMemberType>;
        static constexpr WireType kWireType = TaggedUnionMemberCodec::kWireType;

        static bool Encode(const TaggedUnionType& object, ProtoWriter* writer) {
          const TaggedUnionMemberType* member = object.template get<kTag>();
          if (member) {
            return EncodeField<TaggedUnionMemberCodec>(*member, writer);
          }
          return true;
        }

        static bool Decode(TaggedUnionType& object, ProtoReader* reader) {
          return DecodeField<TaggedUnionMemberCodec>(
              object.template Activate<kTag>(), reader);
        }
      };
    };

    using MemberCodec = typename MemberCodecLookup<FieldSpecType>::Type;

    // Encodes a member. Retrieves a reference to the member within |object| and
    // calls the appropriate encoder.
    static bool EncodeMember(const void* object, ProtoWriter* writer) {
      constexpr auto spec = kFieldSpec;
      return EncodeField<MemberCodec>(
          spec.Get(*static_cast<const StructType*>(object)), writer);
    };

    // Decodes a member. Retrieves a const reference to the member within
    // |object| and calls the appropriate decoder.
    static bool DecodeMember(void* object, ProtoReader* reader) {
      constexpr auto spec = kFieldSpec;
      return DecodeField<MemberCodec>(
          spec.Get(*static_cast<StructType*>(object)), reader);
    };

   public:
    // Assemble the actual descriptor for the field. Note that this is still a
    // compile-time constant (i.e. has no linkage). However, the constant is
    // used below to initialize the entry in the static descriptor table.
    static constexpr FieldDescriptor kDescriptor =
        FieldDescriptor(kFieldSpec.kFieldNumber,
                        MemberCodec::kWireType,
                        &EncodeMember,
                        &DecodeMember);
  };

 public:
  // Descriptor table size.
  static constexpr size_t kNumDescriptors = kFieldSpecList.kSize;

  // The actual descriptor table.
  static constexpr FieldDescriptor kDescriptors[] = {
      FieldDescriptorBuilder<indices>::kDescriptor...};
};

// Provide a definition of the |kDescriptors| array such that the descriptor
// table gets emitted to static data.
template <typename StructType, size_t... index>
constexpr FieldDescriptor
    StructDescriptor<StructType, index_sequence<index...>>::kDescriptors[];

// Note that G++ versions before 5.0 have a bug in handling parameter pack
// expansions that result in an empty array initializer. To work around this,
// the following specialization is provided for empty field lists.
template <typename StructType>
struct StructDescriptor<StructType, index_sequence<>> {
  static constexpr size_t kNumDescriptors = 0;
  static constexpr FieldDescriptor* kDescriptors = nullptr;
};

// A convenience class to initialize |MessageEncoderBase| with the descriptor
// table corresponding to |StructType| as determined by |StructDescriptor|.
template <typename StructType>
class MessageEncoder : public MessageEncoderBase {
 public:
  explicit MessageEncoder(const StructType& object)
      : MessageEncoderBase(&object,
                           StructDescriptor<StructType>::kDescriptors,
                           StructDescriptor<StructType>::kNumDescriptors) {}

  static bool Encode(const StructType& object, ProtoWriter* writer) {
    return MessageEncoderBase::Encode(
        &object, writer, StructDescriptor<StructType>::kDescriptors,
        StructDescriptor<StructType>::kNumDescriptors);
  }
};

// A convenience class to initialize |MessageDecoderBase| with the descriptor
// table corresponding to |StructType| as determined by |StructDescriptor|.
template <typename StructType>
class MessageDecoder : public MessageDecoderBase {
 public:
  explicit MessageDecoder(StructType& object)
      : MessageDecoderBase(&object,
                           StructDescriptor<StructType>::kDescriptors,
                           StructDescriptor<StructType>::kNumDescriptors) {}

  static bool Decode(StructType& object, ProtoReader* reader) {
    return MessageDecoderBase::Decode(
        &object, reader, StructDescriptor<StructType>::kDescriptors,
        StructDescriptor<StructType>::kNumDescriptors);
  }
};

}  // namespace

// |Codec| specialization for struct types. The second template parameter
// evaluates to |void| if the appropriate |DescriptorForType| specialization
// exists, enabling the |Codec| specialization for that case.
//
// Note that this template generates code for each struct type that needs to be
// encoded and decoded. To avoid bloating the binary, we keep the type-dependent
// code at the absolute minimum. The |MessageEncoder| and |MessageDecoder|
// templates merely obtain the appropriate descriptor table for the struct type
// and then invoke the type-agnostic encoder and decoder base classes.
template <typename StructType>
struct Codec<StructType,
             decltype(
                 static_cast<void>(DescriptorForType<StructType>::kFields))> {
  static constexpr WireType kWireType = WireType::kLengthDelimited;

  static bool Encode(const StructType& object, ProtoWriter* writer) {
    return MessageEncoder<StructType>::Encode(object, writer);
  }

  static bool Decode(StructType& object, ProtoReader* reader) {
    return MessageDecoder<StructType>::Decode(object, reader);
  }
};

}  // namespace detail

// Get the encoded size of an object.
template <typename Struct>
size_t GetSize(const Struct& object) {
  detail::MessageEncoder<Struct> encoder(object);
  return encoder.GetSize();
}

// Encode |object| and write the result to |stream|. Returns true if successful,
// false if encoding fails. Encoding may fail because |stream| doesn't have
// enough room to hold the encoded data.
template <typename Struct>
bool Encode(const Struct& object, OutputStreamBuffer* stream) {
  ProtoWriter writer(stream);
  detail::MessageEncoder<Struct> encoder(object);
  return encoder.EncodeData(&writer);
}

// Decode |stream| and update |object| with the decoded information. Returns
// true if successful, false if encoding fails. Failure conditions include:
//  * Binary data isn't valid with respect to the protobuf wire format.
//  * |stream| ends prematurely.
//  * Memory allocation in |object| to hold decoded data fails.
template <typename Struct>
bool Decode(Struct* object, InputStreamBuffer* stream) {
  ProtoReader reader(stream);
  detail::MessageDecoder<Struct> decoder(*object);
  return decoder.DecodeData(&reader);
}

}  // namespace proto
}  // namespace nvram

#endif  // NVRAM_MESSAGES_PROTO_HPP_
