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

#include "service/a2dp_source.h"

#include "base/logging.h"
#include "base/memory/ptr_util.h"
#include "service/logging_helpers.h"

#define PARSE_ADDR(str)                                        \
  ({                                                           \
    RawAddress tmp;                                            \
    if (!RawAddress::FromString((str), tmp)) {                 \
      LOG(ERROR) << "Invalid device address given: " << (str); \
      return false;                                            \
    }                                                          \
    tmp;                                                       \
  })

#define TRY_RET(expr, err_msg) \
  do {                         \
    if (!(expr)) {             \
      LOG(ERROR) << err_msg;   \
      return false;            \
    }                          \
    return true;               \
  } while (0)

#define TRY_RET_FUNC(expr) TRY_RET(expr, __func__ << " failed")

using bluetooth::hal::BluetoothAvInterface;
using LockGuard = std::lock_guard<std::mutex>;

namespace bluetooth {

namespace {

btav_a2dp_codec_config_t CodecConfigToFluoride(const A2dpCodecConfig& config) {
  btav_a2dp_codec_config_t ret = {
      .codec_type = static_cast<btav_a2dp_codec_index_t>(config.codec_type()),
      .codec_priority =
          static_cast<btav_a2dp_codec_priority_t>(config.codec_priority()),
      .sample_rate =
          static_cast<btav_a2dp_codec_sample_rate_t>(config.sample_rate()),
      .bits_per_sample = static_cast<btav_a2dp_codec_bits_per_sample_t>(
          config.bits_per_sample()),
      .channel_mode =
          static_cast<btav_a2dp_codec_channel_mode_t>(config.channel_mode()),
      .codec_specific_1 = config.codec_specific_1(),
      .codec_specific_2 = config.codec_specific_2(),
      .codec_specific_3 = config.codec_specific_3(),
      .codec_specific_4 = config.codec_specific_4(),
  };

  return ret;
}

std::vector<btav_a2dp_codec_config_t> CodecConfigsToFluoride(
    const std::vector<A2dpCodecConfig>& configs) {
  std::vector<btav_a2dp_codec_config_t> ret;
  ret.reserve(configs.size());
  for (const auto& config : configs) {
    ret.push_back(CodecConfigToFluoride(config));
  }

  return ret;
}
A2dpCodecConfig FluorideCodecToCodec(const btav_a2dp_codec_config_t& config) {
  A2dpCodecConfig ret(config.codec_type, config.codec_priority,
                      config.sample_rate, config.bits_per_sample,
                      config.channel_mode, config.codec_specific_1,
                      config.codec_specific_2, config.codec_specific_3,
                      config.codec_specific_4);

  return ret;
}

std::vector<A2dpCodecConfig> FluorideCodecsToCodec(
    const std::vector<btav_a2dp_codec_config_t>& configs) {
  std::vector<A2dpCodecConfig> ret;
  ret.reserve(configs.size());
  for (const auto& config : configs) {
    ret.push_back(FluorideCodecToCodec(config));
  }

  return ret;
}

}  // namespace

// static
const int A2dpSource::kSingletonInstanceId = 0;

A2dpSource::A2dpSource(const Uuid& uuid) : app_identifier_(uuid) {
  hal::BluetoothAvInterface::Get()->AddA2dpSourceObserver(this);
}

A2dpSource::~A2dpSource() {
  hal::BluetoothAvInterface::Get()->RemoveA2dpSourceObserver(this);
}

const Uuid& A2dpSource::GetAppIdentifier() const { return app_identifier_; }

int A2dpSource::GetInstanceId() const { return kSingletonInstanceId; }

void A2dpSource::SetDelegate(Delegate* delegate) {
  LockGuard lock(delegate_mutex_);
  delegate_ = delegate;
}

bool A2dpSource::Enable(const std::vector<A2dpCodecConfig>& codec_priorities) {
  auto fluoride_priorities = CodecConfigsToFluoride(codec_priorities);
  LockGuard lock(mutex_);
  return hal::BluetoothAvInterface::Get()->A2dpSourceEnable(
      fluoride_priorities);
}

void A2dpSource::Disable() {
  LockGuard lock(mutex_);
  hal::BluetoothAvInterface::Get()->A2dpSourceDisable();
}

bool A2dpSource::Connect(const std::string& device_address) {
  RawAddress addr = PARSE_ADDR(device_address);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(
      hal::BluetoothAvInterface::Get()->GetA2dpSourceHALInterface()->connect(
          addr) == BT_STATUS_SUCCESS);
}

bool A2dpSource::Disconnect(const std::string& device_address) {
  RawAddress addr = PARSE_ADDR(device_address);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(
      hal::BluetoothAvInterface::Get()->GetA2dpSourceHALInterface()->disconnect(
          addr) == BT_STATUS_SUCCESS);
}

bool A2dpSource::ConfigCodec(
    const std::string& device_address,
    const std::vector<A2dpCodecConfig>& codec_preferences) {
  RawAddress addr = PARSE_ADDR(device_address);
  auto fluoride_preferences = CodecConfigsToFluoride(codec_preferences);
  LockGuard lock(mutex_);
  TRY_RET_FUNC(hal::BluetoothAvInterface::Get()
                   ->GetA2dpSourceHALInterface()
                   ->config_codec(addr, fluoride_preferences) ==
               BT_STATUS_SUCCESS);
}

void A2dpSource::ConnectionStateCallback(BluetoothAvInterface* iface,
                                         const RawAddress& bd_addr,
                                         btav_connection_state_t state) {
  auto device_address = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnConnectionState(device_address, static_cast<int>(state));
}

void A2dpSource::AudioStateCallback(BluetoothAvInterface* iface,
                                    const RawAddress& bd_addr,
                                    btav_audio_state_t state) {
  auto device_address = BtAddrString(&bd_addr);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnAudioState(device_address, static_cast<int>(state));
}

void A2dpSource::AudioConfigCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    const btav_a2dp_codec_config_t& codec_config_fluoride,
    const std::vector<btav_a2dp_codec_config_t>
        codecs_local_capabilities_fluoride,
    const std::vector<btav_a2dp_codec_config_t>
        codecs_selectable_capabilities_fluoride) {
  auto device_address = BtAddrString(&bd_addr);
  auto codec_config = FluorideCodecToCodec(codec_config_fluoride);
  auto codecs_local_capabilities =
      FluorideCodecsToCodec(codecs_local_capabilities_fluoride);
  auto codecs_selectable_capabilities =
      FluorideCodecsToCodec(codecs_selectable_capabilities_fluoride);
  LockGuard lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnAudioConfig(device_address, codec_config,
                             codecs_local_capabilities,
                             codecs_selectable_capabilities);
}

// A2dpSourceFactory implementation
// ========================================================
A2dpSourceFactory::A2dpSourceFactory() = default;
A2dpSourceFactory::~A2dpSourceFactory() = default;

bool A2dpSourceFactory::RegisterInstance(const Uuid& uuid,
                                         const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - UUID: " << uuid.ToString();

  auto a2dp_source = base::WrapUnique(new A2dpSource(uuid));
  callback(BLE_STATUS_SUCCESS, uuid, std::move(a2dp_source));
  return true;
}

}  // namespace bluetooth
