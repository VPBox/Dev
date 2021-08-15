/*
 * Copyright 2019 The Android Open Source Project
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

#define LOG_TAG "BTAudioClientIf"

#include "client_interface.h"

#include <android/hardware/bluetooth/audio/2.0/IBluetoothAudioPort.h>
#include <android/hardware/bluetooth/audio/2.0/IBluetoothAudioProvidersFactory.h>
#include <android/hidl/manager/1.2/IServiceManager.h>
#include <base/logging.h>
#include <hidl/MQDescriptor.h>
#include <hidl/ServiceManagement.h>
#include <future>

#include "osi/include/log.h"

namespace bluetooth {
namespace audio {

using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::audio::common::V5_0::SourceMetadata;
using ::android::hardware::bluetooth::audio::V2_0::IBluetoothAudioPort;
using ::android::hardware::bluetooth::audio::V2_0::
    IBluetoothAudioProvidersFactory;
using DataMQ = ::android::hardware::MessageQueue<
    uint8_t, ::android::hardware::kSynchronizedReadWrite>;

static constexpr int kDefaultDataReadTimeoutMs = 10;      // 10 ms
static constexpr int kDefaultDataReadPollIntervalMs = 1;  // non-blocking poll
static constexpr char kFullyQualifiedInterfaceName[] =
    "android.hardware.bluetooth.audio@2.0::IBluetoothAudioProvidersFactory";

std::ostream& operator<<(std::ostream& os, const BluetoothAudioCtrlAck& ack) {
  switch (ack) {
    case BluetoothAudioCtrlAck::SUCCESS_FINISHED:
      return os << "SUCCESS_FINISHED";
    case BluetoothAudioCtrlAck::PENDING:
      return os << "PENDING";
    case BluetoothAudioCtrlAck::FAILURE_UNSUPPORTED:
      return os << "FAILURE_UNSUPPORTED";
    case BluetoothAudioCtrlAck::FAILURE_BUSY:
      return os << "FAILURE_BUSY";
    case BluetoothAudioCtrlAck::FAILURE_DISCONNECTING:
      return os << "FAILURE_DISCONNECTING";
    case BluetoothAudioCtrlAck::FAILURE:
      return os << "FAILURE";
    default:
      return os << "UNDEFINED " << static_cast<int8_t>(ack);
  }
}

// Internal class within BluetoothAudioClientInterfaceace to implement
// IBluetoothAudioPort (control interface used by Bluetooth Audio HAL)
class BluetoothAudioPortImpl : public IBluetoothAudioPort {
 public:
  BluetoothAudioPortImpl(IBluetoothTransportInstance* sink,
                         const android::sp<IBluetoothAudioProvider>& provider)
      : sink_(sink), provider_(provider){};

  Return<void> startStream() {
    BluetoothAudioCtrlAck ack = sink_->StartRequest();
    if (ack != BluetoothAudioCtrlAck::PENDING) {
      auto hidl_retval =
          provider_->streamStarted(BluetoothAudioCtrlAckToHalStatus(ack));
      if (!hidl_retval.isOk()) {
        LOG(ERROR) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
      }
    }
    return Void();
  }

  Return<void> suspendStream() {
    BluetoothAudioCtrlAck ack = sink_->SuspendRequest();
    if (ack != BluetoothAudioCtrlAck::PENDING) {
      auto hidl_retval =
          provider_->streamSuspended(BluetoothAudioCtrlAckToHalStatus(ack));
      if (!hidl_retval.isOk()) {
        LOG(ERROR) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
      }
    }
    return Void();
  }

  Return<void> stopStream() {
    sink_->StopRequest();
    return Void();
  }

  Return<void> getPresentationPosition(getPresentationPosition_cb _hidl_cb) {
    uint64_t remote_delay_report_ns;
    uint64_t total_bytes_read;
    timespec data_position;
    bool retval = sink_->GetPresentationPosition(
        &remote_delay_report_ns, &total_bytes_read, &data_position);

    TimeSpec transmittedOctetsTimeStamp;
    if (retval) {
      transmittedOctetsTimeStamp = timespec_convert_to_hal(data_position);
    } else {
      remote_delay_report_ns = 0;
      total_bytes_read = 0;
      transmittedOctetsTimeStamp = {};
    }
    VLOG(2) << __func__ << ": result=" << retval
            << ", delay=" << remote_delay_report_ns
            << ", data=" << total_bytes_read
            << " byte(s), timestamp=" << toString(transmittedOctetsTimeStamp);
    _hidl_cb((retval ? BluetoothAudioStatus::SUCCESS
                     : BluetoothAudioStatus::FAILURE),
             remote_delay_report_ns, total_bytes_read,
             transmittedOctetsTimeStamp);
    return Void();
  }

  Return<void> updateMetadata(const SourceMetadata& sourceMetadata) {
    LOG(INFO) << __func__ << ": " << sourceMetadata.tracks.size()
              << " track(s)";
    // refer to StreamOut.impl.h within Audio HAL (AUDIO_HAL_VERSION_5_0)
    std::vector<playback_track_metadata> metadata_vec;
    metadata_vec.reserve(sourceMetadata.tracks.size());
    for (const auto& metadata : sourceMetadata.tracks) {
      metadata_vec.push_back({
          .usage = static_cast<audio_usage_t>(metadata.usage),
          .content_type =
              static_cast<audio_content_type_t>(metadata.contentType),
          .gain = metadata.gain,
      });
    }
    const source_metadata_t source_metadata = {
        .track_count = metadata_vec.size(), .tracks = metadata_vec.data()};
    sink_->MetadataChanged(source_metadata);
    return Void();
  }

 private:
  IBluetoothTransportInstance* sink_;
  const android::sp<IBluetoothAudioProvider> provider_;
  TimeSpec timespec_convert_to_hal(const timespec& ts) {
    return {.tvSec = static_cast<uint64_t>(ts.tv_sec),
            .tvNSec = static_cast<uint64_t>(ts.tv_nsec)};
  }
};

class BluetoothAudioDeathRecipient
    : public ::android::hardware::hidl_death_recipient {
 public:
  BluetoothAudioDeathRecipient(
      BluetoothAudioClientInterface* clientif,
      bluetooth::common::MessageLoopThread* message_loop)
      : bluetooth_audio_clientif_(clientif), message_loop_(message_loop) {}
  void serviceDied(
      uint64_t /*cookie*/,
      const ::android::wp<::android::hidl::base::V1_0::IBase>& /*who*/) {
    LOG(WARNING) << __func__ << ": restarting connection with new Audio Hal";
    if (bluetooth_audio_clientif_ != nullptr && message_loop_ != nullptr) {
      // restart the session on the correct thread
      message_loop_->DoInThread(
          FROM_HERE,
          base::BindOnce(
              &BluetoothAudioClientInterface::RenewAudioProviderAndSession,
              base::Unretained(bluetooth_audio_clientif_)));
    } else {
      LOG(ERROR) << __func__ << ": BluetoothAudioClientInterface corrupted";
    }
  }

 private:
  BluetoothAudioClientInterface* bluetooth_audio_clientif_;
  bluetooth::common::MessageLoopThread* message_loop_;
};

