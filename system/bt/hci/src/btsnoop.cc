/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#define LOG_TAG "bt_snoop"

#include <mutex>

#include <arpa/inet.h>
#include <base/logging.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <unistd.h>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "bt_types.h"
#include "common/time_util.h"
#include "hci/include/btsnoop.h"
#include "hci/include/btsnoop_mem.h"
#include "hci_layer.h"
#include "internal_include/bt_trace.h"
#include "osi/include/log.h"
#include "osi/include/properties.h"
#include "stack/include/hcimsgs.h"
#include "stack/include/rfcdefs.h"
#include "stack/l2cap/l2c_int.h"
#include "stack_config.h"

// The number of of packets per btsnoop file before we rotate to the next
// file. As of right now there are two snoop files that are rotated through.
// The size can be dynamically configured by seting the relevant system
// property
#define DEFAULT_BTSNOOP_SIZE 0xffff

#define IS_DEBUGGABLE_PROPERTY "ro.debuggable"

#define BTSNOOP_LOG_MODE_PROPERTY "persist.bluetooth.btsnooplogmode"
#define BTSNOOP_DEFAULT_MODE_PROPERTY "persist.bluetooth.btsnoopdefaultmode"
#define BTSNOOP_MODE_DISABLED "disabled"
#define BTSNOOP_MODE_FILTERED "filtered"
#define BTSNOOP_MODE_FULL "full"

#define BTSNOOP_PATH_PROPERTY "persist.bluetooth.btsnooppath"
#define DEFAULT_BTSNOOP_PATH "/data/misc/bluetooth/logs/btsnoop_hci.log"
#define BTSNOOP_MAX_PACKETS_PROPERTY "persist.bluetooth.btsnoopsize"

typedef enum {
  kCommandPacket = 1,
  kAclPacket = 2,
  kScoPacket = 3,
  kEventPacket = 4
} packet_type_t;

// Epoch in microseconds since 01/01/0000.
static const uint64_t BTSNOOP_EPOCH_DELTA = 0x00dcddb30f2f8000ULL;

// Number of bytes into a packet where you can find the value for a channel.
static const size_t ACL_CHANNEL_OFFSET = 0;
static const size_t L2C_CHANNEL_OFFSET = 6;
static const size_t RFC_CHANNEL_OFFSET = 8;
static const size_t RFC_EVENT_OFFSET = 9;

// The size of the L2CAP header. All information past this point is removed from
// a filtered packet.
static const uint32_t L2C_HEADER_SIZE = 9;

static int logfile_fd = INVALID_FD;
static std::mutex btsnoop_mutex;

static int32_t packets_per_file;
static int32_t packet_counter;

// Channel tracking variables for filtering.

// Keeps track of L2CAP channels that need to be filtered out of the snoop
// logs.
class FilterTracker {
 public:
  // NOTE: 1 is used as a static CID for L2CAP signaling
  std::unordered_set<uint16_t> l2c_local_cid = {1};
  std::unordered_set<uint16_t> l2c_remote_cid = {1};
  uint16_t rfc_local_cid = 0;
  uint16_t rfc_remote_cid = 0;
  std::unordered_set<uint16_t> rfc_channels = {0};

  // Adds L2C channel to whitelist.
  void addL2cCid(uint16_t local_cid, uint16_t remote_cid) {
    l2c_local_cid.insert(local_cid);
    l2c_remote_cid.insert(remote_cid);
  }

  // Sets L2CAP channel that RFCOMM uses.
  void setRfcCid(uint16_t local_cid, uint16_t remote_cid) {
    rfc_local_cid = local_cid;
    rfc_remote_cid = remote_cid;
  }

  // Remove L2C channel from whitelist.
  void removeL2cCid(uint16_t local_cid, uint16_t remote_cid) {
    if (rfc_local_cid == local_cid) {
      rfc_channels.clear();
      rfc_channels.insert(0);
      rfc_local_cid = 0;
      rfc_remote_cid = 0;
    }

    l2c_local_cid.erase(local_cid);
    l2c_remote_cid.erase(remote_cid);
  }

  void addRfcDlci(uint8_t channel) { rfc_channels.insert(channel); }

  bool isWhitelistedL2c(bool local, uint16_t cid) {
    const auto& set = local ? l2c_local_cid : l2c_remote_cid;
    return (set.find(cid) != set.end());
  }

