//
//  Copyright 2015 Google, Inc.
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

#include <base/macros.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "service/adapter.h"
#include "service/hal/fake_bluetooth_gatt_interface.h"
#include "service/low_energy_client.h"
#include "stack/include/bt_types.h"
#include "stack/include/hcidefs.h"
#include "test/mock_adapter.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Pointee;
using ::testing::DoAll;
using ::testing::Invoke;

namespace bluetooth {
namespace {

class MockGattHandler
    : public hal::FakeBluetoothGattInterface::TestClientHandler {
 public:
  MockGattHandler(){};
  ~MockGattHandler() override = default;

  MOCK_METHOD1(RegisterClient, bt_status_t(const bluetooth::Uuid&));
  MOCK_METHOD1(UnregisterClient, bt_status_t(int));
  MOCK_METHOD4(Connect, bt_status_t(int, const RawAddress&, bool, int));
  MOCK_METHOD3(Disconnect, bt_status_t(int, const RawAddress&, int));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockGattHandler);
};

class TestDelegate : public LowEnergyClient::Delegate {
 public:
  TestDelegate() : connection_state_count_(0), last_mtu_(0) {}

  ~TestDelegate() override = default;

  int connection_state_count() const { return connection_state_count_; }

  void OnConnectionState(LowEnergyClient* client, int status,
                         const char* address, bool connected) {
    ASSERT_TRUE(client);
    connection_state_count_++;
  }

  void OnMtuChanged(LowEnergyClient* client, int status, const char* address,
                    int mtu) {
    ASSERT_TRUE(client);
    last_mtu_ = mtu;
  }

 private:
  int connection_state_count_;

  int last_mtu_;

  DISALLOW_COPY_AND_ASSIGN(TestDelegate);
};

class LowEnergyClientTest : public ::testing::Test {
 public:
  LowEnergyClientTest() = default;
  ~LowEnergyClientTest() override = default;

  void SetUp() override {
    // Only set |mock_handler_| if a test hasn't set it.
    if (!mock_handler_) mock_handler_.reset(new MockGattHandler());
    fake_hal_gatt_iface_ = new hal::FakeBluetoothGattInterface(
        nullptr, nullptr,
        std::static_pointer_cast<
            hal::FakeBluetoothGattInterface::TestClientHandler>(mock_handler_),
        nullptr);
    hal::BluetoothGattInterface::InitializeForTesting(fake_hal_gatt_iface_);
    ble_factory_.reset(new LowEnergyClientFactory(mock_adapter_));
  }

  void TearDown() override {
    ble_factory_.reset();
    hal::BluetoothGattInterface::CleanUp();
  }

 protected:
  hal::FakeBluetoothGattInterface* fake_hal_gatt_iface_;
  testing::MockAdapter mock_adapter_;
  std::shared_ptr<MockGattHandler> mock_handler_;
  std::unique_ptr<LowEnergyClientFactory> ble_factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(LowEnergyClientTest);
};

// Used for tests that operate on a pre-registered client.
class LowEnergyClientPostRegisterTest : public LowEnergyClientTest {
 public:
  LowEnergyClientPostRegisterTest() : next_client_id_(0) {}
  ~LowEnergyClientPostRegisterTest() override = default;

  void SetUp() override {
    LowEnergyClientTest::SetUp();
    auto callback = [&](std::unique_ptr<LowEnergyClient> client) {
      le_client_ = std::move(client);
    };
    RegisterTestClient(callback);
  }

  void TearDown() override {
    EXPECT_CALL(*mock_handler_, UnregisterClient(_))
        .Times(1)
        .WillOnce(Return(BT_STATUS_SUCCESS));
    le_client_.reset();
    LowEnergyClientTest::TearDown();
  }

  void RegisterTestClient(
      const std::function<void(std::unique_ptr<LowEnergyClient> client)>
          callback) {
    Uuid uuid = Uuid::GetRandom();
    auto api_callback = [&](BLEStatus status, const Uuid& in_uuid,
                            std::unique_ptr<BluetoothInstance> in_client) {
      CHECK(in_uuid == uuid);
      CHECK(in_client.get());
      CHECK(status == BLE_STATUS_SUCCESS);

      callback(std::unique_ptr<LowEnergyClient>(
          static_cast<LowEnergyClient*>(in_client.release())));
    };

    EXPECT_CALL(*mock_handler_, RegisterClient(_))
        .Times(1)
        .WillOnce(Return(BT_STATUS_SUCCESS));

    ble_factory_->RegisterInstance(uuid, api_callback);

    fake_hal_gatt_iface_->NotifyRegisterClientCallback(0, next_client_id_++,
                                                       uuid);
    ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
  }

 protected:
  std::unique_ptr<LowEnergyClient> le_client_;

 private:
  int next_client_id_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyClientPostRegisterTest);
};

