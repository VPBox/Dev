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

#ifndef WIFICOND_NET_NETLINK_MANAGER_H_
#define WIFICOND_NET_NETLINK_MANAGER_H_

#include <array>
#include <functional>
#include <map>
#include <memory>

#include <linux/if_ether.h>

#include <android-base/macros.h>
#include <android-base/unique_fd.h>

#include "event_loop.h"

namespace android {
namespace wificond {

class MlmeEventHandler;
class NL80211Packet;

// Encapsulates all the different things we know about a specific message
// type like its name, and its id.
struct MessageType {
   // This constructor is needed by map[key] operation.
   MessageType() {};
   explicit MessageType(uint16_t id) {
     family_id = id;
   };
   uint16_t family_id;
   // Multicast groups supported by the family.  The string and mapping to
   // a group id are extracted from the CTRL_CMD_NEWFAMILY message.
   std::map<std::string, uint32_t> groups;
};

// This describes a type of function handling scan results ready notification.
// |interface_index| is the index of interface which the scan results
// are from.
// |aborted| is a boolean indicating if this scan was aborted or not.
// According to nl80211.h document, part of the scan result might still be
// available even when the scan was aborted.
// |ssids| is a vector of scan ssids associated with the corresponding
// scan request.
// |frequencies| is a vector of scan frequencies associated with the
// corresponding scan request.
typedef std::function<void(
    uint32_t interface_index,
    bool aborted,
    std::vector<std::vector<uint8_t>>& ssids,
    std::vector<uint32_t>& frequencies)> OnScanResultsReadyHandler;

// This describes a type of function handling scheduled scan results ready
// notification. This can also be used for notificating the stopping of a
// scheduled scan.
// |interface_index| is the index of interface which the scan results
// are from.
// |scan_stopped| is a boolean indicating if this scheduled scan was stopped
// or not.
typedef std::function<void(
    uint32_t interface_index,
    bool scan_stopped)> OnSchedScanResultsReadyHandler;

// This describes a type of function handling regulatory domain change
// notification.
// If the regulatory domain set is one that pertains to a specific country,
// |country_code| will be set accordingly.
// If the regulatory domain set does not pertain to a specific country,
// |country_code| will be an empty string. This could be a world regulatory
// domain or a intersection regulatory domain.
// See details in defination of |nl80211_reg_type| from nl80211.h.
typedef std::function<void(
    std::string& country_code)> OnRegDomainChangedHandler;

// Enum used for identifying channel bandwidth.
// This is used by function |OnChannelSwitchEventHandler|.
enum ChannelBandwidth {
    BW_INVALID,
    BW_20_NOHT,
    BW_20,
    BW_40,
    BW_80,
    BW_80P80,
    BW_160,
};

// This describes a type of function handling channel switch notification.
// |frequency| represents the frequence of the channel in MHz.
typedef std::function<void(
    uint32_t frequency, ChannelBandwidth bandwidth)> OnChannelSwitchEventHandler;

// Enum used for identifying the type of a station event.
// This is used by function |OnStationEventHandler|.
enum StationEvent {
    NEW_STATION,
    DEL_STATION
};

// This describes a type of function handling station events.
// |event| specifies the type of this event.
// |mac_address| is the station mac address associated with this event.
typedef std::function<void(
    StationEvent event,
    const std::array<uint8_t, ETH_ALEN>& mac_address)> OnStationEventHandler;

// This describes a type of function handling frame tx status events.
// |cookie| specifies the cookie of the transmitted frame that this status
// event corresponds to.
// |was_acked| reports whether the transmitted frame was ACKed.
typedef std::function<void(
    uint64_t cookie, bool was_acked)> OnFrameTxStatusEventHandler;

class NetlinkManager {
 public:
  explicit NetlinkManager(EventLoop* event_loop);
  virtual ~NetlinkManager();
  // Initialize netlink manager.
  // This includes setting up socket and requesting nl80211 family id from kernel.
  // Returns true on success.
  virtual bool Start();
  // Returns true if this netlink manager object is started.
  virtual bool IsStarted() const;
  // Returns a sequence number available for use.
  virtual uint32_t GetSequenceNumber();
  // Get NL80211 netlink family id,
  virtual uint16_t GetFamilyId();

