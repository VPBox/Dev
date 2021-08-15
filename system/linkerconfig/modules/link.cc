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

#include "linkerconfig/link.h"

namespace android {
namespace linkerconfig {
namespace modules {
void Link::WriteConfig(ConfigWriter& writer) {
  writer.SetPrefix("namespace." + origin_namespace_ + ".link." +
                   target_namespace_);
  if (allow_all_shared_libs_) {
    writer.WriteLine(".allow_all_shared_libs = true");
  } else {
    bool is_first = true;

    for (auto& lib_name : shared_libs_) {
      writer.WriteLine(".shared_libs %s %s",
                       is_first ? "=" : "+=", lib_name.c_str());
      is_first = false;
    }
  }
  writer.ResetPrefix();
}
}  // namespace modules
}  // namespace linkerconfig
}  // namespace android