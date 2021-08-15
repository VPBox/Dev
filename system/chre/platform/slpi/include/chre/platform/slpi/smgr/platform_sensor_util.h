/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_SMGR_PLATFORM_SENSOR_UTIL_H_
#define CHRE_PLATFORM_SLPI_SMGR_PLATFORM_SENSOR_UTIL_H_

#include <cstdint>

#include "chre/util/time.h"

namespace chre {

/**
 * Converts an interval in nanoseconds to an SMGR report rate. SMGR overloads
 * the value of a uint16_t to have an inversion point at which the value is no
 * longer a sample rate and becomes an interval for sub-zero sampling rates.
 * This function handles that conversion.
 *
 * @param interval The sampling interval to convert to a report rate.
 * @return Returns the SMGR report rate for this interval.
 */
uint16_t intervalToSmgrSamplingRate(Nanoseconds interval);

/**
 * Converts an interval in nanoseconds to an SMGR report rate in Hz and Q16
 * format.
 *
 * @param interval The interval to convert to a report rate.
 * @return Returns the SMGR uint32_t report rate for this interval.
 */
uint32_t intervalToSmgrQ16ReportRate(Nanoseconds interval);

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SMGR_PLATFORM_SENSOR_UTIL_H_
