/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "net/netlink_manager.h"

#include <string>
#include <vector>

#include <linux/netlink.h>
#include <poll.h>
#include <sys/socket.h>

#include <android-base/logging.h>
#include <utils/Timers.h>

#include "net/kernel-header-latest/nl80211.h"
#include "net/mlme_event.h"
#include "net/mlme_event_handler.h"
#include "net/nl80211_attribute.h"
#include "net/nl80211_packet.h"

using android::base::unique_fd;
using std::array;
using std::placeholders::_1;
using std::string;
using std::unique_ptr;
using std::vector;

namespace android {
namespace wificond {

namespace {

// netlink.h suggests NLMSG_GOODSIZE to be at most 8192 bytes.
constexpr int kReceiveBufferSize = 8 * 1024;
constexpr uint32_t kBroadcastSequenceNumber = 0;
constexpr int kMaximumNetlinkMessageWaitMilliSeconds = 300;
uint8_t ReceiveBuffer[kReceiveBufferSize];

void AppendPacket(vector<unique_ptr<const NL80211Packet>>* vec,
                  unique_ptr<const NL80211Packet> packet) {
  vec->push_back(std::move(packet));
}

// Convert enum nl80211_chan_width to enum ChannelBandwidth
ChannelBandwidth getBandwidthType(uint32_t bandwidth) {
  switch (bandwidth) {
    case NL80211_CHAN_WIDTH_20_NOHT:
      return BW_20_NOHT;
    case NL80211_CHAN_WIDTH_20:
      return BW_20;
    case NL80211_CHAN_WIDTH_40:
      return BW_40;
    case NL80211_CHAN_WIDTH_80:
      return BW_80;
    case NL80211_CHAN_WIDTH_80P80:
      return BW_80P80;
    case NL80211_CHAN_WIDTH_160:
      return BW_160;
  }
  LOG(ERROR) << "Unknown bandwidth type: " << bandwidth;
  return BW_INVALID;
}

}  // namespace

NetlinkManager::NetlinkManager(EventLoop* event_loop)
    : started_(false),
      event_loop_(event_loop),
      sequence_number_(0) {
}

NetlinkManager::~NetlinkManager() {
}

uint32_t NetlinkManager::GetSequenceNumber() {
  if (++sequence_number_ == kBroadcastSequenceNumber) {
    ++sequence_number_;
  }
  return sequence_number_;
}

void NetlinkManager::ReceivePacketAndRunHandler(int fd) {
  ssize_t len = read(fd, ReceiveBuffer, kReceiveBufferSize);
  if (len == -1) {
    LOG(ERROR) << "Failed to read packet from buffer";
    return;
  }
  if (len == 0) {
    return;
  }
  // There might be multiple message in one datagram payload.
  uint8_t* ptr = ReceiveBuffer;
  while (ptr < ReceiveBuffer + len) {
    // peek at the header.
    if (ptr + sizeof(nlmsghdr) > ReceiveBuffer + len) {
      LOG(ERROR) << "payload is broken.";
      return;
    }
    const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(ptr);
    unique_ptr<NL80211Packet> packet(
        new NL80211Packet(vector<uint8_t>(ptr, ptr + nl_header->nlmsg_len)));
    ptr += nl_header->nlmsg_len;
    if (!packet->IsValid()) {
      LOG(ERROR) << "Receive invalid packet";
      return;
    }
    // Some document says message from kernel should have port id equal 0.
    // However in practice this is not always true so we don't check that.

    uint32_t sequence_number = packet->GetMessageSequence();

    // Handle multicasts.
    if (sequence_number == kBroadcastSequenceNumber) {
      BroadcastHandler(std::move(packet));
      continue;
    }

    auto itr = message_handlers_.find(sequence_number);
    // There is no handler for this sequence number.
    if (itr == message_handlers_.end()) {
      LOG(WARNING) << "No handler for message: " << sequence_number;
      return;
    }
    // A multipart message is terminated by NLMSG_DONE.
    // In this case we don't need to run the handler.
    // NLMSG_NOOP means no operation, message must be discarded.
    uint32_t message_type =  packet->GetMessageType();
    if (message_type == NLMSG_DONE || message_type == NLMSG_NOOP) {
      message_handlers_.erase(itr);
      return;
    }
    if (message_type == NLMSG_OVERRUN) {
      LOG(ERROR) << "Get message overrun notification";
      message_handlers_.erase(itr);
      return;
    }

    // In case we receive a NLMSG_ERROR message:
    // NLMSG_ERROR could be either an error or an ACK.
    // It is an ACK message only when error code field is set to 0.
    // An ACK could be return when we explicitly request that with NLM_F_ACK.
    // An ERROR could be received on NLM_F_ACK or other failure cases.
    // We should still run handler in this case, leaving it for the caller
    // to decide what to do with the packet.

    bool is_multi = packet->IsMulti();
    // Run the handler.
    itr->second(std::move(packet));
    // Remove handler after processing.
    if (!is_multi) {
      message_handlers_.erase(itr);
    }
  }
}

void NetlinkManager::OnNewFamily(unique_ptr<const NL80211Packet> packet) {
  if (packet->GetMessageType() != GENL_ID_CTRL) {
    LOG(ERROR) << "Wrong message type for new family message";
    return;
  }
  if (packet->GetCommand() != CTRL_CMD_NEWFAMILY) {
    LOG(ERROR) << "Wrong command for new family message";
    return;
  }
  uint16_t family_id;
  if (!packet->GetAttributeValue(CTRL_ATTR_FAMILY_ID, &family_id)) {
    LOG(ERROR) << "Failed to get family id";
    return;
  }
  string family_name;
  if (!packet->GetAttributeValue(CTRL_ATTR_FAMILY_NAME, &family_name)) {
    LOG(ERROR) << "Failed to get family name";
    return;
  }
  if (family_name != NL80211_GENL_NAME) {
    LOG(WARNING) << "Ignoring none nl80211 netlink families";
  }
  MessageType nl80211_type(family_id);
  message_types_[family_name] = nl80211_type;
  // Exract multicast groups.
  NL80211NestedAttr multicast_groups(0);
  if (packet->GetAttribute(CTRL_ATTR_MCAST_GROUPS, &multicast_groups)) {
    vector<NL80211NestedAttr> groups;
    if (!multicast_groups.GetListOfNestedAttributes(&groups)) {
      return;
    }
    for (auto& group : groups) {
      string group_name;
      uint32_t group_id = 0;
      if (!group.GetAttributeValue(CTRL_ATTR_MCAST_GRP_NAME, &group_name)) {
        LOG(ERROR) << "Failed to get group name";
        continue;
      }
      if (!group.GetAttributeValue(CTRL_ATTR_MCAST_GRP_ID, &group_id)) {
        LOG(ERROR) << "Failed to get group id";
        continue;
      }
      message_types_[family_name].groups[group_name] = group_id;
    }
  }
}

bool NetlinkManager::Start() {
  if (started_) {
    LOG(DEBUG) << "NetlinkManager is already started";
    return true;
  }
  bool setup_rt = SetupSocket(&sync_netlink_fd_);
  if (!setup_rt) {
    LOG(ERROR) << "Failed to setup synchronous netlink socket";
    return false;
  }

  setup_rt = SetupSocket(&async_netlink_fd_);
  if (!setup_rt) {
    LOG(ERROR) << "Failed to setup asynchronous netlink socket";
    return false;
  }

  // Request family id for nl80211 messages.
  if (!DiscoverFamilyId()) {
    return false;
  }
  // Watch socket.
  if (!WatchSocket(&async_netlink_fd_)) {
    return false;
  }
  // Subscribe kernel NL80211 broadcast of regulatory changes.
  if (!SubscribeToEvents(NL80211_MULTICAST_GROUP_REG)) {
    return false;
  }
  // Subscribe kernel NL80211 broadcast of scanning events.
  if (!SubscribeToEvents(NL80211_MULTICAST_GROUP_SCAN)) {
    return false;
  }
  // Subscribe kernel NL80211 broadcast of MLME events.
  if (!SubscribeToEvents(NL80211_MULTICAST_GROUP_MLME)) {
    return false;
  }

  started_ = true;
  return true;
}

bool NetlinkManager::IsStarted() const {
  return started_;
}

bool NetlinkManager::RegisterHandlerAndSendMessage(
    const NL80211Packet& packet,
    std::function<void(unique_ptr<const NL80211Packet>)> handler) {
  if (packet.IsDump()) {
    LOG(ERROR) << "Do not use asynchronous interface for dump request !";
    return false;
  }
  if (!SendMessageInternal(packet, async_netlink_fd_.get())) {
    return false;
  }
  message_handlers_[packet.GetMessageSequence()] = handler;
  return true;
}

bool NetlinkManager::SendMessageAndGetResponses(
    const NL80211Packet& packet,
    vector<unique_ptr<const NL80211Packet>>* response) {
  if (!SendMessageInternal(packet, sync_netlink_fd_.get())) {
    return false;
  }
  // Polling netlink socket, waiting for GetFamily reply.
  struct pollfd netlink_output;
  memset(&netlink_output, 0, sizeof(netlink_output));
  netlink_output.fd = sync_netlink_fd_.get();
  netlink_output.events = POLLIN;

  uint32_t sequence = packet.GetMessageSequence();

  int time_remaining = kMaximumNetlinkMessageWaitMilliSeconds;
  // Multipart messages may come with seperated datagrams, ending with a
  // NLMSG_DONE message.
  // ReceivePacketAndRunHandler() will remove the handler after receiving a
  // NLMSG_DONE message.
  message_handlers_[sequence] = std::bind(AppendPacket, response, _1);

  while (time_remaining > 0 &&
      message_handlers_.find(sequence) != message_handlers_.end()) {
    nsecs_t interval = systemTime(SYSTEM_TIME_MONOTONIC);
    int poll_return = poll(&netlink_output,
                           1,
                           time_remaining);

    if (poll_return == 0) {
      LOG(ERROR) << "Failed to poll netlink fd: time out ";
      message_handlers_.erase(sequence);
      return false;
    } else if (poll_return == -1) {
      PLOG(ERROR) << "Failed to poll netlink fd";
      message_handlers_.erase(sequence);
      return false;
    }
    ReceivePacketAndRunHandler(sync_netlink_fd_.get());
    interval = systemTime(SYSTEM_TIME_MONOTONIC) - interval;
    time_remaining -= static_cast<int>(ns2ms(interval));
  }
  if (time_remaining <= 0) {
    LOG(ERROR) << "Timeout waiting for netlink reply messages";
    message_handlers_.erase(sequence);
    return false;
  }
  return true;
}

bool NetlinkManager::SendMessageAndGetSingleResponse(
    const NL80211Packet& packet,
    unique_ptr<const NL80211Packet>* response) {
  unique_ptr<const NL80211Packet> response_or_error;
  if (!SendMessageAndGetSingleResponseOrError(packet, &response_or_error)) {
    return false;
  }
  if (response_or_error->GetMessageType() == NLMSG_ERROR) {
    // We use ERROR because we are not expecting to receive a ACK here.
    // In that case the caller should use |SendMessageAndGetAckOrError|.
    LOG(ERROR) << "Received error message: "
               << strerror(response_or_error->GetErrorCode());
    return false;
  }
  *response = std::move(response_or_error);
  return true;
}

bool NetlinkManager::SendMessageAndGetSingleResponseOrError(
    const NL80211Packet& packet,
    unique_ptr<const NL80211Packet>* response) {
  vector<unique_ptr<const NL80211Packet>> response_vec;
  if (!SendMessageAndGetResponses(packet, &response_vec)) {
    return false;
  }
  if (response_vec.size() != 1) {
    LOG(ERROR) << "Unexpected response size: " << response_vec.size();
    return false;
  }

  *response = std::move(response_vec[0]);
  return true;
}

bool NetlinkManager::SendMessageAndGetAckOrError(const NL80211Packet& packet,
                                                 int* error_code) {
  unique_ptr<const NL80211Packet> response;
  if (!SendMessageAndGetSingleResponseOrError(packet, &response)) {
    return false;
  }
  uint16_t type = response->GetMessageType();
  if (type != NLMSG_ERROR) {
    LOG(ERROR) << "Receive unexpected message type :" << type;
    return false;
  }

  *error_code = response->GetErrorCode();
  return true;
}

bool NetlinkManager::SendMessageAndGetAck(const NL80211Packet& packet) {
  int error_code;
  if (!SendMessageAndGetAckOrError(packet, &error_code)) {
    return false;
  }
  if (error_code != 0) {
    LOG(ERROR) << "Received error messsage: " << strerror(error_code);
    return false;
  }

  return true;
}

bool NetlinkManager::SendMessageInternal(const NL80211Packet& packet, int fd) {
  const vector<uint8_t>& data = packet.GetConstData();
  ssize_t bytes_sent =
      TEMP_FAILURE_RETRY(send(fd, data.data(), data.size(), 0));
  if (bytes_sent == -1) {
    PLOG(ERROR) << "Failed to send netlink message";
    return false;
  }
  return true;
}

bool NetlinkManager::SetupSocket(unique_fd* netlink_fd) {
  struct sockaddr_nl nladdr;

  memset(&nladdr, 0, sizeof(nladdr));
  nladdr.nl_family = AF_NETLINK;

  netlink_fd->reset(
      socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_GENERIC));
  if (netlink_fd->get() < 0) {
    PLOG(ERROR) << "Failed to create netlink socket";
    return false;
  }
  // Set maximum receive buffer size.
  // Datagram which is larger than this size will be discarded.
  if (setsockopt(netlink_fd->get(),
                 SOL_SOCKET,
                 SO_RCVBUFFORCE,
                 &kReceiveBufferSize,
                 sizeof(kReceiveBufferSize)) < 0) {
    PLOG(ERROR) << "Failed to set uevent socket SO_RCVBUFFORCE option";
    return false;
  }
  if (bind(netlink_fd->get(),
           reinterpret_cast<struct sockaddr*>(&nladdr),
           sizeof(nladdr)) < 0) {
    PLOG(ERROR) << "Failed to bind netlink socket";
    return false;
  }
  return true;
}

bool NetlinkManager::WatchSocket(unique_fd* netlink_fd) {
  // Watch socket
  bool watch_fd_rt = event_loop_->WatchFileDescriptor(
      netlink_fd->get(),
      EventLoop::kModeInput,
      std::bind(&NetlinkManager::ReceivePacketAndRunHandler, this, _1));
  if (!watch_fd_rt) {
    LOG(ERROR) << "Failed to watch fd: " << netlink_fd->get();
    return false;
  }
  return true;
}

uint16_t NetlinkManager::GetFamilyId() {
  return message_types_[NL80211_GENL_NAME].family_id;
}

bool NetlinkManager::DiscoverFamilyId() {
  NL80211Packet get_family_request(GENL_ID_CTRL,
                                   CTRL_CMD_GETFAMILY,
                                   GetSequenceNumber(),
                                   getpid());
  NL80211Attr<string> family_name(CTRL_ATTR_FAMILY_NAME, NL80211_GENL_NAME);
  get_family_request.AddAttribute(family_name);
  unique_ptr<const NL80211Packet> response;
  if (!SendMessageAndGetSingleResponse(get_family_request, &response)) {
    LOG(ERROR) << "Failed to get NL80211 family info";
    return false;
  }
  OnNewFamily(std::move(response));
  if (message_types_.find(NL80211_GENL_NAME) == message_types_.end()) {
    LOG(ERROR) << "Failed to get NL80211 family id";
    return false;
  }
  return true;
}

bool NetlinkManager::SubscribeToEvents(const string& group) {
  auto groups = message_types_[NL80211_GENL_NAME].groups;
  if (groups.find(group) == groups.end()) {
    LOG(ERROR) << "Failed to subscribe: group " << group << " doesn't exist";
    return false;
  }
  uint32_t group_id = groups[group];
  int err = setsockopt(async_netlink_fd_.get(),
                       SOL_NETLINK,
                       NETLINK_ADD_MEMBERSHIP,
                       &group_id,
                       sizeof(group_id));
  if (err < 0) {
    PLOG(ERROR) << "Failed to setsockopt";
    return false;
  }
  return true;
}

void NetlinkManager::BroadcastHandler(unique_ptr<const NL80211Packet> packet) {
  if (packet->GetMessageType() != GetFamilyId()) {
    LOG(ERROR) << "Wrong family id for multicast message";
    return;
  }
  uint32_t command = packet->GetCommand();

  if (command == NL80211_CMD_NEW_SCAN_RESULTS ||
      // Scan was aborted, for unspecified reasons.partial scan results may be
      // available.
      command == NL80211_CMD_SCAN_ABORTED) {
    OnScanResultsReady(std::move(packet));
    return;
  }

  if (command == NL80211_CMD_SCHED_SCAN_RESULTS ||
      command == NL80211_CMD_SCHED_SCAN_STOPPED) {
    OnSchedScanResultsReady(std::move(packet));
    return;
  }


  // Driver which supports SME uses both NL80211_CMD_AUTHENTICATE and
  // NL80211_CMD_ASSOCIATE, otherwise it uses NL80211_CMD_CONNECT
  // to notify a combination of authentication and association processses.
  // Currently we monitor CONNECT/ASSOCIATE/ROAM event for up-to-date
  // frequency and bssid.
  // TODO(nywang): Handle other MLME events, which help us track the
  // connection state better.
  if (command == NL80211_CMD_CONNECT ||
      command == NL80211_CMD_ASSOCIATE ||
      command == NL80211_CMD_ROAM ||
      command == NL80211_CMD_DISCONNECT ||
      command == NL80211_CMD_DISASSOCIATE) {
      OnMlmeEvent(std::move(packet));
     return;
  }
  if (command == NL80211_CMD_REG_CHANGE) {
    OnRegChangeEvent(std::move(packet));
    return;
  }
  // Station eventsFor AP mode.
  if (command == NL80211_CMD_NEW_STATION ||
      command == NL80211_CMD_DEL_STATION) {
    uint32_t if_index;
    if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
      LOG(WARNING) << "Failed to get interface index from station event";
      return;
    }
    const auto handler = on_station_event_handler_.find(if_index);
    if (handler != on_station_event_handler_.end()) {
      array<uint8_t, ETH_ALEN> mac_address;
      if (!packet->GetAttributeValue(NL80211_ATTR_MAC, &mac_address)) {
        LOG(WARNING) << "Failed to get mac address from station event";
        return;
      }
      if (command == NL80211_CMD_NEW_STATION) {
        handler->second(NEW_STATION, mac_address);
      } else {
        handler->second(DEL_STATION, mac_address);
      }
    }
    return;
  }
  if (command == NL80211_CMD_CH_SWITCH_NOTIFY) {
    OnChannelSwitchEvent(std::move(packet));
    return;
  }
  if (command == NL80211_CMD_FRAME_TX_STATUS) {
    OnFrameTxStatusEvent(std::move(packet));
    return;
  }
}

