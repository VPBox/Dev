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

#ifndef CHRE_UTIL_UNIQUE_PTR_H_
#define CHRE_UTIL_UNIQUE_PTR_H_

#include <cstddef>

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * Wraps a pointer to a dynamically allocated object and manages the underlying
 * memory. The goal is to be similar to std::unique_ptr, but we do not support
 * custom deleters - deletion is always done via memoryFree().
 */
template<typename ObjectType>
class UniquePtr : public NonCopyable {
 public:
  /**
   * Pointer type of ObjectType.
   */
  typedef ObjectType* pointer;

  /**
   * Construct a UniquePtr instance that does not own any object.
   */
  UniquePtr();

  /**
   * Constructs a UniquePtr instance that owns the given object, and will free
   * its memory when the UniquePtr is destructed.
   *
   * @param object Pointer to an object allocated via memoryAlloc. It is not
   *        valid for this object's memory to come from any other source,
   *        including the stack, or static allocation on the heap.
   */
  UniquePtr(ObjectType *object);

  /**
   * Constructs a new UniquePtr via moving the Object from another UniquePtr.
   *
   * @param other UniquePtr instance to move into this object
   */
  UniquePtr(UniquePtr<ObjectType>&& other);

  /**
   * Constructs a new UniquePtr via moving the Object from another UniquePtr.
   * This constructor allows conversion (ie: upcast) to another type if
   * possible.
   *
   * @param other UniquePtr instance to move and convert into this object.
   */
  template<typename OtherObjectType>
  UniquePtr(UniquePtr<OtherObjectType>&& other);

  /**
   * Deconstructs the object (if necessary) and releases associated memory.
   */
  ~UniquePtr();

  /**
   * Determines if this UniquePtr owns an object, or references null.
   *
   * @return true if get() returns nullptr
   */
  bool isNull() const;

  /**
   * @return A pointer to the underlying object, or nullptr if this object is
   *         not currently valid.
   */
  ObjectType *get() const;

  /**
   * Releases ownership of the underlying object, so it will not be freed when
   * this object is destructed. After this function returns, get() will return
   * nullptr.
   *
   * @return A pointer to the underlying object (i.e. what get() would return
   *         prior to this function call)
   */
  ObjectType *release();

  /**
   * Replaces the object owned by the UniquePtr by an object pointed by a given
   * pointer. Also calls the destructor and releases the associated memory of
   * the previously owned object. Invoking this method on the object managed by
   * the UniquePtr, obtained via get(), is illegal.
   *
   * @param object the object to replace the ownership of the UniquePtr
   */
  void reset(ObjectType *object);

  /**
   * Destroys the object owned by the UniquePtr. Also calls the destructor and
   * releases the associated memory of the previously owned object.
   */
  void reset();

  /**
   * @return A pointer to the underlying object.
   */
  ObjectType *operator->() const;

  /**
   * @return A reference to the underlying object.
   */
  ObjectType& operator*() const;

  /**
   * @param index The index of an object in the underlying array object.
   * @return A reference to the underlying object at an index.
   */
  ObjectType& operator[](size_t index) const;

  /**
   * Move assignment operator. Ownership of this object is transferred and the
   * other object is left in an invalid state.
   *
   * @param other The other object being moved.
   * @return A reference to the newly moved object.
   */
  UniquePtr<ObjectType>& operator=(UniquePtr<ObjectType>&& other);

  /**
   * Two unique_ptr compare equal (==) if their stored pointers compare equal,
   * and not equal (!=) otherwise.
   *
   * @param other The other object being compared.
   * @return true if the other's pointer is same as the underlying pointer,
   * otherwise false.
   */
  bool operator==(const UniquePtr<ObjectType> &other) const;

  /**
   * Two unique_ptr compare equal (==) if their stored pointers compare equal,
   * and not equal (!=) otherwise.
   *
   * @param other The other object being compared.
   * @return true if the other's pointer is different than the underlying
   * pointer, otherwise false.
   */
  bool operator!=(const UniquePtr<ObjectType> &other) const;

 private:
  // Befriend this class to itself to allow the templated conversion constructor
  // permission to access mObject below.
  template<typename OtherObjectType>
  friend class UniquePtr;

  //! A pointer to the underlying storage for this object.
  ObjectType *mObject;
};

/**
 * Allocates and constructs a new object of type ObjectType on the heap, and
 * returns a UniquePtr that owns the object. This function is similar to
 * std::make_unique.
 *
 * @param args The arguments to pass to the object's constructor.
 */
template<typename ObjectType, typename... Args>
UniquePtr<ObjectType> MakeUnique(Args&&... args);

/**
 * Just like MakeUnique(), except it zeros out any allocated memory. Intended to
 * be used for creating objects that have trivial constructors (e.g. C structs)
 * but should start with a known state.
 */
template<typename ObjectType>
UniquePtr<ObjectType> MakeUniqueZeroFill();

}  // namespace chre

#include "chre/util/unique_ptr_impl.h"

#endif  // CHRE_UTIL_UNIQUE_PTR_H_
