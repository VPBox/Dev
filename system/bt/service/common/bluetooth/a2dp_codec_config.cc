//
//  Copyright (C) 2017 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "bluetooth/a2dp_codec_config.h"

namespace bluetooth {

A2dpCodecConfig::A2dpCodecConfig() = default;
A2dpCodecConfig::A2dpCodecConfig(const A2dpCodecConfig& other) = default;
A2dpCodecConfig::A2dpCodecConfig(int codec_type, int codec_priority,
                                 int sample_rate, int bits_per_sample,
                                 int channel_mode, int64_t codec_specific_1,
                                 int64_t codec_specific_2,
                                 int64_t codec_specific_3,
                                 int64_t codec_specific_4)
    : codec_type_(codec_type),
      codec_priority_(codec_priority),
      sample_rate_(sample_rate),
      bits_per_sample_(bits_per_sample),
      channel_mode_(channel_mode),
      codec_specific_1_(codec_specific_1),
      codec_specific_2_(codec_specific_2),
      codec_specific_3_(codec_specific_3),
      codec_specific_4_(codec_specific_4) {}

A2dpCodecConfig::~A2dpCodecConfig() = default;

}  // namespace bluetooth