  // Send |packet| to kernel.
  // This works in an asynchronous way.
  // |handler| will be run when we receive a valid reply from kernel.
  // Do not use this asynchronous interface to send a dump request.
  // Returns true on success.
  virtual bool RegisterHandlerAndSendMessage(const NL80211Packet& packet,
      std::function<void(std::unique_ptr<const NL80211Packet>)> handler);
  // Synchronous version of |RegisterHandlerAndSendMessage|.
  // Returns true on successfully receiving an valid reply.
  // Reply packets will be stored in |*response|.
  virtual bool SendMessageAndGetResponses(
      const NL80211Packet& packet,
      std::vector<std::unique_ptr<const NL80211Packet>>* response);
  // Wrapper of |SendMessageAndGetResponses| for messages with a single
  // response.
  // Returns true on successfully receiving an valid reply.
  // This will returns false if a NLMSG_ERROR is received.
  // Reply packet will be stored in |*response|.
  virtual bool SendMessageAndGetSingleResponse(
      const NL80211Packet& packet,
      std::unique_ptr<const NL80211Packet>* response);

  // Wrapper of |SendMessageAndGetResponses| for messages with a single
  // response.
  // Returns true on successfully receiving an valid reply.
  // This will returns true if a NLMSG_ERROR is received.
  // This is useful when the caller needs the error code from kernel.
  // Reply packet will be stored in |*response|.
  virtual bool SendMessageAndGetSingleResponseOrError(
      const NL80211Packet& packet,
      std::unique_ptr<const NL80211Packet>* response);

  // Wrapper of |SendMessageAndGetResponses| for messages that trigger
  // only a NLMSG_ERROR response
  // Returns true if the message is successfully sent and a NLMSG_ERROR response
  // comes back, regardless of the error code.
  // Error code will be stored in |*error_code|
  virtual bool SendMessageAndGetAckOrError(const NL80211Packet& packet,
                                           int* error_code);
  // Wrapper of |SendMessageAndGetResponses| that returns true iff the response
  // is an ACK.
  virtual bool SendMessageAndGetAck(const NL80211Packet& packet);

  // Sign up to receive and log multicast events of a specific type.
  // |group| is one of the string NL80211_MULTICAST_GROUP_* in nl80211.h.
  virtual bool SubscribeToEvents(const std::string& group);

  // Sign up to be notified when new scan results are available.
  // |handler| will be called when the kernel signals to wificond that a scan
  // has been completed on the given |interface_index|.  See the declaration of
  // OnScanResultsReadyHandler for documentation on the semantics of this
  // callback.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeScanResultNotification(
      uint32_t interface_index,
      OnScanResultsReadyHandler handler);

  // Cancel the sign-up of receiving new scan result notification from
  // interface with index |interface_index|.
  virtual void UnsubscribeScanResultNotification(uint32_t interface_index);

  // Sign up to be notified when there is MLME event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  // NetlinkManager is not going to take ownership of this pointer, and that it
  // is the caller's responsibility to make sure that the object exists for the
  // duration of the subscription.
  virtual void SubscribeMlmeEvent(uint32_t interface_index,
                                  MlmeEventHandler* handler);

  // Cancel the sign-up of receiving MLME event notification
  // from interface with index |interface_index|.
  virtual void UnsubscribeMlmeEvent(uint32_t interface_index);

  // Sign up to be notified when new scan results are available.
  // |handler| will be called when the kernel signals to wificond that a
  // scheduled scan has been completed on the given |interface_index|.
  // See the declaration of OnSchedScanResultsReadyHandler for documentation
  // on the semantics of this callback.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeSchedScanResultNotification(
      uint32_t interface_index,
      OnSchedScanResultsReadyHandler handler);

  // Cancel the sign-up of receiving new scheduled scan result notification from
  // interface with index |interface_index|.
  virtual void UnsubscribeSchedScanResultNotification(uint32_t interface_index);

  // Sign up to be notified when there is an regulatory domain change.
  // Only one handler can be registered per wiphy index.
  // New handler will replace the registered handler if they are for the
  // same wiphy index.
  virtual void SubscribeRegDomainChange(uint32_t wiphy_index,
                                        OnRegDomainChangedHandler handler);

