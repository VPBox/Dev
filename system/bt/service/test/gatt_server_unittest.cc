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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "service/gatt_server.h"
#include "service/hal/fake_bluetooth_gatt_interface.h"

using ::testing::_;
using ::testing::Return;

namespace bluetooth {
namespace {

class MockGattHandler
    : public hal::FakeBluetoothGattInterface::TestServerHandler {
 public:
  MockGattHandler() = default;
  ~MockGattHandler() override = default;

  MOCK_METHOD1(RegisterServer, bt_status_t(const bluetooth::Uuid&));
  MOCK_METHOD1(UnregisterServer, bt_status_t(int));
  MOCK_METHOD2(AddService, bt_status_t(int, std::vector<btgatt_db_element_t>));
  MOCK_METHOD5(AddCharacteristic,
               bt_status_t(int, int, bluetooth::Uuid*, int, int));
  MOCK_METHOD4(AddDescriptor, bt_status_t(int, int, bluetooth::Uuid*, int));
  MOCK_METHOD3(StartService, bt_status_t(int, int, int));
  MOCK_METHOD2(DeleteService, bt_status_t(int, int));
  MOCK_METHOD5(SendIndication,
               bt_status_t(int, int, int, int, std::vector<uint8_t>));
  MOCK_METHOD4(SendResponse,
               bt_status_t(int, int, int, const btgatt_response_t&));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockGattHandler);
};

class TestDelegate : public GattServer::Delegate {
 public:
  TestDelegate() = default;
  ~TestDelegate() override = default;

  struct RequestData {
    RequestData()
        : id(-1),
          offset(-1),
          is_long(false),
          is_prep(false),
          need_rsp(false),
          is_exec(false),
          count(0),
          connected(false) {}
    ~RequestData() = default;

    std::string device_address;
    int id;
    int offset;
    bool is_long;
    bool is_prep;
    bool need_rsp;
    bool is_exec;
    uint16_t handle;
    int count;
    std::vector<uint8_t> write_value;
    bool connected;
  };

  void OnCharacteristicReadRequest(GattServer* gatt_server,
                                   const std::string& device_address,
                                   int request_id, int offset, bool is_long,
                                   uint16_t handle) override {
    ASSERT_TRUE(gatt_server);
    char_read_req_.device_address = device_address;
    char_read_req_.id = request_id;
    char_read_req_.offset = offset;
    char_read_req_.is_long = is_long;
    char_read_req_.handle = handle;
    char_read_req_.count++;
  }

  void OnDescriptorReadRequest(GattServer* gatt_server,
                               const std::string& device_address,
                               int request_id, int offset, bool is_long,
                               uint16_t handle) override {
    ASSERT_TRUE(gatt_server);
    desc_read_req_.device_address = device_address;
    desc_read_req_.id = request_id;
    desc_read_req_.offset = offset;
    desc_read_req_.is_long = is_long;
    desc_read_req_.handle = handle;
    desc_read_req_.count++;
  }

  void OnCharacteristicWriteRequest(GattServer* gatt_server,
                                    const std::string& device_address,
                                    int request_id, int offset,
                                    bool is_prepare_write, bool need_response,
                                    const std::vector<uint8_t>& value,
                                    uint16_t handle) override {
    ASSERT_TRUE(gatt_server);
    char_write_req_.device_address = device_address;
    char_write_req_.id = request_id;
    char_write_req_.offset = offset;
    char_write_req_.is_prep = is_prepare_write;
    char_write_req_.need_rsp = need_response;
    char_write_req_.handle = handle;
    char_write_req_.count++;
    char_write_req_.write_value = value;
  }