TEST_F(LowEnergyClientTest, RegisterInstance) {
  EXPECT_CALL(*mock_handler_, RegisterClient(_))
      .Times(2)
      .WillOnce(Return(BT_STATUS_FAIL))
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // These will be asynchronously populated with a result when the callback
  // executes.
  BLEStatus status = BLE_STATUS_SUCCESS;
  Uuid cb_uuid;
  std::unique_ptr<LowEnergyClient> client;
  int callback_count = 0;

  auto callback = [&](BLEStatus in_status, const Uuid& uuid,
                      std::unique_ptr<BluetoothInstance> in_client) {
    status = in_status;
    cb_uuid = uuid;
    client = std::unique_ptr<LowEnergyClient>(
        static_cast<LowEnergyClient*>(in_client.release()));
    callback_count++;
  };

  Uuid uuid0 = Uuid::GetRandom();

  // HAL returns failure.
  EXPECT_FALSE(ble_factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // HAL returns success.
  EXPECT_TRUE(ble_factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // Calling twice with the same Uuid should fail with no additional call into
  // the stack.
  EXPECT_FALSE(ble_factory_->RegisterInstance(uuid0, callback));

  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Call with a different Uuid while one is pending.
  Uuid uuid1 = Uuid::GetRandom();
  EXPECT_CALL(*mock_handler_, RegisterClient(_))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_TRUE(ble_factory_->RegisterInstance(uuid1, callback));

  // Trigger callback with an unknown Uuid. This should get ignored.
  Uuid uuid2 = Uuid::GetRandom();
  fake_hal_gatt_iface_->NotifyRegisterClientCallback(0, 0, uuid2);
  EXPECT_EQ(0, callback_count);

  // |uuid0| succeeds.
  int client_if0 = 2;  // Pick something that's not 0.
  fake_hal_gatt_iface_->NotifyRegisterClientCallback(BT_STATUS_SUCCESS,
                                                     client_if0, uuid0);

  EXPECT_EQ(1, callback_count);
  ASSERT_TRUE(client.get() != nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_SUCCESS, status);
  EXPECT_EQ(client_if0, client->GetInstanceId());
  EXPECT_EQ(uuid0, client->GetAppIdentifier());
  EXPECT_EQ(uuid0, cb_uuid);

  // The client should unregister itself when deleted.
  EXPECT_CALL(*mock_handler_, UnregisterClient(client_if0))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  client.reset();
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // |uuid1| fails.
  int client_if1 = 3;
  fake_hal_gatt_iface_->NotifyRegisterClientCallback(BT_STATUS_FAIL, client_if1,
                                                     uuid1);

  EXPECT_EQ(2, callback_count);
  ASSERT_TRUE(client.get() == nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_FAILURE, status);
  EXPECT_EQ(uuid1, cb_uuid);
}

MATCHER_P(BitEq, x, std::string(negation ? "isn't" : "is") +
                        " bitwise equal to " + ::testing::PrintToString(x)) {
  static_assert(sizeof(x) == sizeof(arg), "Size mismatch");
  return std::memcmp(&arg, &x, sizeof(x)) == 0;
}

TEST_F(LowEnergyClientPostRegisterTest, Connect) {
  const RawAddress kTestAddress = {{0x01, 0x02, 0x03, 0x0A, 0x0B, 0x0C}};
  const char kTestAddressStr[] = "01:02:03:0A:0B:0C";
  const bool kTestDirect = false;
  const int connId = 12;

  TestDelegate delegate;
  le_client_->SetDelegate(&delegate);

  // TODO(jpawlowski): NotifyConnectCallback should be called after returning
  // success, fix it when it becomes important.
  // These should succeed and result in a HAL call
  EXPECT_CALL(*mock_handler_,
              Connect(le_client_->GetInstanceId(), BitEq(kTestAddress),
                      kTestDirect, BT_TRANSPORT_LE))
      .Times(1)
      .WillOnce(DoAll(Invoke([&](int client_id, const RawAddress& bd_addr,
                                 bool is_direct, int transport) {
                        fake_hal_gatt_iface_->NotifyConnectCallback(
                            connId, BT_STATUS_SUCCESS, client_id, bd_addr);
                      }),
                      Return(BT_STATUS_SUCCESS)));

  EXPECT_TRUE(le_client_->Connect(kTestAddressStr, kTestDirect));
  EXPECT_EQ(1, delegate.connection_state_count());

  // TODO(jpawlowski): same as above
  // These should succeed and result in a HAL call
  EXPECT_CALL(*mock_handler_, Disconnect(le_client_->GetInstanceId(),
                                         BitEq(kTestAddress), connId))
      .Times(1)
      .WillOnce(DoAll(
          Invoke([&](int client_id, const RawAddress& bd_addr, int connId) {
            fake_hal_gatt_iface_->NotifyDisconnectCallback(
                connId, BT_STATUS_SUCCESS, client_id, bd_addr);
          }),
          Return(BT_STATUS_SUCCESS)));

  EXPECT_TRUE(le_client_->Disconnect(kTestAddressStr));
  EXPECT_EQ(2, delegate.connection_state_count());

  le_client_->SetDelegate(nullptr);
  ::testing::Mock::VerifyAndClearExpectations(mock_handler_.get());
}

}  // namespace
}  // namespace bluetooth
