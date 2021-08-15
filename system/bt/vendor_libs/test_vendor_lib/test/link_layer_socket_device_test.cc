/*
 * Copyright 2016 The Android Open Source Project
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

#include "model/devices/link_layer_socket_device.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <cstring>
#include <vector>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/link.h"
#include "model/setup/async_manager.h"
#include "packets/link_layer/command_view.h"
#include "packets/link_layer/link_layer_packet_builder.h"
#include "packets/link_layer/link_layer_packet_view.h"

std::vector<uint8_t> count = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
};

using test_vendor_lib::packets::CommandBuilder;
using test_vendor_lib::packets::CommandView;
using test_vendor_lib::packets::LinkLayerPacketBuilder;
using test_vendor_lib::packets::LinkLayerPacketView;
using test_vendor_lib::packets::PacketView;
using test_vendor_lib::packets::View;

static const size_t kMaxConnections = 300;

namespace test_vendor_lib {

class LinkLayerSocketDeviceTest : public ::testing::Test {
 public:
  static const uint16_t kPort = 6123;

 protected:
  class MockPhyLayer : public PhyLayer {
   public:
    MockPhyLayer(const std::function<void(std::shared_ptr<LinkLayerPacketBuilder>)>& on_receive)
        : PhyLayer(Phy::Type::LOW_ENERGY, 0, [](LinkLayerPacketView) {}), on_receive_(on_receive) {}
    virtual void Send(const std::shared_ptr<LinkLayerPacketBuilder> packet) override {
      on_receive_(packet);
    }
    virtual void Receive(LinkLayerPacketView) override {}
    virtual void TimerTick() override {}

   private:
    std::function<void(std::shared_ptr<LinkLayerPacketBuilder>)> on_receive_;
  };

  int StartServer() {
    struct sockaddr_in serv_addr;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_FALSE(fd < 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(kPort);
    int reuse_flag = 1;
    EXPECT_FALSE(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(reuse_flag)) < 0);
    EXPECT_FALSE(bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);

    listen(fd, 1);
    return fd;
  }

  int AcceptConnection(int fd) {
    return accept(fd, NULL, NULL);
  }

  void ValidatePacket(size_t index, bool at_server, std::shared_ptr<LinkLayerPacketBuilder> received) {
    /* Convert the Builder into a View */
    std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
    std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
    received->Serialize(it);
    LinkLayerPacketView received_view = LinkLayerPacketView::Create(packet_ptr);

    /* Validate received packet */
    ASSERT_EQ(received_view.GetSourceAddress(), source_);
    ASSERT_EQ(received_view.GetDestinationAddress(), dest_);
    ASSERT_EQ(Link::PacketType::COMMAND, received_view.GetType());
    CommandView command_view = CommandView::GetCommand(received_view);
    if (at_server) {
      ASSERT_EQ(client_opcodes_[index], command_view.GetOpcode());
    } else {
      ASSERT_EQ(server_opcodes_[index], command_view.GetOpcode());
    }
    auto args_itr = command_view.GetData();
    ASSERT_EQ(args_itr.NumBytesRemaining(), count.size());
    for (size_t i = 0; i < count.size(); i++) {
      ASSERT_EQ(*args_itr++, count[i]);
    }
    if (at_server) {
      validated_client_packets_[index]++;
    } else {
      validated_server_packets_[index]++;
    }
  }

  void SetUp() override {
    servers_.reserve(kMaxConnections);
    clients_.reserve(kMaxConnections);
    socket_fd_ = StartServer();

    async_manager_.WatchFdForNonBlockingReads(socket_fd_, [this](int fd) {
      int connection_fd = AcceptConnection(fd);
      ASSERT_GE(connection_fd, 0);
      size_t index = servers_.size();
      servers_.emplace_back(connection_fd, Phy::Type::LOW_ENERGY);
      ASSERT_EQ(servers_.size() - 1, index) << "Race condition";
      std::shared_ptr<MockPhyLayer> mock_phy = std::make_shared<MockPhyLayer>(
          [this, index](std::shared_ptr<LinkLayerPacketBuilder> received) { ValidatePacket(index, true, received); });
      servers_[index].RegisterPhyLayer(mock_phy);
    });
  }

  void TearDown() override {
    async_manager_.StopWatchingFileDescriptor(socket_fd_);
    close(socket_fd_);
  }

  int ConnectClient() {
    int socket_cli_fd = socket(AF_INET, SOCK_STREAM, 0);
    EXPECT_FALSE(socket_cli_fd < 0);

    struct hostent* server;
    server = gethostbyname("localhost");
    EXPECT_FALSE(server == NULL);

    struct sockaddr_in serv_addr;
    memset((void*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(kPort);

    int result = connect(socket_cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    EXPECT_FALSE(result < 0);

    EXPECT_GE(socket_cli_fd, 0);

    return socket_cli_fd;
  }

  void ValidateConnection(size_t pair_id) {
    ASSERT_GT(clients_.size(), pair_id);
    ASSERT_GT(servers_.size(), pair_id);
  }

  size_t CreateConnection() {
    int fd = ConnectClient();
    size_t index = clients_.size();
    clients_.emplace_back(fd, Phy::Type::LOW_ENERGY);
    std::shared_ptr<MockPhyLayer> mock_phy = std::make_shared<MockPhyLayer>(
        [this, index](std::shared_ptr<LinkLayerPacketBuilder> received) { ValidatePacket(index, false, received); });
    clients_[index].RegisterPhyLayer(mock_phy);
    for (size_t timeout = 10; timeout > 0 && clients_.size() > servers_.size(); timeout--) {
      sleep(0);  // Wait for server to be created
    }
    ValidateConnection(index);
    return index;
  }

  LinkLayerPacketView NextPacket() {
    std::shared_ptr<std::vector<uint8_t>> count_shared = std::make_shared<std::vector<uint8_t>>(count);
    View count_view(count_shared, 0, count_shared->size());
    PacketView<true> args({count_view});
    auto builder = CommandBuilder::Create(packet_id_++, args);
    auto wrapped_command = LinkLayerPacketBuilder::WrapCommand(std::move(builder), source_, dest_);
    std::shared_ptr<std::vector<uint8_t>> packet_ptr = std::make_shared<std::vector<uint8_t>>();
    std::back_insert_iterator<std::vector<uint8_t>> it(*packet_ptr);
    wrapped_command->Serialize(it);
    LinkLayerPacketView view = LinkLayerPacketView::Create(packet_ptr);
    return view;
  }

  void SendFromClient(size_t pair_id) {
    ASSERT_GT(clients_.size(), pair_id);
    LinkLayerPacketView view = NextPacket();
    client_opcodes_[pair_id] = CommandView::GetCommand(view).GetOpcode();
    clients_[pair_id].IncomingPacket(view);
  }

  void SendFromServer(size_t pair_id) {
    ASSERT_GT(servers_.size(), pair_id);
    LinkLayerPacketView view = NextPacket();
    server_opcodes_[pair_id] = CommandView::GetCommand(view).GetOpcode();
    servers_[pair_id].IncomingPacket(view);
  }

  void ReadFromClient(size_t pair_id) {
    ASSERT_GT(clients_.size(), pair_id);
    size_t validated_packets = validated_server_packets_[pair_id];
    for (size_t tries = 0; tries < 10 && validated_server_packets_[pair_id] == validated_packets; tries++) {
      clients_[pair_id].TimerTick();
    }
    ASSERT_EQ(validated_server_packets_[pair_id], validated_packets + 1);
  }

  void ReadFromServer(size_t pair_id) {
    ASSERT_GT(servers_.size(), pair_id);
    size_t validated_packets = validated_client_packets_[pair_id];
    for (size_t tries = 0; tries < 10 && validated_client_packets_[pair_id] == validated_packets; tries++) {
      servers_[pair_id].TimerTick();
    }
    ASSERT_EQ(validated_client_packets_[pair_id], validated_packets + 1);
  }

 private:
  uint16_t packet_id_{1};
  AsyncManager async_manager_;
  int socket_fd_;
  std::vector<LinkLayerSocketDevice> servers_;
  std::vector<LinkLayerSocketDevice> clients_;
  uint16_t server_opcodes_[kMaxConnections]{0};
  uint16_t client_opcodes_[kMaxConnections]{0};
  size_t validated_server_packets_[kMaxConnections]{0};
  size_t validated_client_packets_[kMaxConnections]{0};
  Address source_{{1, 2, 3, 4, 5, 6}};
  Address dest_{{6, 5, 4, 3, 2, 1}};
};

TEST_F(LinkLayerSocketDeviceTest, TestClientFirst) {
  size_t pair_id = CreateConnection();
  ASSERT_EQ(pair_id, 0u);
  ValidateConnection(pair_id);

  SendFromClient(pair_id);
  ReadFromServer(pair_id);
}

TEST_F(LinkLayerSocketDeviceTest, TestServerFirst) {
  size_t pair_id = CreateConnection();
  ASSERT_EQ(pair_id, 0u);

  SendFromServer(pair_id);
  ReadFromClient(pair_id);
}

TEST_F(LinkLayerSocketDeviceTest, TestMultiplePackets) {
  static const int num_packets = 30;
  size_t pair_id = CreateConnection();
  ASSERT_EQ(pair_id, 0u);
  for (int i = 0; i < num_packets; i++) {
    SendFromClient(pair_id);
    SendFromServer(pair_id);
    ReadFromServer(pair_id);
    ReadFromClient(pair_id);
  }
}

TEST_F(LinkLayerSocketDeviceTest, TestMultipleConnectionsFromServer) {
  static size_t last_pair_id = -1;
  size_t pair_id;
  for (size_t i = 0; i < kMaxConnections; i++) {
    pair_id = CreateConnection();
    ASSERT_EQ(pair_id, last_pair_id + 1);
    last_pair_id = pair_id;
    SendFromServer(pair_id);
    ReadFromClient(pair_id);
  }
}

TEST_F(LinkLayerSocketDeviceTest, TestMultipleConnectionsFromClient) {
  for (size_t i = 0; i < kMaxConnections; i++) {
    size_t pair_id = CreateConnection();
    ASSERT_EQ(pair_id, i);
    SendFromClient(pair_id);
    ReadFromServer(pair_id);
  }
}

TEST_F(LinkLayerSocketDeviceTest, TestMultipleConnections) {
  for (size_t i = 0; i < kMaxConnections; i++) {
    size_t pair_id = CreateConnection();
    ASSERT_EQ(pair_id, i);
    SendFromClient(pair_id);
    SendFromServer(pair_id);
    ReadFromClient(pair_id);
    ReadFromServer(pair_id);
  }
}

}  // namespace test_vendor_lib
