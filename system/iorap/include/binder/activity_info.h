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

#ifndef IORAP_BINDER_ACTIVITY_INFO_H_
#define IORAP_BINDER_ACTIVITY_INFO_H_

#include "binder/common.h"
#include "binder/auto_parcelable.h"
#include "common/introspection.h"

#include <string>

namespace iorap {
namespace binder {

struct ActivityInfo : public AutoParcelable<ActivityInfo> {
  std::string package_name;
  std::string activity_name;
};

IORAP_INTROSPECT_ADAPT_STRUCT(ActivityInfo, package_name, activity_name);

}
}
IORAP_JAVA_NAMESPACE_BINDER_TYPEDEF(ActivityInfo)

#endif  // IORAP_BINDER_ACTIVITY_INFO_H_