  // Cancel the sign-up of receiving regulatory domain change notification
  // from wiphy with index |wiphy_index|.
  virtual void UnsubscribeRegDomainChange(uint32_t wiphy_index);

  // Sign up to be notified when there is a station event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeStationEvent(uint32_t interface_index,
                                     OnStationEventHandler handler);

  // Cancel the sign-up of receiving station events.
  virtual void UnsubscribeStationEvent(uint32_t interface_index);

  // Sign up to be notified when there is a channel switch event.
  // Only one handler can be registered per interface index.
  // New handler will replace the registered handler if they are for the
  // same interface index.
  virtual void SubscribeChannelSwitchEvent(
      uint32_t interface_index,
      OnChannelSwitchEventHandler handler);

  // Cancel the sign-up of receiving channel events.
  virtual void UnsubscribeChannelSwitchEvent(uint32_t interface_index);

  // Sign up to be notified of frame tx status events.
  virtual void SubscribeFrameTxStatusEvent(
      uint32_t interface_index, OnFrameTxStatusEventHandler handler);

  // Cancel the sign-up of receiving frame tx status events.
  virtual void UnsubscribeFrameTxStatusEvent(uint32_t interface_index);

 private:
  bool SetupSocket(android::base::unique_fd* netlink_fd);
  bool WatchSocket(android::base::unique_fd* netlink_fd);
  void ReceivePacketAndRunHandler(int fd);
  bool DiscoverFamilyId();
  bool SendMessageInternal(const NL80211Packet& packet, int fd);
  void BroadcastHandler(std::unique_ptr<const NL80211Packet> packet);
  void OnRegChangeEvent(std::unique_ptr<const NL80211Packet> packet);
  void OnMlmeEvent(std::unique_ptr<const NL80211Packet> packet);
  void OnScanResultsReady(std::unique_ptr<const NL80211Packet> packet);
  void OnSchedScanResultsReady(std::unique_ptr<const NL80211Packet> packet);
  void OnChannelSwitchEvent(std::unique_ptr<const NL80211Packet> packet);
  void OnFrameTxStatusEvent(std::unique_ptr<const NL80211Packet> packet);

  // This handler revceives mapping from NL80211 family name to family id,
  // as well as mapping from group name to group id.
  // These mappings are allocated by kernel.
  void OnNewFamily(std::unique_ptr<const NL80211Packet> packet);

  bool started_;
  // We use different sockets for synchronous and asynchronous interfaces.
  // Kernel will reply error message when we start a new request in the
  // middle of a dump request.
  // Using different sockets help us avoid the complexity of message
  // rescheduling.
  android::base::unique_fd sync_netlink_fd_;
  android::base::unique_fd async_netlink_fd_;
  EventLoop* event_loop_;

  // This is a collection of message handlers, for each sequence number.
  std::map<uint32_t,
      std::function<void(std::unique_ptr<const NL80211Packet>)>> message_handlers_;

  // A mapping from interface index to the handler registered to receive
  // scan results notifications.
  std::map<uint32_t, OnScanResultsReadyHandler> on_scan_result_ready_handler_;
  // A mapping from interface index to the handler registered to receive
  // scheduled scan results notifications.
  std::map<uint32_t, OnSchedScanResultsReadyHandler>
      on_sched_scan_result_ready_handler_;

  std::map<uint32_t, MlmeEventHandler*> on_mlme_event_handler_;

  // A mapping from wiphy index to the handler registered to receive
  // regulatory domain change notifications.
  std::map<uint32_t, OnRegDomainChangedHandler> on_reg_domain_changed_handler_;
  std::map<uint32_t, OnStationEventHandler> on_station_event_handler_;
  std::map<uint32_t, OnChannelSwitchEventHandler> on_channel_switch_event_handler_;

  // mapping from interface_index to frame tx status event handler
  std::map<uint32_t, OnFrameTxStatusEventHandler>
      on_frame_tx_status_event_handler_;

  // Mapping from family name to family id, and group name to group id.
  std::map<std::string, MessageType> message_types_;

  uint32_t sequence_number_;

  DISALLOW_COPY_AND_ASSIGN(NetlinkManager);
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_NETLINK_MANAGER_H_
