/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef HIDL_TYPE_ASSERTIONS_H_

#define HIDL_TYPE_ASSERTIONS_H_

#include <android-base/macros.h>

namespace android {

struct Formatter;

// Declare a HidlTypeAssertion at static scope to enforce a size requirement
// on a type (assumed to be declared in namespace ::android::hardware).
// This will cause the C++ backend of hidl-gen to emit the appropriate
// static_assert(...) statements at the end of FooAll.cpp.
struct HidlTypeAssertion {
    HidlTypeAssertion(const char *name, size_t size);

    size_t size() const;

    static void EmitAll(Formatter &out);

private:
    size_t mSize;

    DISALLOW_COPY_AND_ASSIGN(HidlTypeAssertion);
};

}  // namespace android

#endif  // HIDL_TYPE_ASSERTIONS_H_