void NetlinkManager::OnRegChangeEvent(unique_ptr<const NL80211Packet> packet) {
  uint8_t reg_type;
  if (!packet->GetAttributeValue(NL80211_ATTR_REG_TYPE, &reg_type)) {
    LOG(ERROR) << "Failed to get NL80211_ATTR_REG_TYPE";
  }

  string country_code;
  // NL80211_REGDOM_TYPE_COUNTRY means the regulatory domain set is one that
  // pertains to a specific country
  if (reg_type == NL80211_REGDOM_TYPE_COUNTRY) {
    if (!packet->GetAttributeValue(NL80211_ATTR_REG_ALPHA2, &country_code)) {
      LOG(ERROR) << "Failed to get NL80211_ATTR_REG_ALPHA2";
      return;
    }
  } else if (reg_type == NL80211_REGDOM_TYPE_WORLD ||
      reg_type == NL80211_REGDOM_TYPE_CUSTOM_WORLD ||
      reg_type == NL80211_REGDOM_TYPE_INTERSECTION) {
    // NL80211_REGDOM_TYPE_WORLD refers to the world regulartory domain.
    // NL80211_REGDOM_TYPE_CUSTOM_WORLD refers to the driver specific world
    // regulartory domain.
    // NL80211_REGDOM_TYPE_INTERSECTION refers to an intersection between two
    // regulatory domains:
    // The previously set regulatory domain on the system and the last accepted
    // regulatory domain request to be processed.
    country_code = "";
  } else {
    LOG(ERROR) << "Unknown type of regulatory domain change: " << (int)reg_type;
    return;
  }

  for (const auto& handler : on_reg_domain_changed_handler_) {
    handler.second(country_code);
  }
}