  void OnDescriptorWriteRequest(GattServer* gatt_server,
                                const std::string& device_address,
                                int request_id, int offset,
                                bool is_prepare_write, bool need_response,
                                const std::vector<uint8_t>& value,
                                uint16_t handle) override {
    ASSERT_TRUE(gatt_server);
    desc_write_req_.device_address = device_address;
    desc_write_req_.id = request_id;
    desc_write_req_.offset = offset;
    desc_write_req_.is_prep = is_prepare_write;
    desc_write_req_.need_rsp = need_response;
    desc_write_req_.handle = handle;
    desc_write_req_.count++;
    desc_write_req_.write_value = value;
  }

  void OnExecuteWriteRequest(GattServer* gatt_server,
                             const std::string& device_address, int request_id,
                             bool is_execute) override {
    ASSERT_TRUE(gatt_server);
    exec_req_.device_address = device_address;
    exec_req_.id = request_id;
    exec_req_.is_exec = is_execute;
    exec_req_.count++;
  }

  void OnConnectionStateChanged(GattServer* gatt_server,
                                const std::string& device_address,
                                bool connected) override {
    ASSERT_TRUE(gatt_server);
    conn_state_changed_.device_address = device_address;
    conn_state_changed_.connected = connected;
    conn_state_changed_.count++;
  }

  const RequestData& char_read_req() const { return char_read_req_; }
  const RequestData& desc_read_req() const { return desc_read_req_; }
  const RequestData& char_write_req() const { return char_write_req_; }
  const RequestData& desc_write_req() const { return desc_write_req_; }
  const RequestData& conn_state_changed() const { return conn_state_changed_; }

 private:
  RequestData char_read_req_;
  RequestData desc_read_req_;
  RequestData char_write_req_;
  RequestData desc_write_req_;
  RequestData exec_req_;
  RequestData conn_state_changed_;
};

class GattServerTest : public ::testing::Test {
 public:
  GattServerTest() = default;
  ~GattServerTest() override = default;

  void SetUp() override {
    mock_handler_.reset(new MockGattHandler());
    fake_hal_gatt_iface_ = new hal::FakeBluetoothGattInterface(
        nullptr, nullptr, nullptr,
        std::static_pointer_cast<
            hal::FakeBluetoothGattInterface::TestServerHandler>(mock_handler_));

    hal::BluetoothGattInterface::InitializeForTesting(fake_hal_gatt_iface_);
    factory_.reset(new GattServerFactory());
  }

  void TearDown() override {
    factory_.reset();
    hal::BluetoothGattInterface::CleanUp();
  }

 protected:
  hal::FakeBluetoothGattInterface* fake_hal_gatt_iface_;
  std::shared_ptr<MockGattHandler> mock_handler_;
  std::unique_ptr<GattServerFactory> factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(GattServerTest);
};

const int kDefaultServerId = 4;

class GattServerPostRegisterTest : public GattServerTest {
 public:
  GattServerPostRegisterTest() = default;
  ~GattServerPostRegisterTest() override = default;

  void SetUp() override {
    GattServerTest::SetUp();
    Uuid uuid = Uuid::GetRandom();
    auto callback = [&](BLEStatus status, const Uuid& in_uuid,
                        std::unique_ptr<BluetoothInstance> in_client) {
      CHECK(in_uuid == uuid);
      CHECK(in_client.get());
      CHECK(status == BLE_STATUS_SUCCESS);

      gatt_server_ = std::unique_ptr<GattServer>(
          static_cast<GattServer*>(in_client.release()));
    };

    EXPECT_CALL(*mock_handler_, RegisterServer(_))
        .Times(1)
        .WillOnce(Return(BT_STATUS_SUCCESS));

    factory_->RegisterInstance(uuid, callback);

    fake_hal_gatt_iface_->NotifyRegisterServerCallback(BT_STATUS_SUCCESS,
                                                       kDefaultServerId, uuid);
  }

  void TearDown() override {
    EXPECT_CALL(*mock_handler_, UnregisterServer(_))
        .Times(1)
        .WillOnce(Return(BT_STATUS_SUCCESS));
    gatt_server_ = nullptr;
    GattServerTest::TearDown();
  }

