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

#pragma once

#include <cstdint>

namespace bluetooth {

// Should match btav_a2dp_codec_config_t in bt_av.h
class A2dpCodecConfig {
 public:
  A2dpCodecConfig();
  A2dpCodecConfig(const A2dpCodecConfig& other);
  A2dpCodecConfig(int codec_type, int codec_priority, int sample_rate,
                  int bits_per_sample, int channel_mode,
                  int64_t codec_specific_1, int64_t codec_specific_2,
                  int64_t codec_specific_3, int64_t codec_specific_4);
  ~A2dpCodecConfig();

  int codec_type() const { return codec_type_; }
  int codec_priority() const { return codec_priority_; }
  int sample_rate() const { return sample_rate_; }
  int bits_per_sample() const { return bits_per_sample_; }
  int channel_mode() const { return channel_mode_; }
  int64_t codec_specific_1() const { return codec_specific_1_; }
  int64_t codec_specific_2() const { return codec_specific_2_; }
  int64_t codec_specific_3() const { return codec_specific_3_; }
  int64_t codec_specific_4() const { return codec_specific_4_; }

 protected:
  int codec_type_ = 0;
  int codec_priority_ = 0;
  int sample_rate_ = 0;
  int bits_per_sample_ = 0;
  int channel_mode_ = 0;
  int64_t codec_specific_1_ = 0;
  int64_t codec_specific_2_ = 0;
  int64_t codec_specific_3_ = 0;
  int64_t codec_specific_4_ = 0;
};

}  // namespace bluetooth