void NetlinkManager::OnMlmeEvent(unique_ptr<const NL80211Packet> packet) {
  uint32_t if_index;

  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
    LOG(ERROR) << "Failed to get interface index from a MLME event message";
    return;
  }
  const auto handler = on_mlme_event_handler_.find(if_index);
  if (handler == on_mlme_event_handler_.end()) {
    LOG(DEBUG) << "No handler for mlme event from interface"
               << " with index: " << if_index;
    return;
  }
  uint32_t command = packet->GetCommand();
  if (command == NL80211_CMD_CONNECT) {
    auto event = MlmeConnectEvent::InitFromPacket(packet.get());
    if (event != nullptr) {
      handler->second->OnConnect(std::move(event));
    }
    return;
  }
  if (command == NL80211_CMD_ASSOCIATE) {
    auto event = MlmeAssociateEvent::InitFromPacket(packet.get());
    if (event != nullptr) {
      handler->second->OnAssociate(std::move(event));
    }
    return;
  }
  if (command == NL80211_CMD_ROAM) {
    auto event = MlmeRoamEvent::InitFromPacket(packet.get());
    if (event != nullptr) {
      handler->second->OnRoam(std::move(event));
    }
    return;
  }
  if (command == NL80211_CMD_DISCONNECT) {
    auto event = MlmeDisconnectEvent::InitFromPacket(packet.get());
    if (event != nullptr) {
      handler->second->OnDisconnect(std::move(event));
    }
    return;
  }
  if (command == NL80211_CMD_DISASSOCIATE) {
    auto event = MlmeDisassociateEvent::InitFromPacket(packet.get());
    if (event != nullptr) {
      handler->second->OnDisassociate(std::move(event));
    }
    return;
  }

}