  void SetUpTestService() {
    EXPECT_CALL(*mock_handler_, AddService(_, _))
        .Times(1)
        .WillOnce(Return(BT_STATUS_SUCCESS));

    Uuid uuid0 = Uuid::GetRandom();
    Uuid uuid1 = Uuid::GetRandom();
    Uuid uuid2 = Uuid::GetRandom();

    bool register_success = false;

    Service service(0, true, uuid0, {}, {});

    ASSERT_TRUE(gatt_server_->AddService(
        service, [&](BLEStatus status, const Service& added_service) {
          ASSERT_EQ(BLE_STATUS_SUCCESS, status);
          ASSERT_TRUE(Uuid(added_service.uuid()) == Uuid(service.uuid()));
          ASSERT_TRUE(added_service.handle() == 0x0001);
          register_success = true;
        }));

    srvc_handle_ = 0x0001;
    char_handle_ = 0x0002;
    desc_handle_ = 0x0004;

    std::vector<btgatt_db_element_t> service_with_handles = {
        {.type = BTGATT_DB_PRIMARY_SERVICE,
         .uuid = uuid0,
         .attribute_handle = srvc_handle_},
        {.type = BTGATT_DB_CHARACTERISTIC,
         .uuid = uuid1,
         .attribute_handle = char_handle_},
        {.type = BTGATT_DB_DESCRIPTOR,
         .uuid = uuid2,
         .attribute_handle = desc_handle_},
    };

    fake_hal_gatt_iface_->NotifyServiceAddedCallback(
        BT_STATUS_SUCCESS, kDefaultServerId, service_with_handles);

    testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

    ASSERT_TRUE(register_success);
  }

 protected:
  std::unique_ptr<GattServer> gatt_server_;

  uint16_t srvc_handle_;
  uint16_t char_handle_;
  uint16_t desc_handle_;

