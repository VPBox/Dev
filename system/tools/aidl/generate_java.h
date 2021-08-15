/*
 * Copyright (C) 2016, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "aidl_language.h"
#include "ast_java.h"
#include "io_delegate.h"
#include "options.h"
#include "type_java.h"

#include <string>

namespace android {
namespace aidl {
namespace java {

bool generate_java(const std::string& filename, const AidlDefinedType* iface,
                   java::JavaTypeNamespace* types, const IoDelegate& io_delegate,
                   const Options& options);

android::aidl::java::Class* generate_binder_interface_class(const AidlInterface* iface,
                                                            java::JavaTypeNamespace* types,
                                                            const Options& options);

android::aidl::java::Class* generate_parcel_class(const AidlStructuredParcelable* parcel,
                                                  AidlTypenames& typenames);

std::vector<std::string> generate_java_annotations(const AidlAnnotatable& a);

}  // namespace java
}  // namespace android
}  // namespace aidl