void NetlinkManager::OnSchedScanResultsReady(unique_ptr<const NL80211Packet> packet) {
  uint32_t if_index;
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
    LOG(ERROR) << "Failed to get interface index from scan result notification";
    return;
  }

  const auto handler = on_sched_scan_result_ready_handler_.find(if_index);
  if (handler == on_sched_scan_result_ready_handler_.end()) {
    LOG(DEBUG) << "No handler for scheduled scan result notification from"
               << " interface with index: " << if_index;
    return;
  }
  // Run scan result notification handler.
  handler->second(if_index, packet->GetCommand() == NL80211_CMD_SCHED_SCAN_STOPPED);
}

void NetlinkManager::OnScanResultsReady(unique_ptr<const NL80211Packet> packet) {
  uint32_t if_index;
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
    LOG(ERROR) << "Failed to get interface index from scan result notification";
    return;
  }
  bool aborted = false;
  if (packet->GetCommand() == NL80211_CMD_SCAN_ABORTED) {
    aborted = true;
  }

  const auto handler = on_scan_result_ready_handler_.find(if_index);
  if (handler == on_scan_result_ready_handler_.end()) {
    LOG(WARNING) << "No handler for scan result notification from interface"
                 << " with index: " << if_index;
    return;
  }

  vector<vector<uint8_t>> ssids;
  NL80211NestedAttr ssids_attr(0);
  if (!packet->GetAttribute(NL80211_ATTR_SCAN_SSIDS, &ssids_attr)) {
    if (!aborted) {
      LOG(WARNING) << "Failed to get scan ssids from scan result notification";
    }
  } else {
    if (!ssids_attr.GetListOfAttributeValues(&ssids)) {
      return;
    }
  }
  vector<uint32_t> freqs;
  NL80211NestedAttr freqs_attr(0);
  if (!packet->GetAttribute(NL80211_ATTR_SCAN_FREQUENCIES, &freqs_attr)) {
    if (!aborted) {
      LOG(WARNING) << "Failed to get scan freqs from scan result notification";
    }
  } else {
    if (!freqs_attr.GetListOfAttributeValues(&freqs)) {
      return;
    }
  }
  // Run scan result notification handler.
  handler->second(if_index, aborted, ssids, freqs);
}

