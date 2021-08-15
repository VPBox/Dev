//
//  Copyright 2017 Google, Inc.
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

#include "service/hal/bluetooth_av_interface.h"

#include <shared_mutex>

#include <base/logging.h>
#include <base/memory/ptr_util.h>
#include <base/observer_list.h>

#include "service/hal/bluetooth_interface.h"

namespace bluetooth {
namespace hal {

namespace {

BluetoothAvInterface* g_interface = nullptr;

#if defined(OS_GENERIC) && defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION < 3500)
using shared_mutex_impl = std::shared_mutex;
#else
using shared_mutex_impl = std::shared_timed_mutex;
#endif

// Mutex used by callbacks to access |g_interface|. If we initialize or clean it
// use unique_lock. If only accessing |g_interface| use shared lock.
shared_mutex_impl g_instance_lock;

base::ObserverList<BluetoothAvInterface::A2dpSourceObserver>*
GetA2dpSourceObservers();
base::ObserverList<BluetoothAvInterface::A2dpSinkObserver>*
GetA2dpSinkObservers();

#define VERIFY_INTERFACE_OR_RETURN()                                   \
  do {                                                                 \
    if (!g_interface) {                                                \
      LOG(WARNING) << "Callback received while |g_interface| is NULL"; \
      return;                                                          \
    }                                                                  \
  } while (0)

}  // namespace

void SourceConnectionStateCallback(const RawAddress& bd_addr,
                                   btav_connection_state_t state) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();

  for (auto& observer : *GetA2dpSourceObservers()) {
    observer.ConnectionStateCallback(g_interface, bd_addr, state);
  }
}

void SourceAudioStateCallback(const RawAddress& bd_addr,
                              btav_audio_state_t state) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetA2dpSourceObservers()) {
    observer.AudioStateCallback(g_interface, bd_addr, state);
  }
}

void SourceAudioConfigCallback(
    const RawAddress& bd_addr, btav_a2dp_codec_config_t codec_config,
    std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities,
    std::vector<btav_a2dp_codec_config_t> codecs_selectable_capabilities) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetA2dpSourceObservers()) {
    observer.AudioConfigCallback(g_interface, bd_addr, codec_config,
                                 codecs_local_capabilities,
                                 codecs_selectable_capabilities);
  }
}

void SinkConnectionStateCallback(const RawAddress& bd_addr,
                                 btav_connection_state_t state) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetA2dpSinkObservers()) {
    observer.ConnectionStateCallback(g_interface, bd_addr, state);
  }
}

void SinkAudioStateCallback(const RawAddress& bd_addr,
                            btav_audio_state_t state) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetA2dpSinkObservers()) {
    observer.AudioStateCallback(g_interface, bd_addr, state);
  }
}

void SinkAudioConfigCallback(const RawAddress& bd_addr, uint32_t sample_rate,
                             uint8_t channel_count) {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  VERIFY_INTERFACE_OR_RETURN();
  for (auto& observer : *GetA2dpSinkObservers()) {
    observer.AudioConfigCallback(g_interface, bd_addr, sample_rate,
                                 channel_count);
  }
}

btav_source_callbacks_t av_source_callbacks = {
    .size = sizeof(btav_source_callbacks_t),
    .connection_state_cb = SourceConnectionStateCallback,
    .audio_state_cb = SourceAudioStateCallback,
    .audio_config_cb = SourceAudioConfigCallback,
};

btav_sink_callbacks_t av_sink_callbacks = {
    .size = sizeof(btav_sink_callbacks_t),
    .connection_state_cb = SinkConnectionStateCallback,
    .audio_state_cb = SinkAudioStateCallback,
    .audio_config_cb = SinkAudioConfigCallback,
};

class BluetoothAvInterfaceImpl : public BluetoothAvInterface {
 public:
  BluetoothAvInterfaceImpl() = default;
  ~BluetoothAvInterfaceImpl() override {
    A2dpSinkDisable();
    A2dpSourceDisable();
  }

  bool A2dpSourceEnable(
      std::vector<btav_a2dp_codec_config_t> codec_priorities) override {
    if (source_enabled_) {
      return true;
    }

    // Right now we only support one connected audio device.
    int max_connected_audio_devices = 1;
    if (hal_source_iface_->init(
            &av_source_callbacks, max_connected_audio_devices,
            std::move(codec_priorities)) != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to initialize HAL A2DP source interface";
      return false;
    }
    source_enabled_ = true;
    return true;
  }

  void A2dpSourceDisable() override {
    if (!source_enabled_) {
      return;
    }

    hal_source_iface_->cleanup();
    source_enabled_ = false;
  }

  bool A2dpSinkEnable() override {
    if (sink_enabled_) {
      return true;
    }
    if (hal_sink_iface_->init(&av_sink_callbacks) != BT_STATUS_SUCCESS) {
      LOG(ERROR) << "Failed to initialize HAL A2DP sink interface";
      return false;
    }
    sink_enabled_ = true;
    return true;
  }

  void A2dpSinkDisable() override {
    if (!sink_enabled_) {
      return;
    }
    hal_sink_iface_->cleanup();
    sink_enabled_ = false;
  }

