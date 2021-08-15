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

#include "include/utility.h"

using RpcLog = wifi_offload::RpcLogRecord::RpcLogRecordType;

namespace wifi_offload_test {

void init(uint8_t &val, RandomGenerator &rand_gen) {
  val = rand_gen.get<uint8_t>();
}

void init(uint16_t &val, RandomGenerator &rand_gen) {
  val = rand_gen.get<uint16_t>();
}

void init(uint32_t &val, RandomGenerator &rand_gen) {
  val = rand_gen.get<uint32_t>();
}

void init(uint64_t &val, RandomGenerator &rand_gen) {
  val = rand_gen.get<uint64_t>();
}

void init(uint8_t *arr, size_t len, RandomGenerator &rand_gen) {
  for (size_t i = 0; i < len; i++) {
    arr[i] = rand_gen.get<uint8_t>();
  }
}

void init_security_mode(uint8_t &sec_mode, RandomGenerator &rand_gen) {
  init(sec_mode, rand_gen);
  sec_mode &= 0x0f;
}

void init_rssi(int8_t &rssi, RandomGenerator &rand_gen) {
  rssi = rand_gen.get<int8_t>();
  if (rssi > 0) {
    rssi = -rssi;
  }
}

void init_rpc_log_record(RpcLog &log_record, RandomGenerator &rand_gen) {
  log_record = static_cast<RpcLog>((rand_gen.get<uint32_t>() % 6u) +
                                   static_cast<uint32_t>(RpcLog::CMD_BASE) + 1);
}

void init_frequency(uint32_t &freq, RandomGenerator &rand_gen) {
  freq = kAllFrequencies_Test[rand_gen.get<uint8_t>() % kNumFrequencies_Test];
}

void init_capability(uint16_t &capa, RandomGenerator &rand_gen) {
  capa = (rand_gen.get<uint16_t>() % 0xffff) + 1;
}

void init(wifi_offload::Ssid &ssid, RandomGenerator &rand_gen) {
  uint8_t rand_ssid[wifi_offload::Ssid::kMaxSsidLen];
  size_t len = (rand_gen.get<uint8_t>() % wifi_offload::Ssid::kMaxSsidLen) + 1;
  init(rand_ssid, len, rand_gen);
  ssid.SetData(rand_ssid, len);
}

void init(wifi_offload::PreferredNetwork &net_info, RandomGenerator &rand_gen) {
  init(net_info.ssid_, rand_gen);
  init_security_mode(net_info.security_modes_, rand_gen);
}

void init(wifi_offload::ScanRecord &record, RandomGenerator &rand_gen) {
  init(record.time_spent_scanning_ms_, rand_gen);
  init(record.num_channels_scanned_, rand_gen);
  init(record.num_entries_aggregated_, rand_gen);
}

void init(wifi_offload::RpcLogRecord &record, RandomGenerator &rand_gen) {
  init_rpc_log_record(record.record_type_, rand_gen);
  init(record.timestamp_chre_ms_, rand_gen);
}

void init(wifi_offload::ChannelHistogram &histo, RandomGenerator &rand_gen) {
  for (size_t i = 0; i < kNumFrequencies_Test; i++) {
    histo.IncrementScanCountForFrequencyForTest(kAllFrequencies_Test[i],
                                                rand_gen.get<uint16_t>());
  }
}

void init(wifi_offload::ScanStats &stats, RandomGenerator &rand_gen) {
  init(stats.num_scans_requested_by_nanoapp_, rand_gen);
  init(stats.num_scans_serviced_by_hardware_, rand_gen);
  init(stats.num_scans_serviced_by_cache_, rand_gen);
  init(stats.updated_at_chre_ms_, rand_gen);
  init(stats.sent_at_chre_ms_, rand_gen);
  init(stats.last_subscription_duration_ms_, rand_gen);
  init(stats.channel_histogram_, rand_gen);
  init<wifi_offload::ScanRecord>(stats.scan_records_, rand_gen);
  init<wifi_offload::RpcLogRecord>(stats.rpc_log_records_, rand_gen);
}

void init(wifi_offload::ScanParams &params, RandomGenerator &rand_gen) {
  init<wifi_offload::Ssid>(params.ssids_to_scan_, rand_gen);
  // Need to init frequency vector with valid frequency values
  // init<uint32_t>(params.frequencies_to_scan_mhz_, rand_gen);
  params.frequencies_to_scan_mhz_.clear();
  size_t vec_len = (rand_gen.get<uint8_t>() % 10) + 1;
  for (size_t i = 0; i < vec_len; i++) {
    uint32_t new_freq;
    init_frequency(new_freq, rand_gen);
    params.frequencies_to_scan_mhz_.push_back(new_freq);
  }
  init(params.disconnected_mode_scan_interval_ms_, rand_gen);
}

void init(wifi_offload::ScanFilter &filter, RandomGenerator &rand_gen) {
  init<wifi_offload::PreferredNetwork>(filter.networks_to_match_, rand_gen);
  init_rssi(filter.min_rssi_threshold_dbm_, rand_gen);
}

void init(wifi_offload::ScanConfig &config, RandomGenerator &rand_gen) {
  init(config.scan_params_, rand_gen);
  init(config.scan_filter_, rand_gen);
}

void init(wifi_offload::ScanResult &result, RandomGenerator &rand_gen) {
  init(result.ssid_, rand_gen);
  init_security_mode(result.security_modes_, rand_gen);
  init(result.bssid_, wifi_offload::ScanResult::kBssidSize, rand_gen);
  init_capability(result.capability_, rand_gen);
  init_frequency(result.frequency_scanned_mhz_, rand_gen);
  init_rssi(result.rssi_dbm_, rand_gen);
  init(result.tsf_, rand_gen);
}

void init(chreWifiScanResult &result, RandomGenerator &rand_gen) {
  init(result.ageMs, rand_gen);
  init_capability(result.capabilityInfo, rand_gen);
  result.ssidLen =
      (rand_gen.get<uint8_t>() % wifi_offload::Ssid::kMaxSsidLen) + 1;
  init(result.ssid, result.ssidLen, rand_gen);
  init(result.bssid, CHRE_WIFI_BSSID_LEN, rand_gen);
  init(result.flags, rand_gen);
  init_rssi(result.rssi, rand_gen);
  init(result.band, rand_gen);
  init_frequency(result.primaryChannel, rand_gen);
  init(result.centerFreqPrimary, rand_gen);
  init(result.centerFreqSecondary, rand_gen);
  result.channelWidth = CHRE_WIFI_CHANNEL_WIDTH_20_MHZ;
  init_security_mode(result.securityMode, rand_gen);
}

}  // wifioffloadtesthelper namespace
