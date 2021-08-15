/******************************************************************************
 *
 *  Copyright 2015 Google, Inc.
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

#include <cstdint>

namespace bluetooth {

namespace common {

// Get the OS boot time in milliseconds.
uint64_t time_get_os_boottime_ms();

// Get the OS boot time in microseconds.
uint64_t time_get_os_boottime_us();

// Get the current wall clock time in microseconds.
uint64_t time_gettimeofday_us();

}  // namespace common

}  // namespace bluetooth