 private:
  DISALLOW_COPY_AND_ASSIGN(GattServerPostRegisterTest);
};

TEST_F(GattServerTest, RegisterServer) {
  EXPECT_CALL(*mock_handler_, RegisterServer(_))
      .Times(2)
      .WillOnce(Return(BT_STATUS_FAIL))
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // These will be asynchronously populate with a result when the callback
  // executes.
  BLEStatus status = BLE_STATUS_SUCCESS;
  Uuid cb_uuid;
  std::unique_ptr<GattServer> server;
  int callback_count = 0;

  auto callback = [&](BLEStatus in_status, const Uuid& uuid,
                      std::unique_ptr<BluetoothInstance> in_server) {
    status = in_status;
    cb_uuid = uuid;
    server = std::unique_ptr<GattServer>(
        static_cast<GattServer*>(in_server.release()));
    callback_count++;
  };

  Uuid uuid0 = Uuid::GetRandom();

  // HAL returns failure.
  EXPECT_FALSE(factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // HAL returns success.
  EXPECT_TRUE(factory_->RegisterInstance(uuid0, callback));
  EXPECT_EQ(0, callback_count);

  // Calling twice with the same Uuid should fail with no additional calls into
  // the stack.
  EXPECT_FALSE(factory_->RegisterInstance(uuid0, callback));

  testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // Call with a different Uuid while one is pending.
  Uuid uuid1 = Uuid::GetRandom();
  EXPECT_CALL(*mock_handler_, RegisterServer(_))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_TRUE(factory_->RegisterInstance(uuid1, callback));

  // Trigger callback with an unknown Uuid. This should get ignored.
  bluetooth::Uuid hal_uuid = bluetooth::Uuid::GetRandom();
  fake_hal_gatt_iface_->NotifyRegisterServerCallback(0, 0, hal_uuid);
  EXPECT_EQ(0, callback_count);

  // |uuid0| succeeds.
  int server_if0 = 2;  // Pick something that's not 0.
  fake_hal_gatt_iface_->NotifyRegisterServerCallback(BT_STATUS_SUCCESS,
                                                     server_if0, uuid0);

  EXPECT_EQ(1, callback_count);
  ASSERT_TRUE(server.get() != nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_SUCCESS, status);
  EXPECT_EQ(server_if0, server->GetInstanceId());
  EXPECT_EQ(uuid0, server->GetAppIdentifier());
  EXPECT_EQ(uuid0, cb_uuid);

  // The server should unregister itself when deleted.
  EXPECT_CALL(*mock_handler_, UnregisterServer(server_if0))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  server.reset();

  testing::Mock::VerifyAndClearExpectations(mock_handler_.get());

  // |uuid1| fails.
  int server_if1 = 3;
  fake_hal_gatt_iface_->NotifyRegisterServerCallback(BT_STATUS_FAIL, server_if1,
                                                     uuid1);

  EXPECT_EQ(2, callback_count);
  ASSERT_TRUE(server.get() == nullptr);  // Assert to terminate in case of error
  EXPECT_EQ(BLE_STATUS_FAILURE, status);
  EXPECT_EQ(uuid1, cb_uuid);
}

TEST_F(GattServerPostRegisterTest, RequestRead) {
  SetUpTestService();

  TestDelegate test_delegate;
  gatt_server_->SetDelegate(&test_delegate);

  const std::vector<uint8_t> kTestValue = {0x01, 0x02, 0x03};
  const std::vector<uint8_t> kTestValueTooLarge(BTGATT_MAX_ATTR_LEN + 1, 0);
  const std::string kTestAddress0 = "01:23:45:67:89:AB";
  const std::string kTestAddress1 = "CD:EF:01:23:45:67";
  const int kReqId0 = 0;
  const int kReqId1 = 1;
  const int kConnId0 = 1;

  // No pending request.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  RawAddress hal_addr0, hal_addr1;
  ASSERT_TRUE(RawAddress::FromString(kTestAddress0, hal_addr0));
  ASSERT_TRUE(RawAddress::FromString(kTestAddress1, hal_addr1));

  // Send a connection callback. The GattServer should store the connection
  // information and be able to process the incoming read requests for this
  // connection.
  fake_hal_gatt_iface_->NotifyServerConnectionCallback(
      kConnId0, kDefaultServerId, true, hal_addr0);

  // Unknown connection ID shouldn't trigger anything.
  fake_hal_gatt_iface_->NotifyRequestReadCharacteristicCallback(
      kConnId0 + 1, kReqId0, hal_addr0, char_handle_, 0, false);
  EXPECT_EQ(0, test_delegate.char_read_req().count);
  EXPECT_EQ(0, test_delegate.desc_read_req().count);

  // Unknown device address shouldn't trigger anything.
  fake_hal_gatt_iface_->NotifyRequestReadCharacteristicCallback(
      kConnId0, kReqId0, hal_addr1, char_handle_, 0, false);
  EXPECT_EQ(0, test_delegate.char_read_req().count);
  EXPECT_EQ(0, test_delegate.desc_read_req().count);

  // Characteristic and descriptor handles should trigger correct callbacks.
  fake_hal_gatt_iface_->NotifyRequestReadCharacteristicCallback(
      kConnId0, kReqId0, hal_addr0, char_handle_, 0, false);
  EXPECT_EQ(1, test_delegate.char_read_req().count);
  EXPECT_EQ(kTestAddress0, test_delegate.char_read_req().device_address);
  EXPECT_EQ(kReqId0, test_delegate.char_read_req().id);
  EXPECT_EQ(0, test_delegate.char_read_req().offset);
  EXPECT_FALSE(test_delegate.char_read_req().is_long);
  EXPECT_TRUE(char_handle_ == test_delegate.char_read_req().handle);
  EXPECT_EQ(0, test_delegate.desc_read_req().count);

  fake_hal_gatt_iface_->NotifyRequestReadDescriptorCallback(
      kConnId0, kReqId1, hal_addr0, desc_handle_, 2, true);
  EXPECT_EQ(1, test_delegate.char_read_req().count);
  EXPECT_EQ(1, test_delegate.desc_read_req().count);
  EXPECT_EQ(kTestAddress0, test_delegate.desc_read_req().device_address);
  EXPECT_EQ(kReqId1, test_delegate.desc_read_req().id);
  EXPECT_EQ(2, test_delegate.desc_read_req().offset);
  EXPECT_TRUE(test_delegate.desc_read_req().is_long);
  EXPECT_TRUE(desc_handle_ == test_delegate.desc_read_req().handle);

  // Callback with a pending request ID will be ignored.
  fake_hal_gatt_iface_->NotifyRequestReadCharacteristicCallback(
      kConnId0, kReqId0, hal_addr0, char_handle_, 0, false);
  fake_hal_gatt_iface_->NotifyRequestReadCharacteristicCallback(
      kConnId0, kReqId1, hal_addr0, char_handle_, 0, false);
  EXPECT_EQ(1, test_delegate.char_read_req().count);
  EXPECT_EQ(1, test_delegate.desc_read_req().count);

  // Send response for wrong device address.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress1, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  // Send response for a value that's too large.
  EXPECT_FALSE(gatt_server_->SendResponse(
      kTestAddress0, kReqId0, GATT_ERROR_NONE, 0, kTestValueTooLarge));

  EXPECT_CALL(*mock_handler_,
              SendResponse(kConnId0, kReqId0, BT_STATUS_SUCCESS, _))
      .Times(2)
      .WillOnce(Return(BT_STATUS_FAIL))
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // Stack call fails.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  // Successful send response for characteristic.
  EXPECT_TRUE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                         GATT_ERROR_NONE, 0, kTestValue));

