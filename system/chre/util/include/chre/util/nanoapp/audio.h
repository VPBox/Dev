/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_UTIL_NANOAPP_AUDIO_H_
#define CHRE_UTIL_NANOAPP_AUDIO_H_

#include <chre/audio.h>
#include <cstdint>

namespace chre {

/**
 * Returns a string description of a given audio format.
 *
 * @param format the format to obtain a string for.
 * @return a pointer to the string or some indication of invalid.
 */
const char *getChreAudioFormatString(uint8_t format);

}  // namespace chre

#endif  // CHRE_UTIL_NANOAPP_AUDIO_H_
