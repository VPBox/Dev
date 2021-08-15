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

// This file provides facilities to declare compile-time descriptors for C++
// struct types. This enables generic code to access the declared struct
// members in an object.
//
// For example, consider the following struct type:
//
//   struct Employee {
//     uint32_t id;
//     std::string name;
//     std::vector<uint32_t> reports;
//   };
//
// The descriptor is declared as follows, providing access to |Employee|'s
// members and assigning a unique field number to each of them:
//
//   template <>
//   struct DescriptorForType<Employee> {
//     static constexpr auto kFields =
//         MakeFieldList(MakeField(1, &Employee::id),
//                       MakeField(2, &Employee::name),
//                       MakeField(3, &Employee::reports));
//   };
//
// Note that the |kFields| member is a constexpr, which creates a compile-time
// constant, so the field meta data can be used in compile-time computations and
// as template parameters.
//
// To access the declared members, there is a |Get()| member function template
// on the declared field list, which allows to retrieve one of the field
// specifications by index (zero-based declaration index, *not* field number).
// Once you have the field spec for a field, you can use |FieldSpec::Get()| to
// get a reference to the member within a struct instance. This can be used to
// implement generic algorithms that make use of the descriptor behind the
// scenes. Here is an example that shows how to build a generic comparator:
//
//   template <typename Struct>
//   struct StructCompare {
//     template <typename Member>
//     int compareMember(const Member& left, const Member& right) {
//       return left < right ? -1 : (right < left ? 1 : 0);
//     }
//
//     template <size_t... indices>
//     int compare(const Struct& left,
//                 const Struct& right,
//                 index_sequence<indices...>) {
//       constexpr auto kFieldSpecList = DescriptorForType<Struct>::kFields;
//       int results[] = {compareMember(
//           kFieldSpecList.template Get<indices>().Get(left),
//           kFieldSpecList.template Get<indices>().Get(right))...};
//       for (int result : results) {
//         if (result != 0) {
//           return result;
//         }
//       }
//
//       return 0;
//     }
//
//     bool operator()(const Struct& left, const Struct& right) {
//       constexpr auto kFieldSpecList = DescriptorForType<Struct>::kFields;
//       return compare(left, right,
//                      make_index_sequence<kFieldSpecList.kSize>()) < 0;
//     }
//   };
//
// You can now use |StructCompare| as a key comparison function with std::set
// like this:
//
//   std::set<Employee, StructCompare<Employee>> employees;
//   employees.emplace(std::move(new_employee));
//
// The ability to write generic algorithms that can process arbitrarily-typed
// struct fields comes at the cost of heavy usage of template constructs.
// However, potential alternatives are not without drawbacks:
//  * Avoiding generic code entirely and writing the necessary operations for
//    each struct type manually is tedious and error-prone.
//  * Tool-generated code is just as hard to comprehend and maintain, and code
//    making use of the generated constructs may need to be generated as well.
//  * For the intended use in message serialization, there are existing message
//    serialization solutions such as protobuf. Unfortunately, our serialization
//    code needs to run in resource-constrained environments that don't provide
//    a C++ standard library (which is a dependency of the regular protobuf
//    implementation), and the library weighs in as a non-trivial dependency in
//    terms of code size.

#ifndef NVRAM_MESSAGES_STRUCT_H_
#define NVRAM_MESSAGES_STRUCT_H_

#include <nvram/messages/type_traits.h>

namespace nvram {

// This class template is used to resolve struct types to their corresponding
// descriptors, which provide a list of struct fields that includes the field
// numbers as well as the corresponding C++ struct members in |Struct|. See the
// file comment above for an example.
template <typename Struct>
struct DescriptorForType;

// |FieldSpec| describes a member field of the struct type |Struct|. The
// template parameters capture the C++ |Member| type of the |Struct| member that
// holds the field's data.
//
// Note that this class template is a literal type, i.e. can be used with
// constexpr. As an implication, |FieldSpec| instances can be used as
// compile-time data.
template <typename Struct, typename Member>
struct FieldSpec {
  using MemberType = Member;

  constexpr FieldSpec(uint32_t field_number, MemberType Struct::* member)
      : kFieldNumber(field_number), kMember(member) {}

  const MemberType& Get(const Struct& object) const {
    return object.*kMember;
  }

  MemberType& Get(Struct& object) const {
    return object.*kMember;
  }

  // The field number for this field.
  const uint32_t kFieldNumber;

