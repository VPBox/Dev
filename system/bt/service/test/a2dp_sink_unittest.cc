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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "service/a2dp_sink.h"
#include "service/hal/fake_bluetooth_av_interface.h"

using ::testing::_;
using ::testing::Return;

namespace bluetooth {
namespace {

class MockA2dpSinkHandler
    : public hal::FakeBluetoothAvInterface::TestA2dpSinkHandler {
 public:
  MockA2dpSinkHandler() = default;
  ~MockA2dpSinkHandler() override = default;

  MOCK_METHOD1(Connect, bt_status_t(RawAddress));
  MOCK_METHOD1(Disconnect, bt_status_t(RawAddress));
  MOCK_METHOD1(SetAudioFocusState, void(int));
  MOCK_METHOD1(SetAudioTrackGain, void(float));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockA2dpSinkHandler);
};

class TestDelegate : public A2dpSink::Delegate {
 public:
  TestDelegate() = default;
  ~TestDelegate() override = default;

  struct RequestData {
    std::string device_address;
    int state = -1;
    uint32_t sample_rate = 0;
    uint8_t channel_count = 0;
    int count = 0;
  };

  // A2dpSink::Delegate implementation:
  void OnConnectionState(const std::string& device_address,
                         int state) override {
    ++connection_state_.count;
    connection_state_.device_address = device_address;
    connection_state_.state = state;
  }
  void OnAudioState(const std::string& device_address, int state) override {
    ++audio_state_.count;
    audio_state_.device_address = device_address;
    audio_state_.state = state;
  }
  void OnAudioConfig(const std::string& device_address, uint32_t sample_rate,
                     uint8_t channel_count) override {
    ++audio_config_.count;
    audio_config_.device_address = device_address;
    audio_config_.sample_rate = sample_rate;
    audio_config_.channel_count = channel_count;
  }

  const RequestData& connection_state() const { return connection_state_; }
  const RequestData& audio_state() const { return audio_state_; }
  const RequestData& audio_config() const { return audio_config_; }

 private:
  RequestData connection_state_;
  RequestData audio_state_;
  RequestData audio_config_;
};

class A2dpSinkTest : public ::testing::Test {
 public:
  A2dpSinkTest() = default;
  ~A2dpSinkTest() override = default;

  void SetUp() override {
    mock_handler_.reset(new MockA2dpSinkHandler());
    fake_hal_av_iface_ = new hal::FakeBluetoothAvInterface(mock_handler_);
    hal::BluetoothAvInterface::InitializeForTesting(fake_hal_av_iface_);
    factory_.reset(new A2dpSinkFactory());
  }

  void TearDown() override {
    factory_.reset();
    hal::BluetoothAvInterface::CleanUp();
  }

 protected:
  hal::FakeBluetoothAvInterface* fake_hal_av_iface_;
  std::shared_ptr<MockA2dpSinkHandler> mock_handler_;
  std::unique_ptr<A2dpSinkFactory> factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(A2dpSinkTest);
};

class A2dpSinkPostRegisterTest : public A2dpSinkTest {
 public:
  A2dpSinkPostRegisterTest() = default;
  ~A2dpSinkPostRegisterTest() override = default;

  void SetUp() override {
    A2dpSinkTest::SetUp();
    Uuid uuid = Uuid::GetRandom();
    auto callback = [&](BLEStatus status, const Uuid& in_uuid,
                        std::unique_ptr<BluetoothInstance> in_client) {
      CHECK(in_uuid == uuid);
      CHECK(in_client.get());
      CHECK(status == BLE_STATUS_SUCCESS);

      a2dp_sink_ = std::unique_ptr<A2dpSink>(
          static_cast<A2dpSink*>(in_client.release()));
    };

    factory_->RegisterInstance(uuid, callback);
  }

  void TearDown() override {
    a2dp_sink_ = nullptr;
    A2dpSinkTest::TearDown();
  }

 protected:
  void Connect(const std::string& addr) {
    RawAddress hal_addr;
    ASSERT_TRUE(RawAddress::FromString(addr, hal_addr));

    EXPECT_CALL(*mock_handler_, Connect(hal_addr))
        .WillOnce(Return(BT_STATUS_SUCCESS));

    EXPECT_TRUE(a2dp_sink_->Connect(addr));
  }

  void Disconnect(const std::string& addr) {
    RawAddress hal_addr;
    ASSERT_TRUE(RawAddress::FromString(addr, hal_addr));

    EXPECT_CALL(*mock_handler_, Disconnect(hal_addr))
        .WillOnce(Return(BT_STATUS_SUCCESS));

    EXPECT_TRUE(a2dp_sink_->Disconnect(addr));
  }

