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

#ifndef CHRE_UTIL_SINGLETON_H_
#define CHRE_UTIL_SINGLETON_H_

#include <type_traits>

#include "chre/util/non_copyable.h"

namespace chre {

/**
 * The Singleton template provides static storage for one instance of the
 * provided type. Initialization does not happen automatically which allows
 * users of this API to control the order of initialization.
 *
 * Caution is recommended when using this class to avoid Singleton hell. In
 * many cases there is a better solution than using a Singleton in your design.
 * One good use of this class is for a class that owns the state of your
 * application (the "root" of a tree of object ownership).
 */
template<typename ObjectType>
class Singleton : public NonCopyable {
 public:
  /**
   * Constructs the object in the space provided by this container. If the
   * object is already constructed, no operation is performed. Use the
   * isInitialized method to determine if construction is required.
   *
   * @param args The constructor arguments to pass to the singleton instance.
   */
  template<typename... Args>
  static void init(Args&&... args);

  /**
   * Invokes the destructor on the underlying object if it has been constructed
   * already.
   */
  static void deinit();

  /**
   * Returns whether or not this singleton instance has been constructed.
   *
   * @return Returns true if the object has already been constructed.
   */
  static bool isInitialized();

  /**
   * Returns a pointer to the underlying object. The singleton object must be
   * constructed prior to using get to interact with it. If the object is not
   * initialized, the behavior is undefined and the returned pointer is not
   * initialized.
   *
   * @return A pointer to the singleton instance.
   */
  static ObjectType *get();

  /**
   * Returns a pointer to the underlying object. The singleton object must be
   * constructed prior to using get to interact with it. If the object is not
   * initialized, nullptr is returned.
   *
   * @return A pointer to the singleton instance or nullptr if it is not
   * initialized.
   */
  static ObjectType *safeGet();

 private:
  //! Static storage for the type of this singleton.
  static typename std::aligned_storage<
      sizeof(ObjectType), alignof(ObjectType)>::type sObject;

  //! Static storage for the initialized state of this singleton.
  static bool sIsInitialized;
};

}  // namespace chre

#include "chre/util/singleton_impl.h"

#endif  // CHRE_UTIL_SINGLETON_H_