  bool isRfcChannel(bool local, uint16_t cid) {
    const auto& channel = local ? rfc_local_cid : rfc_remote_cid;
    return cid == channel;
  }

  bool isWhitelistedDlci(uint8_t dlci) {
    return rfc_channels.find(dlci) != rfc_channels.end();
  }
};

std::mutex filter_list_mutex;
std::unordered_map<uint16_t, FilterTracker> filter_list;
std::unordered_map<uint16_t, uint16_t> local_cid_to_acl;

// Cached value for whether full snoop logs are enabled. So the property isn't
// checked for every packet.
static bool is_btsnoop_enabled;
static bool is_btsnoop_filtered;

// TODO(zachoverflow): merge btsnoop and btsnoop_net together
void btsnoop_net_open();
void btsnoop_net_close();
void btsnoop_net_write(const void* data, size_t length);

static void delete_btsnoop_files(bool filtered);
static std::string get_btsnoop_log_path(bool filtered);
static std::string get_btsnoop_last_log_path(std::string log_path);
static void open_next_snoop_file();
static void btsnoop_write_packet(packet_type_t type, uint8_t* packet,
                                 bool is_received, uint64_t timestamp_us);

// Module lifecycle functions

static future_t* start_up() {
  std::array<char, PROPERTY_VALUE_MAX> property = {};
  std::lock_guard<std::mutex> lock(btsnoop_mutex);

  // Default mode is FILTERED on userdebug/eng build, DISABLED on user build.
  // It can also be overwritten by modifying the global setting.
  int is_debuggable = osi_property_get_int32(IS_DEBUGGABLE_PROPERTY, 0);
  std::string default_mode = BTSNOOP_MODE_DISABLED;
  if (is_debuggable) {
    int len = osi_property_get(BTSNOOP_DEFAULT_MODE_PROPERTY, property.data(),
                               BTSNOOP_MODE_DISABLED);
    default_mode = std::string(property.data(), len);
  }

  // Get the actual mode
  int len = osi_property_get(BTSNOOP_LOG_MODE_PROPERTY, property.data(),
                             default_mode.c_str());
  std::string btsnoop_mode(property.data(), len);

  if (btsnoop_mode == BTSNOOP_MODE_FILTERED) {
    LOG(INFO) << __func__ << ": Filtered Snoop Logs enabled";
    is_btsnoop_enabled = true;
    is_btsnoop_filtered = true;
    delete_btsnoop_files(false);
  } else if (btsnoop_mode == BTSNOOP_MODE_FULL) {
    LOG(INFO) << __func__ << ": Snoop Logs fully enabled";
    is_btsnoop_enabled = true;
    is_btsnoop_filtered = false;
    delete_btsnoop_files(true);
  } else {
    LOG(INFO) << __func__ << ": Snoop Logs disabled";
    is_btsnoop_enabled = false;
    is_btsnoop_filtered = false;
    delete_btsnoop_files(true);
    delete_btsnoop_files(false);
  }

  if (is_btsnoop_enabled) {
    open_next_snoop_file();
    packets_per_file = osi_property_get_int32(BTSNOOP_MAX_PACKETS_PROPERTY,
                                              DEFAULT_BTSNOOP_SIZE);
    btsnoop_net_open();
  }

  return NULL;
}

static future_t* shut_down(void) {
  std::lock_guard<std::mutex> lock(btsnoop_mutex);

  if (is_btsnoop_enabled) {
    if (is_btsnoop_filtered) {
      delete_btsnoop_files(false);
    } else {
      delete_btsnoop_files(true);
    }
  } else {
    delete_btsnoop_files(true);
    delete_btsnoop_files(false);
  }

  if (logfile_fd != INVALID_FD) close(logfile_fd);
  logfile_fd = INVALID_FD;

  if (is_btsnoop_enabled) btsnoop_net_close();

  return NULL;
}

EXPORT_SYMBOL extern const module_t btsnoop_module = {
    .name = BTSNOOP_MODULE,
    .init = NULL,
    .start_up = start_up,
    .shut_down = shut_down,
    .clean_up = NULL,
    .dependencies = {STACK_CONFIG_MODULE, NULL}};

