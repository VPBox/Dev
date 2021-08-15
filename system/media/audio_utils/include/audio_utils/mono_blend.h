/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_MONO_BLEND_H
#define ANDROID_AUDIO_MONO_BLEND_H
#include <stdint.h>
#include <sys/cdefs.h>
#include <system/audio.h>

/** \cond */
__BEGIN_DECLS
/** \endcond */

// for in-place conversion routines

/**
 * In-place mono blend using the arithmetic average of the channels in each audio frame.
 *
 *   \param buf          buffer of frames
 *   \param format       one of AUDIO_FORMAT_PCM_16_BIT or AUDIO_FORMAT_PCM_FLOAT
 *   \param channelCount number of channels per frame
 *   \param frames       number of frames in buffer
 *   \param limit        whether to use a limiter (experimental, currently only for stereo float)
 *
 * \return
 *   none
 *
 */

void mono_blend(void *buf, audio_format_t format, size_t channelCount, size_t frames,
        bool limit = false);

/** \cond */
__END_DECLS
/** \endcond */

#endif // !ANDROID_AUDIO_MONO_BLEND_H