void NetlinkManager::OnChannelSwitchEvent(unique_ptr<const NL80211Packet> packet) {
    uint32_t if_index = 0;
    if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
      LOG(WARNING) << "Failed to get NL80211_ATTR_IFINDEX"
                   << "from channel switch event";
      return;
    }
    uint32_t frequency = 0;
    if (!packet->GetAttributeValue(NL80211_ATTR_WIPHY_FREQ, &frequency)) {
      LOG(WARNING) << "Failed to get NL80211_ATTR_WIPHY_FREQ"
                   << "from channel switch event";
      return;
    }
    uint32_t bandwidth = 0;
    if (!packet->GetAttributeValue(NL80211_ATTR_CHANNEL_WIDTH, &bandwidth)) {
      LOG(WARNING) << "Failed to get NL80211_ATTR_CHANNEL_WIDTH"
                   << "from channel switch event";
      return;
    }

    const auto handler = on_channel_switch_event_handler_.find(if_index);
    if (handler != on_channel_switch_event_handler_.end()) {
      handler->second(frequency, getBandwidthType(bandwidth));
    }
}

void NetlinkManager::OnFrameTxStatusEvent(
    unique_ptr<const NL80211Packet> packet) {

  uint32_t if_index;
  if (!packet->GetAttributeValue(NL80211_ATTR_IFINDEX, &if_index)) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_IFINDEX"
                 << "from NL80211_CMD_FRAME_TX_STATUS event";
    return;
  }

  uint64_t cookie;
  if (!packet->GetAttributeValue(NL80211_ATTR_COOKIE, &cookie)) {
    LOG(WARNING) << "Failed to get NL80211_ATTR_COOKIE"
                 << "from NL80211_CMD_FRAME_TX_STATUS event";
    return;
  }

  bool was_acked = packet->HasAttribute(NL80211_ATTR_ACK);

  const auto handler = on_frame_tx_status_event_handler_.find(if_index);
  if (handler != on_frame_tx_status_event_handler_.end()) {
    handler->second(cookie, was_acked);
  }
}

