/*
**
** Copyright 2018, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_IO_H_
#define SYSTEM_EXTRAS_PERFPROFD_PERFPROFD_IO_H_

#include <android-base/unique_fd.h>

#include "perfprofd_record-fwd.h"

namespace android {
namespace perfprofd {

bool SerializeProtobuf(android::perfprofd::PerfprofdRecord* encodedProfile,
                       const char* encoded_file_path,
                       bool compress = true);
bool SerializeProtobuf(android::perfprofd::PerfprofdRecord* encodedProfile,
                       android::base::unique_fd&& fd,
                       bool compress = true);

}  // namespace perfprofd
}  // namespace android

#endif
