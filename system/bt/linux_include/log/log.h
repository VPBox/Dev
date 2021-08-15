/******************************************************************************
 *
 *  Copyright 2018 The Android Open Source Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#pragma once

/* This file provides empty implementation of android_errorWriteLog, which is
 * not required on linux. It should be on include path only for linux build. */

#if defined(OS_GENERIC)

#include <cstdint>

inline int android_errorWriteLog(int, const char*) { return 0; };
inline int android_errorWriteWithInfoLog(int tag, const char* subTag,
                                         int32_t uid, const char* data,
                                         uint32_t dataLen) {
  return 0;
};
#endif