  void AddA2dpSourceObserver(A2dpSourceObserver* observer) override {
    a2dp_source_observers_.AddObserver(observer);
  }

  void RemoveA2dpSourceObserver(A2dpSourceObserver* observer) override {
    a2dp_source_observers_.RemoveObserver(observer);
  }

  void AddA2dpSinkObserver(A2dpSinkObserver* observer) override {
    a2dp_sink_observers_.AddObserver(observer);
  }

  void RemoveA2dpSinkObserver(A2dpSinkObserver* observer) override {
    a2dp_sink_observers_.RemoveObserver(observer);
  }

  const btav_source_interface_t* GetA2dpSourceHALInterface() override {
    return hal_source_iface_;
  }

  const btav_sink_interface_t* GetA2dpSinkHALInterface() override {
    return hal_sink_iface_;
  }

  bool Initialize() {
    const bt_interface_t* bt_iface =
        BluetoothInterface::Get()->GetHALInterface();
    CHECK(bt_iface);

    const auto* hal_source_iface =
        reinterpret_cast<const btav_source_interface_t*>(
            bt_iface->get_profile_interface(BT_PROFILE_ADVANCED_AUDIO_ID));
    if (!hal_source_iface) {
      LOG(ERROR) << "Failed to obtain A2DP source interface handle";
      return false;
    }

    const auto* hal_sink_iface = reinterpret_cast<const btav_sink_interface_t*>(
        bt_iface->get_profile_interface(BT_PROFILE_ADVANCED_AUDIO_SINK_ID));
    if (!hal_sink_iface) {
      LOG(ERROR) << "Failed to obtain A2DP sink interface handle";
      return false;
    }

    hal_sink_iface_ = hal_sink_iface;
    hal_source_iface_ = hal_source_iface;

    // Only initialize the sink interface.
    return A2dpSinkEnable();
  }

  base::ObserverList<A2dpSourceObserver>* source_observers() {
    return &a2dp_source_observers_;
  }

  base::ObserverList<A2dpSinkObserver>* sink_observers() {
    return &a2dp_sink_observers_;
  }

 private:
  base::ObserverList<A2dpSourceObserver> a2dp_source_observers_;
  base::ObserverList<A2dpSinkObserver> a2dp_sink_observers_;

  const btav_source_interface_t* hal_source_iface_ = nullptr;
  const btav_sink_interface_t* hal_sink_iface_ = nullptr;

  bool source_enabled_ = false;
  bool sink_enabled_ = false;

  DISALLOW_COPY_AND_ASSIGN(BluetoothAvInterfaceImpl);
};

namespace {

base::ObserverList<BluetoothAvInterface::A2dpSourceObserver>*
GetA2dpSourceObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothAvInterfaceImpl*>(g_interface)
      ->source_observers();
}

base::ObserverList<BluetoothAvInterface::A2dpSinkObserver>*
GetA2dpSinkObservers() {
  CHECK(g_interface);
  return static_cast<BluetoothAvInterfaceImpl*>(g_interface)->sink_observers();
}

}  // namespace

void BluetoothAvInterface::A2dpSourceObserver::ConnectionStateCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    btav_connection_state_t state) {
  // Do nothing.
}

void BluetoothAvInterface::A2dpSourceObserver::AudioStateCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    btav_audio_state_t state) {
  // Do nothing.
}

void BluetoothAvInterface::A2dpSourceObserver::AudioConfigCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    const btav_a2dp_codec_config_t& codec_config,
    const std::vector<btav_a2dp_codec_config_t> codecs_local_capabilities,
    const std::vector<btav_a2dp_codec_config_t>
        codecs_selectable_capabilities) {
  // Do nothing.
}

void BluetoothAvInterface::A2dpSinkObserver::ConnectionStateCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    btav_connection_state_t state) {
  // Do nothing.
}

void BluetoothAvInterface::A2dpSinkObserver::AudioStateCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    btav_audio_state_t state) {
  // Do nothing.
}

void BluetoothAvInterface::A2dpSinkObserver::AudioConfigCallback(
    BluetoothAvInterface* iface, const RawAddress& bd_addr,
    uint32_t sample_rate, uint8_t channel_count) {
  // Do nothing.
}

// static
bool BluetoothAvInterface::Initialize() {
  std::unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(!g_interface);

  auto impl = std::make_unique<BluetoothAvInterfaceImpl>();
  if (!impl->Initialize()) {
    LOG(ERROR) << "Failed to initialize BluetoothAvInterface";
    return false;
  }

  g_interface = impl.release();
  return true;
}

// static
void BluetoothAvInterface::CleanUp() {
  std::unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);

  delete g_interface;
  g_interface = nullptr;
}

// static
bool BluetoothAvInterface::IsInitialized() {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  return g_interface != nullptr;
}

// static
void BluetoothAvInterface::InitializeForTesting(
    BluetoothAvInterface* test_instance) {
  std::unique_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(test_instance);
  CHECK(!g_interface);

  g_interface = test_instance;
}

// static
BluetoothAvInterface* BluetoothAvInterface::Get() {
  std::shared_lock<shared_mutex_impl> lock(g_instance_lock);
  CHECK(g_interface);
  return g_interface;
}

}  // namespace hal
}  // namespace bluetooth