  // Characteristic request ID no longer pending.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  EXPECT_CALL(*mock_handler_,
              SendResponse(kConnId0, kReqId1, BT_STATUS_SUCCESS, _))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // Successful send response for descriptor.
  EXPECT_TRUE(gatt_server_->SendResponse(kTestAddress0, kReqId1,
                                         GATT_ERROR_NONE, 0, kTestValue));

  // Descriptor request ID no longer pending.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId1,
                                          GATT_ERROR_NONE, 0, kTestValue));

  gatt_server_->SetDelegate(nullptr);
}

TEST_F(GattServerPostRegisterTest, RequestWrite) {
  SetUpTestService();

  TestDelegate test_delegate;
  gatt_server_->SetDelegate(&test_delegate);

  const std::vector<uint8_t> kTestValue = {0x01, 0x02, 0x03};
  const std::string kTestAddress0 = "01:23:45:67:89:AB";
  const std::string kTestAddress1 = "CD:EF:01:23:45:67";
  const int kReqId0 = 0;
  const int kReqId1 = 1;
  const int kConnId0 = 1;

  // No pending request.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  RawAddress hal_addr0, hal_addr1;
  ASSERT_TRUE(RawAddress::FromString(kTestAddress0, hal_addr0));
  ASSERT_TRUE(RawAddress::FromString(kTestAddress1, hal_addr1));

  // Send a connection callback. The GattServer should store the connection
  // information and be able to process the incoming read requests for this
  // connection.
  fake_hal_gatt_iface_->NotifyServerConnectionCallback(
      kConnId0, kDefaultServerId, true, hal_addr0);

  // Unknown connection ID shouldn't trigger anything.
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0 + 1, kReqId0, hal_addr0, char_handle_, 0, true, false,
      kTestValue);
  EXPECT_EQ(0, test_delegate.char_write_req().count);
  EXPECT_EQ(0, test_delegate.desc_write_req().count);

  // Unknown device address shouldn't trigger anything.
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0, kReqId0, hal_addr1, char_handle_, 0, true, false, kTestValue);
  EXPECT_EQ(0, test_delegate.char_write_req().count);
  EXPECT_EQ(0, test_delegate.desc_write_req().count);

  // Characteristic and descriptor handles should trigger correct callbacks.
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0, kReqId0, hal_addr0, char_handle_, 0, true, false, kTestValue);
  EXPECT_EQ(1, test_delegate.char_write_req().count);
  EXPECT_EQ(kTestAddress0, test_delegate.char_write_req().device_address);
  EXPECT_EQ(kReqId0, test_delegate.char_write_req().id);
  EXPECT_EQ(0, test_delegate.char_write_req().offset);
  EXPECT_EQ(true, test_delegate.char_write_req().need_rsp);
  EXPECT_EQ(false, test_delegate.char_write_req().is_exec);
  EXPECT_EQ(kTestValue, test_delegate.char_write_req().write_value);
  EXPECT_TRUE(char_handle_ == test_delegate.char_write_req().handle);
  EXPECT_EQ(0, test_delegate.desc_write_req().count);

  fake_hal_gatt_iface_->NotifyRequestWriteDescriptorCallback(
      kConnId0, kReqId1, hal_addr0, desc_handle_, 2, true, false, kTestValue);
  EXPECT_EQ(1, test_delegate.char_write_req().count);
  EXPECT_EQ(1, test_delegate.desc_write_req().count);
  EXPECT_EQ(kTestAddress0, test_delegate.desc_write_req().device_address);
  EXPECT_EQ(kReqId1, test_delegate.desc_write_req().id);
  EXPECT_EQ(2, test_delegate.desc_write_req().offset);
  EXPECT_EQ(true, test_delegate.desc_write_req().need_rsp);
  EXPECT_EQ(false, test_delegate.desc_write_req().is_exec);
  EXPECT_EQ(kTestValue, test_delegate.desc_write_req().write_value);
  EXPECT_TRUE(desc_handle_ == test_delegate.desc_write_req().handle);

  // Callback with a pending request ID will be ignored.
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0, kReqId0, hal_addr0, char_handle_, 0, true, false, kTestValue);
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0, kReqId1, hal_addr0, char_handle_, 0, true, false, kTestValue);
  EXPECT_EQ(1, test_delegate.char_write_req().count);
  EXPECT_EQ(1, test_delegate.desc_write_req().count);

  // Send response for wrong device address.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress1, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  EXPECT_CALL(*mock_handler_,
              SendResponse(kConnId0, kReqId0, BT_STATUS_SUCCESS, _))
      .Times(2)
      .WillOnce(Return(BT_STATUS_FAIL))
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // Stack call fails.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  // Successful send response for characteristic.
  EXPECT_TRUE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                         GATT_ERROR_NONE, 0, kTestValue));

  // Characteristic request ID no longer pending.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  EXPECT_CALL(*mock_handler_,
              SendResponse(kConnId0, kReqId1, BT_STATUS_SUCCESS, _))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));

  // Successful send response for descriptor.
  EXPECT_TRUE(gatt_server_->SendResponse(kTestAddress0, kReqId1,
                                         GATT_ERROR_NONE, 0, kTestValue));

  // Descriptor request ID no longer pending.
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId1,
                                          GATT_ERROR_NONE, 0, kTestValue));

  // SendResponse should fail for a "Write Without Response".
  fake_hal_gatt_iface_->NotifyRequestWriteCharacteristicCallback(
      kConnId0, kReqId0, hal_addr0, char_handle_, 0, false, false, kTestValue);
  EXPECT_EQ(false, test_delegate.char_write_req().need_rsp);
  EXPECT_FALSE(gatt_server_->SendResponse(kTestAddress0, kReqId0,
                                          GATT_ERROR_NONE, 0, kTestValue));

  gatt_server_->SetDelegate(nullptr);
}