// Interface functions
static void capture(const BT_HDR* buffer, bool is_received) {
  uint8_t* p = const_cast<uint8_t*>(buffer->data + buffer->offset);

  std::lock_guard<std::mutex> lock(btsnoop_mutex);

  struct timespec ts_now = {};
  clock_gettime(CLOCK_REALTIME, &ts_now);
  uint64_t timestamp_us =
      ((uint64_t)ts_now.tv_sec * 1000000L) + ((uint64_t)ts_now.tv_nsec / 1000);

  btsnoop_mem_capture(buffer, timestamp_us);

  if (logfile_fd == INVALID_FD) return;

  switch (buffer->event & MSG_EVT_MASK) {
    case MSG_HC_TO_STACK_HCI_EVT:
      btsnoop_write_packet(kEventPacket, p, false, timestamp_us);
      break;
    case MSG_HC_TO_STACK_HCI_ACL:
    case MSG_STACK_TO_HC_HCI_ACL:
      btsnoop_write_packet(kAclPacket, p, is_received, timestamp_us);
      break;
    case MSG_HC_TO_STACK_HCI_SCO:
    case MSG_STACK_TO_HC_HCI_SCO:
      btsnoop_write_packet(kScoPacket, p, is_received, timestamp_us);
      break;
    case MSG_STACK_TO_HC_HCI_CMD:
      btsnoop_write_packet(kCommandPacket, p, true, timestamp_us);
      break;
  }
}

static void whitelist_l2c_channel(uint16_t conn_handle, uint16_t local_cid,
                                  uint16_t remote_cid) {
  LOG(INFO) << __func__
            << ": Whitelisting l2cap channel. conn_handle=" << conn_handle
            << " cid=" << loghex(local_cid) << ":" << loghex(remote_cid);
  std::lock_guard lock(filter_list_mutex);

  // This will create the entry if there is no associated filter with the
  // connection.
  filter_list[conn_handle].addL2cCid(local_cid, remote_cid);
}

static void whitelist_rfc_dlci(uint16_t local_cid, uint8_t dlci) {
  LOG(INFO) << __func__
            << ": Whitelisting rfcomm channel. L2CAP CID=" << loghex(local_cid)
            << " DLCI=" << loghex(dlci);
  std::lock_guard lock(filter_list_mutex);

  tL2C_CCB* p_ccb = l2cu_find_ccb_by_cid(nullptr, local_cid);
  filter_list[p_ccb->p_lcb->handle].addRfcDlci(dlci);
}

static void add_rfc_l2c_channel(uint16_t conn_handle, uint16_t local_cid,
                                uint16_t remote_cid) {
  LOG(INFO) << __func__
            << ": rfcomm data going over l2cap channel. conn_handle="
            << conn_handle << " cid=" << loghex(local_cid) << ":"
            << loghex(remote_cid);
  std::lock_guard lock(filter_list_mutex);

  filter_list[conn_handle].setRfcCid(local_cid, remote_cid);
  local_cid_to_acl.insert({local_cid, conn_handle});
}

static void clear_l2cap_whitelist(uint16_t conn_handle, uint16_t local_cid,
                                  uint16_t remote_cid) {
  LOG(INFO) << __func__
            << ": Clearing whitelist from l2cap channel. conn_handle="
            << conn_handle << " cid=" << local_cid << ":" << remote_cid;

  std::lock_guard lock(filter_list_mutex);
  filter_list[conn_handle].removeL2cCid(local_cid, remote_cid);
}

static const btsnoop_t interface = {capture, whitelist_l2c_channel,
                                    whitelist_rfc_dlci, add_rfc_l2c_channel,
                                    clear_l2cap_whitelist};

const btsnoop_t* btsnoop_get_interface() { return &interface; }

static void delete_btsnoop_files(bool filtered) {
  LOG(INFO) << __func__
            << ": Deleting snoop logs if they exist. filtered = " << filtered;
  auto log_path = get_btsnoop_log_path(filtered);
  remove(log_path.c_str());
  remove(get_btsnoop_last_log_path(log_path).c_str());
}

std::string get_btsnoop_log_path(bool filtered) {
  char btsnoop_path[PROPERTY_VALUE_MAX];
  osi_property_get(BTSNOOP_PATH_PROPERTY, btsnoop_path, DEFAULT_BTSNOOP_PATH);
  std::string result(btsnoop_path);
  if (filtered) result = result.append(".filtered");

  return result;
}

std::string get_btsnoop_last_log_path(std::string btsnoop_path) {
  return btsnoop_path.append(".last");
}

