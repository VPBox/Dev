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

#ifndef ANDROID_EFFECT_DYNAMICSPROCESSING_CORE_H_
#define ANDROID_EFFECT_DYNAMICSPROCESSING_CORE_H_

#include <system/audio_effect.h>

#if __cplusplus
extern "C" {
#endif

#ifndef OPENSL_ES_H_
static const effect_uuid_t SL_IID_DYNAMICSPROCESSING_ = { 0x7261676f, 0x6d75, 0x7369, 0x6364,
        { 0x28, 0xe2, 0xfd, 0x3a, 0xc3, 0x9e } };
const effect_uuid_t * const SL_IID_DYNAMICSPROCESSING = &SL_IID_DYNAMICSPROCESSING_;
#endif //OPENSL_ES_H_

/* enumerated parameters for dynamics processing effect */
typedef enum
{
    DP_PARAM_GET_CHANNEL_COUNT = 0x10,
    DP_PARAM_INPUT_GAIN = 0x20,
    DP_PARAM_ENGINE_ARCHITECTURE = 0x30,
    DP_PARAM_PRE_EQ = 0x40,
    DP_PARAM_PRE_EQ_BAND = 0x45,
    DP_PARAM_MBC = 0x50,
    DP_PARAM_MBC_BAND = 0x55,
    DP_PARAM_POST_EQ = 0x60,
    DP_PARAM_POST_EQ_BAND = 0x65,
    DP_PARAM_LIMITER = 0x70,
} t_dynamicsprocessing_params;

/* enumerated variants */
typedef enum
{
    VARIANT_FAVOR_FREQUENCY_RESOLUTION = 0x00,
    VARIANT_FAVOR_TIME_RESOLUTION = 0x01,
} t_dynamicsprocessing_variants;

#if __cplusplus
} // extern "C"
#endif


#endif /*ANDROID_EFFECT_DYNAMICSPROCESSING_CORE_H_*/