TEST_F(GattServerPostRegisterTest, SendNotification) {
  SetUpTestService();

  const std::string kTestAddress0 = "01:23:45:67:89:AB";
  const std::string kTestAddress1 = "cd:ef:01:23:45:67";
  const std::string kInvalidAddress = "thingamajig blabbidyboop";
  const int kConnId0 = 0;
  const int kConnId1 = 1;
  std::vector<uint8_t> value;
  RawAddress hal_addr0;
  ASSERT_TRUE(RawAddress::FromString(kTestAddress0, hal_addr0));

  // Set up two connections with the same address.
  fake_hal_gatt_iface_->NotifyServerConnectionCallback(
      kConnId0, kDefaultServerId, true, hal_addr0);
  fake_hal_gatt_iface_->NotifyServerConnectionCallback(
      kConnId1, kDefaultServerId, true, hal_addr0);

  // Set up a test callback.
  GATTError gatt_error;
  int callback_count = 0;
  auto callback = [&](GATTError in_error) {
    gatt_error = in_error;
    callback_count++;
  };

  // Bad device address.
  EXPECT_FALSE(gatt_server_->SendNotification(kInvalidAddress, char_handle_,
                                              false, value, callback));

  // Bad connection.
  EXPECT_FALSE(gatt_server_->SendNotification(kTestAddress1, char_handle_,
                                              false, value, callback));

  // We should get a HAL call for each connection for this address. The calls
  // fail.
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId0, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_FAIL));
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId1, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_FAIL));
  EXPECT_FALSE(gatt_server_->SendNotification(kTestAddress0, char_handle_,
                                              false, value, callback));

  // One of the calls succeeds.
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId0, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId1, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_FAIL));
  EXPECT_TRUE(gatt_server_->SendNotification(kTestAddress0, char_handle_, false,
                                             value, callback));

  // One of the connections is already pending so there should be only one call.
  // This one we send with confirm=true.
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId1, 1, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_TRUE(gatt_server_->SendNotification(kTestAddress0, char_handle_, true,
                                             value, callback));

  // Calls are already pending.
  EXPECT_FALSE(gatt_server_->SendNotification(kTestAddress0, char_handle_, true,
                                              value, callback));

  // Trigger one confirmation callback. We should get calls for two callbacks
  // since we have two separate calls pending.
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId0,
                                                     BT_STATUS_SUCCESS);
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId1,
                                                     BT_STATUS_SUCCESS);
  EXPECT_EQ(2, callback_count);
  EXPECT_EQ(GATT_ERROR_NONE, gatt_error);

  callback_count = 0;

  // Restart. Both calls succeed now.
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId0, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_CALL(*mock_handler_, SendIndication(kDefaultServerId, char_handle_,
                                             kConnId1, 0, value))
      .Times(1)
      .WillOnce(Return(BT_STATUS_SUCCESS));
  EXPECT_TRUE(gatt_server_->SendNotification(kTestAddress0, char_handle_, false,
                                             value, callback));

  // Trigger one confirmation callback. The callback we passed should still be
  // pending. The first callback is for the wrong connection ID.
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId0 + 50,
                                                     BT_STATUS_FAIL);
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId0,
                                                     BT_STATUS_SUCCESS);
  EXPECT_EQ(0, callback_count);

  // This should be ignored since |kConnId0| was already processed.
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId0,
                                                     BT_STATUS_SUCCESS);
  EXPECT_EQ(0, callback_count);

  // Run the callback with failure. Since the previous callback reported
  // success, we should report success.
  fake_hal_gatt_iface_->NotifyIndicationSentCallback(kConnId1,
                                                     BT_STATUS_SUCCESS);
  EXPECT_EQ(1, callback_count);
  EXPECT_EQ(GATT_ERROR_NONE, gatt_error);
}

}  // namespace
}  // namespace bluetooth