  // A member pointer to the |Struct| member that holds the field data.
  MemberType Struct::*const kMember;
};

// A helper function template that enables template argument deduction to be
// used to construct |FieldSpec| instances.
template <typename Struct, typename Member>
constexpr FieldSpec<Struct, Member> MakeField(uint32_t field_number,
                                              Member Struct::*member) {
  return FieldSpec<Struct, Member>(field_number, member);
};

// Forward declaration for |TaggedUnion|, so we don't have to include the full
// header.
template <typename TagType, typename... Member>
class TaggedUnion;

// A special field specification type for protobuf fields belonging to a "oneof"
// construct, of which one field may be active at a time. This is represented by
// a |TaggedUnion| struct member. In addition to the field number and member
// pointer, the field specification also records the |TaggedUnion| tag value
// that selects the |TaggedUnion| member which corresponds to the field.
template <typename Struct, typename TagType, typename... Member>
struct OneOfFieldSpec
    : public FieldSpec<Struct, TaggedUnion<TagType, Member...>> {
  using TaggedUnionType = TaggedUnion<TagType, Member...>;

  constexpr OneOfFieldSpec(uint32_t field_number,
                           TaggedUnionType Struct::*member,
                           TagType tag)
      : FieldSpec<Struct, TaggedUnionType>(field_number, member), kTag(tag) {}

  // The |TaggedUnion| tag corresponding to the |TaggedUnion| member that holds
  // the field's data.
  const TagType kTag;
};

// A helper function template that simplifies |OneOfFieldSpec| creation by
// enabling template argument type deduction.
template <typename Struct, typename TagType, typename... Member>
constexpr OneOfFieldSpec<Struct, TagType, Member...> MakeOneOfField(
    uint32_t field_number,
    TaggedUnion<TagType, Member...> Struct::*member,
    TagType tag) {
  return OneOfFieldSpec<Struct, TagType, Member...>(field_number, member, tag);
};

// A simple type list intended to hold field specification values.
//
// Note that |FieldSpecList| is a literal type so can be used with constexpr to
// hold compile-time data.
template <typename... FieldSpec>
struct FieldSpecList;

namespace {

// A helper template that extracts the field spec at |index| from a field spec
// list.
template <size_t index, typename... FieldSpec>
struct FieldSpecLookup;

// Recursion step: This specialization matches if |index| is larger than 0, and
// the |Get()| definition just forwards to the list tail.
template <size_t index, typename FieldSpec, typename... Tail>
struct FieldSpecLookup<index, FieldSpec, Tail...> {
  using Prev = FieldSpecLookup<index - 1, Tail...>;
  using Type = typename Prev::Type;
  static constexpr Type Get(FieldSpecList<FieldSpec, Tail...> self) {
    return Prev::Get(self.kTail);
  }
};

// Recursion base case: |index| as reached 0, so |Get()| returns the field spec
// corresponding to the current |FieldSpec|.
template <typename FieldSpec, typename... Tail>
struct FieldSpecLookup<0, FieldSpec, Tail...> {
  using Type = FieldSpec;
  static constexpr Type Get(FieldSpecList<FieldSpec, Tail...> self) {
    return self.kFieldSpec;
  }
};

// Produces an error message in case the provided |index| is too large, i.e.
// doesn't match any field. This specialization only matches once the
// |FieldSpec| parameters are exhausted.
template <size_t index>
struct FieldSpecLookup<index> {
  // Note that |index < 0| will never be satisfied, so this static assert
  // triggers unconditionally if this template specialization ever gets
  // instantiated. It will only be instantiated if |index| exceeds the number of
  // declared fields.
  //
  // Just putting |false| as the static_assert condition would seem a saner
  // alternative, but doesn't work since the static_assert would then be
  // evaluated at declaration time. Using the |index| parameter in the condition
  // forces evaluation to take place at template instantiation time.
  static_assert(index < 0, "Out-of-bounds |index| in field spec lookup.");
};

}  // namespace

// |FieldSpecList| specialization that holds the data of the front-most element
// of |FieldSpecList|'s |Fields| arguments. Note that this class contains a
// nested |FieldSpecList| instance with the front-most element removed, thus
// inheriting the members for subsequent |Fields| arguments.
template <typename FieldSpec, typename... Tail>
struct FieldSpecList<FieldSpec, Tail...> {
  using List = FieldSpecList<FieldSpec, Tail...>;
  using TailList = FieldSpecList<Tail...>;

  constexpr explicit FieldSpecList(FieldSpec field_spec, Tail... tail)
      : kFieldSpec(field_spec), kTail(tail...) {}

  template <size_t index>
  constexpr typename FieldSpecLookup<index, FieldSpec, Tail...>::Type Get()
      const {
    return FieldSpecLookup<index, FieldSpec, Tail...>::Get(*this);
  }

  static constexpr size_t kSize = TailList::kSize + 1;
  const FieldSpec kFieldSpec;
  const TailList kTail;
};

// |FieldSpecList| specialization acting as the recursion base case. This
// doesn't have further members and thus stops the expansion of
// |FieldSpecList|'s |Fields| parameter.
template <>
struct FieldSpecList<> {
  static constexpr size_t kSize = 0;
};

// Helper function template that enables convenient creation of |FieldSpecList|
// instances by enabling template argument deduction.
template <typename... FieldSpec>
constexpr FieldSpecList<FieldSpec...> MakeFieldList(FieldSpec... field_spec) {
  return FieldSpecList<FieldSpec...>(field_spec...);
}

}  // namespace nvram

#endif  // NVRAM_MESSAGES_STRUCT_H_
