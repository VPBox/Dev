/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

#include <iostream>
#include <memory>
#include <stack>

#include <base/bind.h>
#include <base/cancelable_callback.h>

#include "avrcp_internal.h"
#include "hardware/avrcp/avrcp.h"
#include "packet/avrcp/avrcp_browse_packet.h"
#include "packet/avrcp/avrcp_packet.h"
#include "packet/avrcp/capabilities_packet.h"
#include "packet/avrcp/change_path.h"
#include "packet/avrcp/get_element_attributes_packet.h"
#include "packet/avrcp/get_folder_items.h"
#include "packet/avrcp/get_item_attributes.h"
#include "packet/avrcp/get_total_number_of_items.h"
#include "packet/avrcp/play_item.h"
#include "packet/avrcp/register_notification_packet.h"
#include "packet/avrcp/set_addressed_player.h"
#include "packet/avrcp/set_browsed_player.h"
#include "packet/avrcp/vendor_packet.h"
#include "profile/avrcp/media_id_map.h"
#include "raw_address.h"

namespace bluetooth {
namespace avrcp {

/**
 * A class representing a connection with a remote AVRCP device. It holds all
 * the state and message handling for the device that it represents.
 */
// TODO (apanicke): Once we move over to having the individual message
// responders for Browse and Classic AVRCP Messages move the device around via a
// weak pointer.
class Device {
 public:
  /**
   * Device is friends with Avrcp::ConnectionHandler so that ConnectionHandler
   * can deliver messages to individual devices.
   */
  friend class ConnectionHandler;

  Device(
      const RawAddress& bdaddr, bool avrcp13_compatibility,
      base::Callback<void(uint8_t label, bool browse,
                          std::unique_ptr<::bluetooth::PacketBuilder> message)>
          send_msg_cb,
      uint16_t ctrl_mtu, uint16_t browse_mtu);
  virtual ~Device() = default;

  /**
   * Gets a weak pointer to this device that is invalidated when the device is
   * disconnected.
   */
  base::WeakPtr<Device> Get();

  const RawAddress& GetAddress() const { return address_; };

  /**
   * Disconnects the AVRCP connection that this device represents.
   */
  bool Disconnect();

  /**
   * Returns true if the current device is silenced.
   */
  bool IsInSilenceMode() const;

  /**
   * Returns true if the current device is active.
   */
  bool IsActive() const;

  /**
   * Register the interfaces that the device uses to get information. If the
   * Volume Interface is null, then absolute volume is disabled.
   * TODO (apanicke): Add these to the constructor/factory so that each device
   * is created valid and can't be accidentally interacted with when no
   * interfaces are registered.
   */
  void RegisterInterfaces(MediaInterface* interface,
                          A2dpInterface* a2dp_interface,
                          VolumeInterface* volume_interface);

  /**
   * Set the maximum size of a AVRCP Browsing Packet. This is done after the
   * connection of the Browsing channel.
   */
  void SetBrowseMtu(uint16_t browse_mtu);

  /**
   * Notify the device that metadata, play_status, and/or queue have updated
   * via a boolean. Each boolean represents whether its respective content has
   * updated.
   */
  virtual void SendMediaUpdate(bool metadata, bool play_status, bool queue);

  /**
   * Notify the device that the available_player, addressed_player, or UIDs
   * have updated via a boolean. Each boolean represents whether its respective
   * content has updated.
   */
  virtual void SendFolderUpdate(bool available_player, bool addressed_player,
                                bool uids);

  // TODO (apanicke): Split the message handlers into two files. One
  // for handling Browse Messages and the other for handling all other
  // messages. This prevents the .cc file from getting bloated like it is
  // now. The Device class will then become a state holder for each message
  // and all the functions in these handler classes can be static since the
  // device will be passed in. The extensions of the Device class can contain
  // any interop handling for specific messages on specific devices.

  void MessageReceived(uint8_t label, std::shared_ptr<Packet> pkt);
  void BrowseMessageReceived(uint8_t label, std::shared_ptr<BrowsePacket> pkt);
  void VendorPacketHandler(uint8_t label, std::shared_ptr<VendorPacket> pkt);