BluetoothAudioClientInterface::BluetoothAudioClientInterface(IBluetoothTransportInstance* sink,
                                                             bluetooth::common::MessageLoopThread* message_loop)
    : sink_(sink), provider_(nullptr), session_started_(false), mDataMQ(nullptr),
      death_recipient_(new BluetoothAudioDeathRecipient(this, message_loop)) {
  auto service_manager = android::hardware::defaultServiceManager1_2();
  CHECK(service_manager != nullptr);
  size_t instance_count = 0;
  auto listManifestByInterface_cb = [&instance_count](const hidl_vec<android::hardware::hidl_string>& instanceNames) {
    instance_count = instanceNames.size();
    LOG(INFO) << "listManifestByInterface_cb returns " << instance_count << " instance(s)";
  };
  auto hidl_retval = service_manager->listManifestByInterface(kFullyQualifiedInterfaceName, listManifestByInterface_cb);
  if (!hidl_retval.isOk()) {
    LOG(FATAL) << __func__ << ": IServiceManager::listByInterface failure: " << hidl_retval.description();
  }
  if (instance_count > 0) {
    fetch_audio_provider();
  } else {
    LOG(WARNING) << "IBluetoothAudioProvidersFactory not declared";
  }
}

BluetoothAudioClientInterface::~BluetoothAudioClientInterface() {
  if (provider_ != nullptr) {
    auto hidl_retval = provider_->unlinkToDeath(death_recipient_);
    if (!hidl_retval.isOk()) {
      LOG(FATAL) << __func__ << ": BluetoothAudioDeathRecipient failure: " << hidl_retval.description();
    }
  }
}

