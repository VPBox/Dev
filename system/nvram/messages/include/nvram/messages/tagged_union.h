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

// This file defines the |TaggedUnion| class template. It implements a tagged
// union, i.e. it both holds a value of one of a set of pre-determined types, as
// well as an enum value indicating which union member is active. The enum type
// used as tag and the set of union member types a specified as template
// parameters.
//
// For example, consider this declaration of a simple variant type:
//
//   enum VariantType {
//     Variant_Int = 0,
//     Variant_Double = 1,
//     Variant_Boolean = 3,
//     Variant_String = 2,
//   };
//
//   using Variant = TaggedUnion<VariantType,
//                        TaggedUnionMember<Variant_Int, int>,
//                        TaggedUnionMember<Variant_Double, double>,
//                        TaggedUnionMember<Variant_Boolean, bool>,
//                        TaggedUnionMember<Variant_String, std::string>>;
//
// |TaggedUnion::which()| can be used to determine what the active member is,
// and |TaggedUnion::get()| returns a pointer to the member as long as that
// member is active:
//
//   Variant value;
//   ASSERT_EQ(Variant_Int, value.which());
//   ASSERT_NE(nullptr, value.get<Variant_Int>());
//   ASSERT_EQ(0, *value.get<Variant_Int>());
//
// |TaggedUnion::Activate()| activates a member. It returns a reference to the
// activated member:
//
//   value.Activate<Variant_String>() = "-1";
//   ASSERT_EQ(Variant_String, value.which());
//
// To allow generic code to process a |TaggedUnion|, you can use a variant of
// what is commonly known as "the indices trick". The idea is to use template
// parameter pack expansion on |TaggedUnion|'s |Member| parameter to invoke some
// function for each member. For example, the following code determines the
// integer value of a |Variant| value as declared above:
//
//   template <typename Type>
//   void MemberToInt(const Type* member, int* value) {
//     if (member) {
//       *value = static_cast<int>(*member);
//     }
//   }
//
//   void MemberToInt(const std::string* member, int* value) {
//     if (member) {
//       *value = std::stoi(*member);
//     }
//   }
//
//   template <typename... Member>
//   int VariantToInt(const TaggedUnion<VariantType, Member...>& variant) {
//     int value = 0;
//     int dummy[] = {
//         (MemberToInt(
//              variant.template get<static_cast<VariantType>(Member::kTag)>(),
//              &value),
//          0)...};
//     (void)dummy;
//     return value;
//   };
//
// Using this, you can convert a |Variant| in arbitrary state to an integer:
//
//   ASSERT_EQ(-1, VariantToInt(value));

#ifndef NVRAM_MESSAGES_TAGGED_UNION_H_
#define NVRAM_MESSAGES_TAGGED_UNION_H_

extern "C" {
#include <stddef.h>
}

#include <new>

#include <nvram/messages/compiler.h>

namespace nvram {

template <uint64_t tag, typename Member>
struct TaggedUnionMember {
  static constexpr uint64_t kTag = tag;
  using Type = Member;
};

template <typename TagType, typename... Members>
class TaggedUnion;

namespace detail {

// A compile-time maximum implementation.
template <size_t... Values>
struct Max;

template <>
struct Max<> {
  static constexpr size_t value = 0;
};

template <size_t head, size_t... tail>
struct Max<head, tail...> {
  static constexpr size_t value =
      head > Max<tail...>::value ? head : Max<tail...>::value;
};

// A helper template that determines the |TaggedUnionMember| type corresponding
// to |tag| via recursive expansion of the |Member| parameter list.
template <typename TagType, TagType tag, typename... Member>
struct MemberForTag;

template <typename TagType,
          TagType tag,
          uint64_t member_tag,
          typename MemberType,
          typename... Tail>
struct MemberForTag<TagType,
                    tag,
                    TaggedUnionMember<member_tag, MemberType>,
                    Tail...> {
  using Type = typename MemberForTag<TagType, tag, Tail...>::Type;
};

template <typename TagType, TagType tag, typename MemberType, typename... Tail>
struct MemberForTag<TagType,
                    tag,
                    TaggedUnionMember<static_cast<uint64_t>(tag), MemberType>,
                    Tail...> {
  using Type = TaggedUnionMember<tag, MemberType>;
};

// Extracts the first element of its template parameter list.
template <typename Elem, typename...Tail>
struct Head {
  using Type = Elem;
};

}  // namespace detail

template <typename TagType, typename... Member>
class TaggedUnion {
 public:
  template <TagType tag>
  struct MemberLookup {
    using Type = typename detail::MemberForTag<TagType, tag, Member...>::Type;
  };