void NetlinkManager::SubscribeStationEvent(
    uint32_t interface_index,
    OnStationEventHandler handler) {
  on_station_event_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeStationEvent(uint32_t interface_index) {
  on_station_event_handler_.erase(interface_index);
}

void NetlinkManager::SubscribeChannelSwitchEvent(
      uint32_t interface_index,
      OnChannelSwitchEventHandler handler) {
  on_channel_switch_event_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeChannelSwitchEvent(uint32_t interface_index) {
  on_channel_switch_event_handler_.erase(interface_index);
}


void NetlinkManager::SubscribeRegDomainChange(
    uint32_t wiphy_index,
    OnRegDomainChangedHandler handler) {
  on_reg_domain_changed_handler_[wiphy_index] = handler;
}

void NetlinkManager::UnsubscribeRegDomainChange(uint32_t wiphy_index) {
  on_reg_domain_changed_handler_.erase(wiphy_index);
}

void NetlinkManager::SubscribeScanResultNotification(
    uint32_t interface_index,
    OnScanResultsReadyHandler handler) {
  on_scan_result_ready_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeScanResultNotification(
    uint32_t interface_index) {
  on_scan_result_ready_handler_.erase(interface_index);
}

void NetlinkManager::SubscribeMlmeEvent(uint32_t interface_index,
                                        MlmeEventHandler* handler) {
  on_mlme_event_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeMlmeEvent(uint32_t interface_index) {
  on_mlme_event_handler_.erase(interface_index);
}

void NetlinkManager::SubscribeSchedScanResultNotification(
      uint32_t interface_index,
      OnSchedScanResultsReadyHandler handler) {
  on_sched_scan_result_ready_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeSchedScanResultNotification(
    uint32_t interface_index) {
  on_sched_scan_result_ready_handler_.erase(interface_index);
}

void NetlinkManager::SubscribeFrameTxStatusEvent(
    uint32_t interface_index, OnFrameTxStatusEventHandler handler) {
  on_frame_tx_status_event_handler_[interface_index] = handler;
}

void NetlinkManager::UnsubscribeFrameTxStatusEvent(uint32_t interface_index) {
  on_frame_tx_status_event_handler_.erase(interface_index);
}

}  // namespace wificond
}  // namespace android
