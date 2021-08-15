/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef UTIL_CHRE_NESTED_DATA_PTR_H_
#define UTIL_CHRE_NESTED_DATA_PTR_H_

namespace chre {

/**
 * A template that provides the ability to store data inside of a void pointer
 * to avoid allocating space on the heap in the case where the data is smaller
 * than the size of a void pointer.
 */
template<typename DataType>
union NestedDataPtr {
  NestedDataPtr() {
    static_assert(sizeof(NestedDataPtr<DataType>) == sizeof(void *),
                  "Size of NestedDataPtr must be equal to that of void *");
  }
  void *dataPtr;
  DataType data;
};

}  // namespace chre

#endif  // UTIL_CHRE_NESTED_DATA_PTR_H_