  /********************
   * MESSAGE RESPONSES
   ********************/
  // CURRENT TRACK CHANGED
  virtual void HandleTrackUpdate();
  virtual void TrackChangedNotificationResponse(
      uint8_t label, bool interim, std::string curr_song_id,
      std::vector<SongInfo> song_list);

  // GET CAPABILITY
  virtual void HandleGetCapabilities(
      uint8_t label, const std::shared_ptr<GetCapabilitiesRequest>& pkt);

  // REGISTER NOTIFICATION
  virtual void HandleNotification(
      uint8_t label, const std::shared_ptr<RegisterNotificationRequest>& pkt);

  // PLAY STATUS CHANGED
  virtual void HandlePlayStatusUpdate();

  // NOW PLAYING LIST CHANGED
  virtual void HandleNowPlayingUpdate();
  virtual void HandleNowPlayingNotificationResponse(
      uint8_t label, bool interim, std::string curr_song_id,
      std::vector<SongInfo> song_list);

  // PLAY POSITION CHANGED
  virtual void HandlePlayPosUpdate();
  virtual void PlaybackPosNotificationResponse(uint8_t label, bool interim,
                                               PlayStatus status);

  // GET PLAY STATUS
  virtual void GetPlayStatusResponse(uint8_t label, PlayStatus status);
  virtual void PlaybackStatusNotificationResponse(uint8_t label, bool interim,
                                                  PlayStatus status);

  // GET ELEMENT ATTRIBUTE
  // TODO (apanicke): Add a Handler function for this so if a specific device
  // needs to implement an interop fix, you only need to overload the one
  // function.
  virtual void GetElementAttributesResponse(
      uint8_t label, std::shared_ptr<GetElementAttributesRequest> pkt,
      SongInfo info);

  // AVAILABLE PLAYER CHANGED
  virtual void HandleAvailablePlayerUpdate();

  // ADDRESSED PLAYER CHANGED
  virtual void HandleAddressedPlayerUpdate();
  virtual void RejectNotification();
  virtual void AddressedPlayerNotificationResponse(
      uint8_t label, bool interim, uint16_t curr_player,
      std::vector<MediaPlayerInfo> /* unused */);

  // GET FOLDER ITEMS
  virtual void HandleGetFolderItems(
      uint8_t label, std::shared_ptr<GetFolderItemsRequest> request);
  virtual void GetMediaPlayerListResponse(
      uint8_t label, std::shared_ptr<GetFolderItemsRequest> pkt,
      uint16_t curr_player, std::vector<MediaPlayerInfo> players);
  virtual void GetVFSListResponse(uint8_t label,
                                  std::shared_ptr<GetFolderItemsRequest> pkt,
                                  std::vector<ListItem> items);
  virtual void GetNowPlayingListResponse(
      uint8_t label, std::shared_ptr<GetFolderItemsRequest> pkt,
      std::string curr_song_id, std::vector<SongInfo> song_list);

  // GET TOTAL NUMBER OF ITEMS
  virtual void HandleGetTotalNumberOfItems(
      uint8_t label, std::shared_ptr<GetTotalNumberOfItemsRequest> pkt);
  virtual void GetTotalNumberOfItemsMediaPlayersResponse(
      uint8_t label, uint16_t curr_player, std::vector<MediaPlayerInfo> list);
  virtual void GetTotalNumberOfItemsVFSResponse(uint8_t label,
                                                std::vector<ListItem> items);
  virtual void GetTotalNumberOfItemsNowPlayingResponse(
      uint8_t label, std::string curr_song_id, std::vector<SongInfo> song_list);

  // GET ITEM ATTRIBUTES
  virtual void HandleGetItemAttributes(
      uint8_t label, std::shared_ptr<GetItemAttributesRequest> request);
  virtual void GetItemAttributesNowPlayingResponse(
      uint8_t label, std::shared_ptr<GetItemAttributesRequest> pkt,
      std::string curr_media_id, std::vector<SongInfo> song_list);
  virtual void GetItemAttributesVFSResponse(
      uint8_t label, std::shared_ptr<GetItemAttributesRequest> pkt,
      std::vector<ListItem> item_list);