  std::unique_ptr<A2dpSink> a2dp_sink_;

 private:
  DISALLOW_COPY_AND_ASSIGN(A2dpSinkPostRegisterTest);
};

TEST_F(A2dpSinkTest, RegisterA2dpSink) {
  // These will be asynchronously populate with a result when the callback
  // executes.
  BLEStatus status = BLE_STATUS_SUCCESS;
  Uuid cb_uuid;
  std::unique_ptr<A2dpSink> a2dp_sink;
  int callback_count = 0;

  auto callback = [&](BLEStatus in_status, const Uuid& uuid,
                      std::unique_ptr<BluetoothInstance> in_a2dp_sink) {
    status = in_status;
    cb_uuid = uuid;
    a2dp_sink = std::unique_ptr<A2dpSink>(
        static_cast<A2dpSink*>(in_a2dp_sink.release()));
    callback_count++;
  };

  Uuid uuid0 = Uuid::GetRandom();

  // This should always succeed.
  EXPECT_TRUE(factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(1, callback_count);

  testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  ASSERT_TRUE(a2dp_sink.get() !=
              nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_SUCCESS, status);
  EXPECT_EQ(bluetooth::A2dpSink::kSingletonInstanceId,
            a2dp_sink->GetInstanceId());
  EXPECT_EQ(uuid0, a2dp_sink->GetAppIdentifier());
  EXPECT_EQ(uuid0, cb_uuid);

  testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
}

TEST_F(A2dpSinkPostRegisterTest, Connect) {
  static const char kTestAddr[] = "AA:BB:CC:DD:EE:FF";
  Connect(kTestAddr);
  Disconnect(kTestAddr);
}

TEST_F(A2dpSinkPostRegisterTest, SetAudioFocusState) {
  static const char kTestAddr[] = "AA:BB:CC:DD:EE:FF";
  static const int kFocusState = 2;
  Connect(kTestAddr);

  EXPECT_CALL(*mock_handler_, SetAudioFocusState(kFocusState));
  a2dp_sink_->SetAudioFocusState(kFocusState);

  Disconnect(kTestAddr);
}

TEST_F(A2dpSinkPostRegisterTest, SetAudioTrackGain) {
  static const char kTestAddr[] = "AA:BB:CC:DD:EE:FF";
  static const float kTrackGain = 0.5;
  Connect(kTestAddr);

  EXPECT_CALL(*mock_handler_, SetAudioTrackGain(kTrackGain));
  a2dp_sink_->SetAudioTrackGain(kTrackGain);

  Disconnect(kTestAddr);
}

TEST_F(A2dpSinkPostRegisterTest, CallbackTest) {
  static const char kTestAddr[] = "AA:BB:CC:DD:EE:FF";
  RawAddress hal_addr;
  ASSERT_TRUE(RawAddress::FromString(kTestAddr, hal_addr));

  TestDelegate delegate;
  a2dp_sink_->SetDelegate(&delegate);
  Connect(kTestAddr);

  // OnConnectionState
  const int kConnectionState = 2;
  EXPECT_EQ(0, delegate.connection_state().count);
  fake_hal_av_iface_->NotifyConnectionState(
      hal_addr, static_cast<btav_connection_state_t>(kConnectionState));
  EXPECT_EQ(1, delegate.connection_state().count);
  EXPECT_EQ(kTestAddr, delegate.connection_state().device_address);
  EXPECT_EQ(kConnectionState, delegate.connection_state().state);

  // OnAudioState
  const int kAudioState = 1;
  EXPECT_EQ(0, delegate.audio_state().count);
  fake_hal_av_iface_->NotifyAudioState(
      hal_addr, static_cast<btav_audio_state_t>(kAudioState));
  EXPECT_EQ(1, delegate.audio_state().count);
  EXPECT_EQ(kTestAddr, delegate.audio_state().device_address);
  EXPECT_EQ(kAudioState, delegate.audio_state().state);

  // OnAudioConfig
  const uint32_t kSampleRate = 44100;
  const uint32_t kChannelCount = 2;
  EXPECT_EQ(0, delegate.audio_config().count);
  fake_hal_av_iface_->NotifyAudioConfig(hal_addr, kSampleRate, kChannelCount);
  EXPECT_EQ(1, delegate.audio_config().count);
  EXPECT_EQ(kTestAddr, delegate.audio_config().device_address);
  EXPECT_EQ(kSampleRate, delegate.audio_config().sample_rate);
  EXPECT_EQ(kChannelCount, delegate.audio_config().channel_count);

  Disconnect(kTestAddr);
}

}  // namespace
}  // namespace bluetooth
