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

#ifndef NETDUTILS_UID_CONSTANTS_H
#define NETDUTILS_UID_CONSTANTS_H

// These are used by both eBPF kernel programs and netd, we cannot put them in NetdConstant.h since
// we have to minimize the number of headers included by the BPF kernel program.
#define MIN_SYSTEM_UID 0
#define MAX_SYSTEM_UID 9999

#define PER_USER_RANGE 100000

#endif  // NETDUTILS_UID_CONSTANTS_H
