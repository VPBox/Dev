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

#ifndef UTIL_CHRE_OPTIONAL_H_
#define UTIL_CHRE_OPTIONAL_H_

#include <type_traits>

namespace chre {

/**
 * This container keeps track of an optional object. The container is similar to
 * std::optional introduced in C++17.
 */
template<typename ObjectType>
class Optional {
 public:
  // Per the standard, a program that instantiates template optional for a
  // reference type is ill-formed
  static_assert(!std::is_reference<ObjectType>::value,
                "Optional references are not allowed");

  /**
   * Default constructs the optional object with no initial value.
   */
  Optional() = default;

  /**
   * Default copy constructor.
   *
   * @param object The object to copy construct from.
   */
  Optional(const Optional<ObjectType>& object) = default;

  /**
   * Default copy constructor.
   *
   * @param object The object to copy construct from.
   */
  Optional(Optional<ObjectType>& object) = default;

  /**
   * Constructs an optional instance with an initial value.
   *
   * @param object The initial value of the object.
   */
  Optional(const ObjectType& object);

  /**
   * Constructs an optional instance with an initial value by moving it.
   *
   * @param object The instance of the initial object to take ownership of.
   */
  Optional(ObjectType&& object);

  /**
   * Destructs the object. Calls through reset() to destroy the contained
   * object before destructing this container.
   */
  ~Optional();

  /**
   * @return Returns true if this container holds an object
   */
  bool has_value() const;

  /**
   * Destroys any contained object, and marks this Optional as empty (i.e.
   * has_value() will return false after this function returns)
   */
  void reset();

  /**
   * Gets a reference to the contained object. Does not check that this optional
   * contains a value, so this object will be uninitialized if has_value() is
   * false.
   */
  ObjectType& value();
  const ObjectType& value() const;

  /**
   * Performs a move assignment operation to the underlying object managed by
   * this container.
   *
   * @param other The other object to move from.
   * @return Returns a reference to this object.
   */
  Optional<ObjectType>& operator=(ObjectType&& other);

  /**
   * Performs a move assignment from one optional to another. Note that the
   * other object still holds a value, but it is left in the moved-from state
   * (as is the case in std::optional).
   *
   * @param other The other object to move.
   * @return Returns a reference to this object.
   */
  Optional<ObjectType>& operator=(Optional<ObjectType>&& other);

  /**
   * Performs a copy assignment operation to the underlying object managed by
   * this container.
   *
   * @param other The other object to copy from.
   * @return Returns a reference to this object.
   */
  Optional<ObjectType>& operator=(const ObjectType& other);

  /**
   * Performs a copy assignment from one optional to another.
   *
   * @param other The other object to copy.
   * @return Returns a reference to this object.
   */
  Optional<ObjectType>& operator=(const Optional<ObjectType>& other);

  /**
   * Obtains a reference to the underlying object managed by this container.
   * The behavior of this is undefined if has_value() returns false.
   *
   * @return Returns a reference to the underlying object tracked by this
   *         container.
   */
  ObjectType& operator*();

  /**
   * Obtains a const reference to the underlying object managed by this
   * container. The behavior of this is undefined if has_value() returns false.
   *
   * @return Returns a const reference to the underlying object tracked by this
   *         container.
   */
  const ObjectType& operator*() const;

  /**
   * Obtains a pointer to the underlying object managed by this container. The
   * object may not be well-formed if has_value() returns false.
   *
   * @return Returns a pointer to the underlying object tracked by this
   *         container.
   */
  ObjectType *operator->();

  /**
   * Obtains a const pointer to the underlying object managed by this container.
   * The object may not be well-formed if has_value() returns false.
   *
   * @return Returns a const pointer to the underlying object tracked by this
   *         container.
   */
  const ObjectType *operator->() const;

 private:
  //! The optional object being tracked by this container.
  typename std::aligned_storage<sizeof(ObjectType), alignof(ObjectType)>::type
      mObject;

  //! Whether or not the object is set.
  bool mHasValue = false;

  ObjectType& object();
  const ObjectType& object() const;

  ObjectType *objectAddr();
  const ObjectType *objectAddr() const;
};

}  // namespace chre

#include "chre/util/optional_impl.h"

#endif  // UTIL_CHRE_OPTIONAL_H_
