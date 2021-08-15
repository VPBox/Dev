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

#ifndef CHRE_WIFI_OFFLOAD_TEST_UTILITY_H_
#define CHRE_WIFI_OFFLOAD_TEST_UTILITY_H_

#include "chre/apps/wifi_offload/wifi_offload.h"

#include "chre/apps/wifi_offload/channel_histogram.h"
#include "chre/apps/wifi_offload/preferred_network.h"
#include "chre/apps/wifi_offload/rpc_log_record.h"
#include "chre/apps/wifi_offload/scan_config.h"
#include "chre/apps/wifi_offload/scan_filter.h"
#include "chre/apps/wifi_offload/scan_params.h"
#include "chre/apps/wifi_offload/scan_record.h"
#include "chre/apps/wifi_offload/scan_result.h"
#include "chre/apps/wifi_offload/scan_stats.h"

#include "../include/random_generator.h"

namespace wifi_offload_test {

constexpr uint8_t kNumFrequencies_Test = 74;
/* Supported frequencies in 2.4GHz (802.11b/g/n) and 5GHz (802.11a/h/j/n/ac) */
constexpr uint16_t kAllFrequencies_Test[kNumFrequencies_Test] = {
    2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462,
    2467, 2472, 2484, 5035, 5040, 5045, 5055, 5060, 5080, 5170, 5180,
    5190, 5200, 5210, 5220, 5230, 5240, 5250, 5260, 5270, 5280, 5290,
    5300, 5310, 5320, 5500, 5510, 5520, 5530, 5540, 5550, 5560, 5570,
    5580, 5590, 5600, 5610, 5620, 5630, 5640, 5660, 5670, 5680, 5690,
    5700, 5710, 5720, 5745, 5755, 5765, 5775, 5785, 5795, 5805, 5825,
    4915, 4920, 4925, 4935, 4940, 4945, 4960, 4980,
};

/* Supported channel numbers that matches frequencies in
 * kAllFrequencies_Test. 2.4GHz (802.11b/g/n) and 5GHz (802.11a/h/j/n/ac) */
constexpr uint8_t kAllChannels_Test[kNumFrequencies_Test] = {
    1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  7,
    8,   9,   11,  12,  16,  34,  36,  38,  40,  42,  44,  46,  48,  50,  52,
    54,  56,  58,  60,  62,  64,  100, 102, 104, 106, 108, 110, 112, 114, 116,
    118, 120, 122, 124, 126, 128, 132, 134, 136, 138, 140, 142, 144, 149, 151,
    153, 155, 157, 159, 161, 165, 183, 184, 185, 187, 188, 189, 192, 196,
};

void init(uint8_t &val, RandomGenerator &rand_gen);
void init(uint16_t &val, RandomGenerator &rand_gen);
void init(uint32_t &val, RandomGenerator &rand_gen);
void init(uint64_t &val, RandomGenerator &rand_gen);
void init(uint8_t *arr, size_t len, RandomGenerator &rand_gen);
void init(wifi_offload::Ssid &ssid, RandomGenerator &rand_gen);
void init(wifi_offload::PreferredNetwork &net_info, RandomGenerator &rand_gen);
void init(wifi_offload::ScanRecord &record, RandomGenerator &rand_gen);
void init(wifi_offload::RpcLogRecord &record, RandomGenerator &rand_gen);
void init(wifi_offload::ChannelHistogram &histo, RandomGenerator &rand_gen);
void init(wifi_offload::ScanStats &stats, RandomGenerator &rand_gen);
void init(wifi_offload::ScanParams &params, RandomGenerator &rand_gen);
void init(wifi_offload::ScanFilter &filter, RandomGenerator &rand_gen);
void init(wifi_offload::ScanConfig &config, RandomGenerator &rand_gen);
void init(wifi_offload::ScanResult &result, RandomGenerator &rand_gen);
void init(chreWifiScanResult &result, RandomGenerator &rand_gen);

template <typename T>
void init(wifi_offload::Vector<T> &vec, RandomGenerator &rand_gen,
          size_t vec_len) {
  vec.clear();
  for (size_t i = 0; i < vec_len; i++) {
    T new_elem;
    init(new_elem, rand_gen);
    vec.push_back(std::move(new_elem));
  }
}

template <typename T>
void init(wifi_offload::Vector<T> &vec, RandomGenerator &rand_gen) {
  size_t vec_len = (rand_gen.get<uint8_t>() % 10) + 1;
  init(vec, rand_gen, vec_len);
}

}  // wifioffloadtesthelper namespace

#endif  // CHRE_WIFI_OFFLOAD_TEST_UTILITY_H_
