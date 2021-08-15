/*
 * Copyright (C) 2011 The Android Open Source Project
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

/*
 * USAGE NOTE: Only include this header when _implementing_ a particular
 * effect. When access to UUID and properties is enough, include the
 * corresponding header from system/audio_effects/, which doesn't include
 * hardware/audio_effect.h.
 *
 * Only code that immediately calls into HAL or implements an effect
 * can import hardware/audio_effect.h.
 */

#ifndef ANDROID_EFFECT_EQUALIZER_H_
#define ANDROID_EFFECT_EQUALIZER_H_

#include <hardware/audio_effect.h>
#include <system/audio_effects/effect_equalizer.h>

#endif /*ANDROID_EFFECT_EQUALIZER_H_*/