  // SET BROWSED PLAYER
  virtual void HandleSetBrowsedPlayer(
      uint8_t label, std::shared_ptr<SetBrowsedPlayerRequest> request);
  virtual void SetBrowsedPlayerResponse(
      uint8_t label, std::shared_ptr<SetBrowsedPlayerRequest> pkt, bool success,
      std::string root_id, uint32_t num_items);

  // CHANGE PATH
  virtual void HandleChangePath(uint8_t label,
                                std::shared_ptr<ChangePathRequest> request);
  virtual void ChangePathResponse(uint8_t label,
                                  std::shared_ptr<ChangePathRequest> request,
                                  std::vector<ListItem> list);

  // PLAY ITEM
  virtual void HandlePlayItem(uint8_t label,
                              std::shared_ptr<PlayItemRequest> request);

  // SET ADDRESSED PLAYER
  virtual void HandleSetAddressedPlayer(
      uint8_t label, std::shared_ptr<SetAddressedPlayerRequest> request,
      uint16_t curr_player, std::vector<MediaPlayerInfo> players);

  /********************
   * MESSAGE REQUESTS
   ********************/
  // VOLUME CHANGED NOTIFICATION
  virtual void RegisterVolumeChanged();
  virtual void HandleVolumeChanged(
      uint8_t label, const std::shared_ptr<RegisterNotificationResponse>& pkt);

  // SET VOLUME
  virtual void SetVolume(int8_t volume);

  /**
   * This function is called by Avrcp::ConnectionHandler to signify that
   * the remote device was disconnected.
   *
   * TODO (apanicke): Prevent allowing responses to messages while the device is
   * disconnected by using a weak pointer handle to the device when we separate
   * out the message handling. Also separate the logic in the future when
   * disconnecting only browsing (Though this shouldn't matter as if we are
   * disconnecting browsing then we should be fully disconnecting the device).
   */
  void DeviceDisconnected();

  friend std::ostream& operator<<(std::ostream& out, const Device& c);

 private:
  // This should always contain one item which represents the root id on the
  // current player.
  std::string CurrentFolder() const {
    if (current_path_.empty()) return "";
    return current_path_.top();
  }

  void send_message(uint8_t label, bool browse,
                    std::unique_ptr<::bluetooth::PacketBuilder> message) {
    active_labels_.erase(label);
    send_message_cb_.Run(label, browse, std::move(message));
  }
  base::WeakPtrFactory<Device> weak_ptr_factory_;

  // TODO (apanicke): Initialize all the variables in the constructor.
  RawAddress address_;

  // Enables AVRCP 1.3 Compatibility mode. This disables any AVRCP 1.4+ features
  // such as browsing and playlists but has the highest chance of working.
  bool avrcp13_compatibility_ = false;
  base::Callback<void(uint8_t label, bool browse,
                      std::unique_ptr<::bluetooth::PacketBuilder> message)>
      send_message_cb_;
  uint16_t ctrl_mtu_;
  uint16_t browse_mtu_;

  int curr_browsed_player_id_ = -1;

  std::stack<std::string> current_path_;

  // Notification Trackers
  using Notification = std::pair<bool, uint8_t>;
  Notification track_changed_ = Notification(false, 0);
  Notification play_status_changed_ = Notification(false, 0);
  Notification play_pos_changed_ = Notification(false, 0);
  Notification now_playing_changed_ = Notification(false, 0);
  Notification addr_player_changed_ = Notification(false, 0);
  Notification avail_players_changed_ = Notification(false, 0);
  Notification uids_changed_ = Notification(false, 0);

  MediaIdMap vfs_ids_;
  MediaIdMap now_playing_ids_;

  uint32_t play_pos_interval_ = 0;

  SongInfo last_song_info_;
  PlayStatus last_play_status_;

  base::CancelableClosure play_pos_update_cb_;

  MediaInterface* media_interface_ = nullptr;
  A2dpInterface* a2dp_interface_ = nullptr;
  VolumeInterface* volume_interface_ = nullptr;

  // Labels used for messages currently in flight.
  std::set<uint8_t> active_labels_;

  int8_t volume_ = -1;
  DISALLOW_COPY_AND_ASSIGN(Device);
};

}  // namespace avrcp
}  // namespace bluetooth