std::vector<AudioCapabilities>
BluetoothAudioClientInterface::GetAudioCapabilities() const {
  return capabilities_;
}

void BluetoothAudioClientInterface::fetch_audio_provider() {
  if (provider_ != nullptr) {
    LOG(WARNING) << __func__ << ": reflash";
  }

  android::sp<IBluetoothAudioProvidersFactory> providersFactory =
      IBluetoothAudioProvidersFactory::getService();
  CHECK(providersFactory != nullptr) << "IBluetoothAudioProvidersFactory::getService() failed";
  LOG(INFO) << "IBluetoothAudioProvidersFactory::getService() returned "
            << providersFactory.get()
            << (providersFactory->isRemote() ? " (remote)" : " (local)");

  std::promise<void> getProviderCapabilities_promise;
  auto getProviderCapabilities_future =
      getProviderCapabilities_promise.get_future();
  auto getProviderCapabilities_cb =
      [& capabilities = this->capabilities_, &getProviderCapabilities_promise](
          const hidl_vec<AudioCapabilities>& audioCapabilities) {
        for (auto capability : audioCapabilities) {
          capabilities.push_back(capability);
        }
        getProviderCapabilities_promise.set_value();
      };
  auto hidl_retval = providersFactory->getProviderCapabilities(
      sink_->GetSessionType(), getProviderCapabilities_cb);
  getProviderCapabilities_future.get();
  if (!hidl_retval.isOk()) {
    LOG(FATAL) << __func__ << ": BluetoothAudioHal::getProviderCapabilities failure: " << hidl_retval.description();
    return;
  }
  if (capabilities_.empty()) {
    LOG(WARNING) << __func__
                 << ": SessionType=" << toString(sink_->GetSessionType())
                 << " Not supported by BluetoothAudioHal";
    return;
  }
  LOG(INFO) << __func__ << ": BluetoothAudioHal SessionType="
            << toString(sink_->GetSessionType()) << " has "
            << capabilities_.size() << " AudioCapabilities";

  std::promise<void> openProvider_promise;
  auto openProvider_future = openProvider_promise.get_future();
  auto openProvider_cb =
      [& provider_ = this->provider_, &openProvider_promise](
          BluetoothAudioStatus status,
          const android::sp<IBluetoothAudioProvider>& provider) {
        LOG(INFO) << "openProvider_cb(" << toString(status) << ")";
        if (status == BluetoothAudioStatus::SUCCESS) {
          provider_ = provider;
        }
        ALOGE_IF(!provider_, "Failed to open BluetoothAudio provider");
        openProvider_promise.set_value();
      };
  hidl_retval =
      providersFactory->openProvider(sink_->GetSessionType(), openProvider_cb);
  openProvider_future.get();
  if (!hidl_retval.isOk()) {
    LOG(FATAL) << __func__ << ": BluetoothAudioHal::openProvider failure: " << hidl_retval.description();
  }
  CHECK(provider_ != nullptr);

  if (!provider_->linkToDeath(death_recipient_, 0).isOk()) {
    LOG(FATAL) << __func__ << ": BluetoothAudioDeathRecipient failure: " << hidl_retval.description();
  }

  LOG(INFO) << "IBluetoothAudioProvidersFactory::openProvider() returned "
            << provider_.get()
            << (provider_->isRemote() ? " (remote)" : " (local)");
}