  // Construct a |TaggedUnion| object. Note that the constructor will activate
  // the first declared union member.
  TaggedUnion() {
    Construct<static_cast<TagType>(detail::Head<Member...>::Type::kTag)>();
  }

  ~TaggedUnion() {
    Destroy();
  }

  // |TaggedUnion| is copyable and movable, provided the members have suitable
  // copy and move assignment operators.
  TaggedUnion(const TaggedUnion<TagType, Member...>& other) {
    CopyFrom(other);
  }
  TaggedUnion(TaggedUnion<TagType, Member...>&& other) {
    MoveFrom(other);
  }
  TaggedUnion<TagType, Member...>& operator=(
      const TaggedUnion<TagType, Member...>& other) {
    CopyFrom(other);
  }
  TaggedUnion<TagType, Member...>& operator=(
      TaggedUnion<TagType, Member...>&& other) {
    MoveFrom(other);
  }

  // Returns the tag value corresponding to the active member.
  TagType which() const { return which_; }

  // Get a pointer to the member corresponding to |tag|. Returns nullptr if
  // |tag| doesn't correspond to the active member.
  template <TagType tag>
  const typename MemberLookup<tag>::Type::Type* get() const {
    return which_ == tag ? GetUnchecked<tag>() : nullptr;
  }

  // Get a pointer to the member corresponding to |tag|. Returns nullptr if
  // |tag| doesn't correspond to the active member.
  template <TagType tag>
  typename MemberLookup<tag>::Type::Type* get() {
    return which_ == tag ? GetUnchecked<tag>() : nullptr;
  }

  // Activate the member identified by |tag|. First, the currently active member
  // will be destroyed. Then, the member corresponding to |tag| will be
  // constructed (i.e. value-initialized). Returns a reference to the activated
  // member.
  template <TagType tag>
  typename MemberLookup<tag>::Type::Type& Activate() {
    Destroy();
    Construct<tag>();
    return *GetUnchecked<tag>();
  }

 private:
  template<TagType tag>
  const typename MemberLookup<tag>::Type::Type* GetUnchecked() const {
    return reinterpret_cast<const typename MemberLookup<tag>::Type::Type*>(
        storage_);
  }

  template<TagType tag>
  typename MemberLookup<tag>::Type::Type* GetUnchecked() {
    return reinterpret_cast<typename MemberLookup<tag>::Type::Type*>(storage_);
  }

  template <TagType tag>
  void Construct() {
    using MemberType = typename MemberLookup<tag>::Type::Type;
    new (storage_) MemberType();
    which_ = tag;
  }

  template <typename CurrentMember>
  void DestroyMember() {
    using MemberType = typename CurrentMember::Type;
    if (CurrentMember::kTag == which_) {
      GetUnchecked<static_cast<TagType>(CurrentMember::kTag)>()->~MemberType();
    }
  }

  // This is marked noinline to prevent bloat due to the compiler inlining
  // |Destroy()| into each instance of the |Activate()| function.
  NVRAM_NOINLINE void Destroy() {
    int dummy[] = {(DestroyMember<Member>(), 0)...};
    (void)dummy;
  }

  template <typename CurrentMember>
  void CopyMember(const typename CurrentMember::Type* member) {
    if (member) {
      if (CurrentMember::kTag != which_) {
        Activate<CurrentMember::kTag>();
      }
      *GetUnchecked<static_cast<TagType>(CurrentMember::kTag)>() = *member;
    }
  }

  NVRAM_NOINLINE void CopyFrom(const TaggedUnion<TagType, Member...>& other) {
    int dummy[] = {
        (CopyMember<Member>(
             other.template get<static_cast<TagType>(Member::kTag)>()),
         0)...};
    (void)dummy;
  }

  template <typename CurrentMember>
  void MoveMember(const typename CurrentMember::Type* member) {
    if (member) {
      if (CurrentMember::kTag != which_) {
        Activate<CurrentMember::kTag>();
      }
      *GetUnchecked<static_cast<TagType>(CurrentMember::kTag)>() =
          static_cast<typename CurrentMember::Type&&>(*member);
    }
  }

  NVRAM_NOINLINE void MoveFrom(const TaggedUnion<TagType, Member...>& other) {
    int dummy[] = {
        (MoveMember<Member>(
             other.template get<static_cast<TagType>(Member::kTag)>()),
         0)...};
    (void)dummy;
  }

  // The + 0 is required to work around a G++ bug:
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55382
  alignas(detail::Max<alignof(typename Member::Type)...>::value + 0)
      uint8_t storage_[detail::Max<sizeof(typename Member::Type)...>::value];
  TagType which_;
};

}  // namespace nvram

#endif  // NVRAM_MESSAGES_TAGGED_UNION_H_