static void open_next_snoop_file() {
  packet_counter = 0;

  if (logfile_fd != INVALID_FD) {
    close(logfile_fd);
    logfile_fd = INVALID_FD;
  }

  auto log_path = get_btsnoop_log_path(is_btsnoop_filtered);
  auto last_log_path = get_btsnoop_last_log_path(log_path);

  if (rename(log_path.c_str(), last_log_path.c_str()) != 0 && errno != ENOENT)
    LOG(ERROR) << __func__ << ": unable to rename '" << log_path << "' to '"
               << last_log_path << "' : " << strerror(errno);

  mode_t prevmask = umask(0);
  logfile_fd = open(log_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  umask(prevmask);
  if (logfile_fd == INVALID_FD) {
    LOG(ERROR) << __func__ << ": unable to open '" << log_path
               << "' : " << strerror(errno);
    return;
  }

  write(logfile_fd, "btsnoop\0\0\0\0\1\0\0\x3\xea", 16);
}

typedef struct {
  uint32_t length_original;
  uint32_t length_captured;
  uint32_t flags;
  uint32_t dropped_packets;
  uint64_t timestamp;
  uint8_t type;
} __attribute__((__packed__)) btsnoop_header_t;

static uint64_t htonll(uint64_t ll) {
  const uint32_t l = 1;
  if (*(reinterpret_cast<const uint8_t*>(&l)) == 1)
    return static_cast<uint64_t>(htonl(ll & 0xffffffff)) << 32 |
           htonl(ll >> 32);

  return ll;
}

static bool should_filter_log(bool is_received, uint8_t* packet) {
  uint16_t acl_handle =
      HCID_GET_HANDLE((((uint16_t)packet[ACL_CHANNEL_OFFSET + 1]) << 8) +
                      packet[ACL_CHANNEL_OFFSET]);

  std::lock_guard lock(filter_list_mutex);
  auto& filters = filter_list[acl_handle];
  uint16_t l2c_channel =
      (packet[L2C_CHANNEL_OFFSET + 1] << 8) + packet[L2C_CHANNEL_OFFSET];
  if (filters.isRfcChannel(is_received, l2c_channel)) {
    uint8_t rfc_event = packet[RFC_EVENT_OFFSET] & 0b11101111;
    if (rfc_event == RFCOMM_SABME || rfc_event == RFCOMM_UA) {
      return false;
    }

    uint8_t rfc_dlci = packet[RFC_CHANNEL_OFFSET] >> 2;
    if (!filters.isWhitelistedDlci(rfc_dlci)) {
      return true;
    }
  } else if (!filters.isWhitelistedL2c(is_received, l2c_channel)) {
    return true;
  }

  return false;
}

static void btsnoop_write_packet(packet_type_t type, uint8_t* packet,
                                 bool is_received, uint64_t timestamp_us) {
  uint32_t length_he = 0;
  uint32_t flags = 0;

  switch (type) {
    case kCommandPacket:
      length_he = packet[2] + 4;
      flags = 2;
      break;
    case kAclPacket:
      length_he = (packet[3] << 8) + packet[2] + 5;
      flags = is_received;
      break;
    case kScoPacket:
      length_he = packet[2] + 4;
      flags = is_received;
      break;
    case kEventPacket:
      length_he = packet[1] + 3;
      flags = 3;
      break;
  }

  btsnoop_header_t header;
  header.length_original = htonl(length_he);

  bool blacklisted = false;
  if (is_btsnoop_filtered && type == kAclPacket) {
    blacklisted = should_filter_log(is_received, packet);
  }

  header.length_captured =
      blacklisted ? htonl(L2C_HEADER_SIZE) : header.length_original;
  if (blacklisted) length_he = L2C_HEADER_SIZE;
  header.flags = htonl(flags);
  header.dropped_packets = 0;
  header.timestamp = htonll(timestamp_us + BTSNOOP_EPOCH_DELTA);
  header.type = type;

  btsnoop_net_write(&header, sizeof(btsnoop_header_t));
  btsnoop_net_write(packet, length_he - 1);

  if (logfile_fd != INVALID_FD) {
    packet_counter++;
    if (packet_counter > packets_per_file) {
      open_next_snoop_file();
    }

    iovec iov[] = {{&header, sizeof(btsnoop_header_t)},
                   {reinterpret_cast<void*>(packet), length_he - 1}};
    TEMP_FAILURE_RETRY(writev(logfile_fd, iov, 2));
  }
}
