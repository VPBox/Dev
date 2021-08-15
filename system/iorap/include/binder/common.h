/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef IORAP_BINDER_COMMON_H_
#define IORAP_BINDER_COMMON_H_

// AIDL has no way to specify a custom C++ namespace for parcelables.
//
// We want our parcelables to live in the ::iorap::binder namespace,
//   not in com.google.android.startop.iorap
// So this macro is just a short-hand for doing an alias across namespaces.
#define IORAP_JAVA_NAMESPACE_BINDER_TYPEDEF(what) \
namespace com { namespace google { namespace android { namespace startop { namespace iorap { using what = ::iorap::binder::what; } } } } }

#endif  // IORAP_BINDER_COMMON_H_
