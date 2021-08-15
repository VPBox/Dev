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

#ifndef NETDUTILS_NETWORKCONSTANTS_H_
#define NETDUTILS_NETWORKCONSTANTS_H_

namespace android {
namespace netdutils {

// See also NetworkConstants.java in frameworks/base.
constexpr int IPV4_ADDR_LEN = 4;
constexpr int IPV4_ADDR_BITS = 32;
constexpr int IPV6_ADDR_LEN = 16;
constexpr int IPV6_ADDR_BITS = 128;

}  // namespace netdutils
}  // namespace android

#endif  // NETDUTILS_NETWORKCONSTANTS_H_