bool BluetoothAudioClientInterface::UpdateAudioConfig(
    const AudioConfiguration& audio_config) {
  bool is_software_session =
      (sink_->GetSessionType() ==
           SessionType::A2DP_SOFTWARE_ENCODING_DATAPATH ||
       sink_->GetSessionType() ==
           SessionType::HEARING_AID_SOFTWARE_ENCODING_DATAPATH);
  bool is_offload_session =
      (sink_->GetSessionType() == SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH);
  auto audio_config_discriminator = audio_config.getDiscriminator();
  bool is_software_audio_config =
      (is_software_session &&
       audio_config_discriminator ==
           AudioConfiguration::hidl_discriminator::pcmConfig);
  bool is_offload_audio_config =
      (is_offload_session &&
       audio_config_discriminator ==
           AudioConfiguration::hidl_discriminator::codecConfig);
  if (!is_software_audio_config && !is_offload_audio_config) {
    return false;
  }
  sink_->UpdateAudioConfiguration(audio_config);
  return true;
}

int BluetoothAudioClientInterface::StartSession() {
  std::lock_guard<std::mutex> guard(internal_mutex_);
  if (provider_ == nullptr) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal nullptr";
    session_started_ = false;
    return -EINVAL;
  }
  if (session_started_) {
    LOG(ERROR) << __func__ << ": session started already";
    return -EBUSY;
  }

  android::sp<IBluetoothAudioPort> stack_if =
      new BluetoothAudioPortImpl(sink_, provider_);
  std::unique_ptr<DataMQ> tempDataMQ;
  BluetoothAudioStatus session_status;

  std::promise<void> hidl_startSession_promise;
  auto hidl_startSession_future = hidl_startSession_promise.get_future();
  auto hidl_cb = [&session_status, &tempDataMQ, &hidl_startSession_promise](
                     BluetoothAudioStatus status,
                     const DataMQ::Descriptor& dataMQ) {
    LOG(INFO) << "startSession_cb(" << toString(status) << ")";
    session_status = status;
    if (status == BluetoothAudioStatus::SUCCESS && dataMQ.isHandleValid()) {
      tempDataMQ.reset(new DataMQ(dataMQ));
    }
    hidl_startSession_promise.set_value();
  };
  auto hidl_retval = provider_->startSession(
      stack_if, sink_->GetAudioConfiguration(), hidl_cb);
  hidl_startSession_future.get();
  if (!hidl_retval.isOk()) {
    LOG(FATAL) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
    return -EPROTO;
  }

  if (tempDataMQ && tempDataMQ->isValid()) {
    mDataMQ = std::move(tempDataMQ);
  } else if (sink_->GetSessionType() ==
                 SessionType::A2DP_HARDWARE_OFFLOAD_DATAPATH &&
             session_status == BluetoothAudioStatus::SUCCESS) {
    sink_->ResetPresentationPosition();
    session_started_ = true;
    return 0;
  }
  if (mDataMQ && mDataMQ->isValid()) {
    sink_->ResetPresentationPosition();
    session_started_ = true;
    return 0;
  } else {
    ALOGE_IF(!mDataMQ, "Failed to obtain audio data path");
    ALOGE_IF(mDataMQ && !mDataMQ->isValid(), "Audio data path is invalid");
    session_started_ = false;
    return -EIO;
  }
}

