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

#ifndef IORAP_SRC_INODE2FILENAME_INODE_H_
#define IORAP_SRC_INODE2FILENAME_INODE_H_

#include <functional>
#include <ostream>
#include <string>

#include <stddef.h>

namespace iorap::inode2filename {

struct Inode {
  size_t device_major;  // dev_t = makedev(major, minor)
  size_t device_minor;
  size_t inode;         // ino_t = inode

  static bool Parse(const std::string& str, /*out*/Inode* out, /*out*/std::string* error_msg);

  bool operator==(const Inode& rhs) const {
    return device_major == rhs.device_major &&
        device_minor == rhs.device_minor &&
        inode == rhs.inode;
  }

  bool operator!=(const Inode& rhs) const {
    return !(*this == rhs);
  }
};

inline std::ostream& operator<<(std::ostream& os, const Inode& inode) {
  os << inode.device_major << ":" << inode.device_minor << ":" << inode.inode;
  return os;
}

}  // namespace iorap::inode2filename

namespace std {
  template <>
  struct hash<iorap::inode2filename::Inode> {
      using argument_type = iorap::inode2filename::Inode;
      using result_type = size_t;
      result_type operator()(argument_type const& s) const noexcept {
        // Hash the inode by using only the inode#. Ignore devices, we are extremely unlikely
        // to ever collide on the devices.
        result_type const h1 = std::hash<size_t>{}(s.inode);
        return h1;
      }
  };
}  // namespace std

#endif  // IORAP_SRC_INODE2FILENAME_INODE_H_
