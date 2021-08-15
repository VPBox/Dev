// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "inode2filename/inode.h"

#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>

#include <string>
#include <vector>

#include <sys/sysmacros.h>

using android::base::ParseUint;

namespace iorap::inode2filename {

// TODO: refactor to return expected<Inode, string>
bool Inode::Parse(const std::string& str, Inode* out, std::string* error_msg) {
  DCHECK(out != nullptr);
  DCHECK(error_msg != nullptr);

  // Major:minor:inode OR dev_t@inode
  std::vector<std::string> lst_pair = android::base::Split(str, "@");
  if (lst_pair.size() == 2) {
    size_t dev_whole = 0;
    if (!ParseUint(lst_pair[0], &dev_whole)) {
      *error_msg = "Failed to parse the whole device id as uint.";
      return false;
    }

    dev_t dev_w = static_cast<dev_t>(dev_whole);
    out->device_major = major(dev_w);
    out->device_minor = minor(dev_w);

    if (!ParseUint(lst_pair[1], &out->inode)) {
      *error_msg = "Failed to parse inode as uint.";
      return false;
    }

    return true;
  }

  std::vector<std::string> lst = android::base::Split(str, ":");

  if (lst.size() != 3) {
    *error_msg = "Too few : separated items";
    return false;
  }

  if (!ParseUint(lst[0], &out->device_major)) {
    *error_msg = "Failed to parse 0th element as a uint";
    return false;
  }

  if (!ParseUint(lst[1], &out->device_minor)) {
    *error_msg = "Failed to parse 1st element as a uint";
    return false;
  }

  if (!ParseUint(lst[2], &out->inode)) {
    *error_msg = "Failed to parse 2nd element as a uint";
    return false;
  }

  return true;
}

}  // namespace iorap::inode2filename
