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

#include <set>
#include <string>

#include <base/callback_forward.h>

#include "avrcp_common.h"
#include "raw_address.h"

namespace bluetooth {
namespace avrcp {

struct SongInfo {
  std::string media_id;  // This gets converted to a UID in the native service
  std::set<AttributeEntry> attributes;
};

enum PlayState : uint8_t {
  STOPPED = 0x00,
  PLAYING,
  PAUSED,
  FWD_SEEK,
  REV_SEEK,
  ERROR = 0xFF,
};

struct PlayStatus {
  uint32_t position;
  uint32_t duration;
  PlayState state;
};

struct MediaPlayerInfo {
  uint16_t id;
  std::string name;
  bool browsing_supported;
};

struct FolderInfo {
  std::string media_id;
  bool is_playable;
  std::string name;
};

// TODO (apanicke): Convert this to a union
struct ListItem {
  enum : uint8_t {
    FOLDER,
    SONG,
  } type;

  FolderInfo folder;
  SongInfo song;
};

class MediaCallbacks {
 public:
  virtual void SendMediaUpdate(bool track_changed, bool play_state,
                               bool queue) = 0;
  virtual void SendFolderUpdate(bool available_players, bool addressed_players,
                                bool uids_changed) = 0;
  virtual void SendActiveDeviceChanged(const RawAddress& address) = 0;
  virtual ~MediaCallbacks() = default;
};

// The classes below are used by the JNI and are loaded dynamically with the
// Bluetooth library. All classes must be pure virtual otherwise a compiler
// error occurs when trying to link the function implementation.

// MediaInterface defines the class that the AVRCP Service uses in order
// communicate with the media layer. The media layer will define its own
// implementation of this object and register it with the service using
// Avrcp::ServiceInterface::Init(). At this point the AVRCP Service will
// call RegisterUpdateCallbacks() to provide an handle to use to send
// notifications about changes in the Media Interface.
//
// NOTES: The current implementation has the native service handle all the
// thread switching. It will call the interface functions on the btif/jni
// thread and the callback will post its results to the bta thread.
// In the future the interface the JNI registered with the
// service should post all its tasks to the JNI thread itself so that the native
// service isn't aware of the thread the interface functions need to be called
// on. It can then supply callbacks that post results to the correct thread
// allowing the threading model to be totally encapsulated and allow correct
// behavior in case the threading model changes on either side.
class MediaInterface {
 public:
  virtual void SendKeyEvent(uint8_t key, KeyState state) = 0;

  using SongInfoCallback = base::Callback<void(SongInfo)>;
  virtual void GetSongInfo(SongInfoCallback info_cb) = 0;

  using PlayStatusCallback = base::Callback<void(PlayStatus)>;
  virtual void GetPlayStatus(PlayStatusCallback status_cb) = 0;

  // Contains the current queue and the media ID of the currently playing item
  // in the queue
  using NowPlayingCallback =
      base::Callback<void(std::string, std::vector<SongInfo>)>;
  virtual void GetNowPlayingList(NowPlayingCallback now_playing_cb) = 0;

  // TODO (apanicke): Use a map with the ID as the key instead of vector
  // in follow up cleanup patches. This allows simplification of the
  // MediaPlayerInfo object
  using MediaListCallback =
      base::Callback<void(uint16_t curr_player, std::vector<MediaPlayerInfo>)>;
  virtual void GetMediaPlayerList(MediaListCallback list_cb) = 0;

  using FolderItemsCallback = base::Callback<void(std::vector<ListItem>)>;
  virtual void GetFolderItems(uint16_t player_id, std::string media_id,
                              FolderItemsCallback folder_cb) = 0;

  using SetBrowsedPlayerCallback = base::Callback<void(
      bool success, std::string root_id, uint32_t num_items)>;
  virtual void SetBrowsedPlayer(uint16_t player_id,
                                SetBrowsedPlayerCallback browse_cb) = 0;

  virtual void PlayItem(uint16_t player_id, bool now_playing,
                        std::string media_id) = 0;

  virtual void SetActiveDevice(const RawAddress& address) = 0;

  virtual void RegisterUpdateCallback(MediaCallbacks* callback) = 0;

  virtual void UnregisterUpdateCallback(MediaCallbacks* callback) = 0;

  MediaInterface() = default;
  virtual ~MediaInterface() = default;
};

class VolumeInterface {
 public:
  // TODO (apanicke): Investigate the best value type for volume. Right now it
  // is a value from 0-127 because thats what AVRCP uses.
  using VolumeChangedCb = base::Callback<void(int8_t volume)>;

  // Indicate that a device has been connected that does not support absolute
  // volume.
  virtual void DeviceConnected(const RawAddress& bdaddr) = 0;

  // Indicate that a device has been connected that does support absolute
  // volume. The callback will be immediately called with the current volume
  // which will be sent to the device.
  virtual void DeviceConnected(const RawAddress& bdaddr,
                               VolumeChangedCb cb) = 0;

  // Indicate that a device has been disconnected from AVRCP. Will unregister
  // any callbacks if absolute volume is supported.
  virtual void DeviceDisconnected(const RawAddress& bdaddr) = 0;

  virtual void SetVolume(int8_t volume) = 0;

  virtual ~VolumeInterface() = default;
};

class ServiceInterface {
 public:
  // mediaInterface can not be null. If volumeInterface is null then Absolute
  // Volume is disabled.
  virtual void Init(MediaInterface* mediaInterface,
                    VolumeInterface* volumeInterface) = 0;
  virtual bool ConnectDevice(const RawAddress& bdaddr) = 0;
  virtual bool DisconnectDevice(const RawAddress& bdaddr) = 0;
  virtual bool Cleanup() = 0;

 protected:
  virtual ~ServiceInterface() = default;
};

}  // namespace avrcp
}  // namespace bluetooth