void BluetoothAudioClientInterface::StreamStarted(
    const BluetoothAudioCtrlAck& ack) {
  if (provider_ == nullptr) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal nullptr";
    return;
  }
  if (ack == BluetoothAudioCtrlAck::PENDING) {
    LOG(INFO) << __func__ << ": " << ack << " ignored";
    return;
  }
  BluetoothAudioStatus status = BluetoothAudioCtrlAckToHalStatus(ack);
  auto hidl_retval = provider_->streamStarted(status);
  if (!hidl_retval.isOk()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
  }
}

void BluetoothAudioClientInterface::StreamSuspended(
    const BluetoothAudioCtrlAck& ack) {
  if (provider_ == nullptr) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal nullptr";
    return;
  }
  if (ack == BluetoothAudioCtrlAck::PENDING) {
    LOG(INFO) << __func__ << ": " << ack << " ignored";
    return;
  }
  BluetoothAudioStatus status = BluetoothAudioCtrlAckToHalStatus(ack);
  auto hidl_retval = provider_->streamSuspended(status);
  if (!hidl_retval.isOk()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
  }
}

int BluetoothAudioClientInterface::EndSession() {
  std::lock_guard<std::mutex> guard(internal_mutex_);
  if (!session_started_) {
    LOG(INFO) << __func__ << ": sessoin ended already";
    return 0;
  }

  session_started_ = false;
  if (provider_ == nullptr) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal nullptr";
    return -EINVAL;
  }
  mDataMQ = nullptr;
  auto hidl_retval = provider_->endSession();
  if (!hidl_retval.isOk()) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal failure: " << hidl_retval.description();
    return -EPROTO;
  }
  return 0;
}

size_t BluetoothAudioClientInterface::ReadAudioData(uint8_t* p_buf,
                                                    uint32_t len) {
  if (provider_ == nullptr) {
    LOG(ERROR) << __func__ << ": BluetoothAudioHal nullptr";
    return 0;
  }
  if (p_buf == nullptr || len == 0) return 0;

  std::lock_guard<std::mutex> guard(internal_mutex_);

  size_t total_read = 0;
  int timeout_ms = kDefaultDataReadTimeoutMs;
  do {
    if (mDataMQ == nullptr || !mDataMQ->isValid()) break;

    size_t avail_to_read = mDataMQ->availableToRead();
    if (avail_to_read) {
      if (avail_to_read > len - total_read) {
        avail_to_read = len - total_read;
      }
      if (mDataMQ->read(p_buf + total_read, avail_to_read) == 0) {
        LOG(WARNING) << __func__ << ": len=" << len
                     << " total_read=" << total_read << " failed";
        break;
      }
      total_read += avail_to_read;
    } else if (timeout_ms >= kDefaultDataReadPollIntervalMs) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(kDefaultDataReadPollIntervalMs));
      timeout_ms -= kDefaultDataReadPollIntervalMs;
      continue;
    } else {
      LOG(WARNING) << __func__ << ": " << (len - total_read) << "/" << len
                   << " no data " << (kDefaultDataReadTimeoutMs - timeout_ms)
                   << " ms";
      break;
    }
  } while (total_read < len);

  if (timeout_ms <
          (kDefaultDataReadTimeoutMs - kDefaultDataReadPollIntervalMs) &&
      timeout_ms >= kDefaultDataReadPollIntervalMs) {
    VLOG(1) << __func__ << ": underflow " << len << " -> " << total_read
            << " read " << (kDefaultDataReadTimeoutMs - timeout_ms) << " ms";
  } else {
    VLOG(2) << __func__ << ": " << len << " -> " << total_read << " read";
  }

  sink_->LogBytesRead(total_read);
  return total_read;
}

size_t BluetoothAudioClientInterface::WriteAudioData(uint8_t* p_buf,
                                                     uint32_t len) {
  // Not implemented!
  return 0;
}

void BluetoothAudioClientInterface::RenewAudioProviderAndSession() {
  // NOTE: must be invoked on the same thread where this
  // BluetoothAudioClientInterface is running
  fetch_audio_provider();
  session_started_ = false;
  StartSession();
}

}  // namespace audio
}  // namespace bluetooth
