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

#include <base/sys_byteorder.h>

namespace bluetooth {
namespace avrcp {

constexpr uint32_t BLUETOOTH_COMPANY_ID = 0x001958;

constexpr uint8_t MAX_TRANSACTION_LABEL = 0xF;

enum class CType : uint8_t {
  CONTROL = 0x0,
  STATUS = 0x1,
  NOTIFY = 0x3,
  NOT_IMPLEMENTED = 0x8,
  ACCEPTED = 0x9,
  REJECTED = 0xa,
  STABLE = 0xc,
  CHANGED = 0xd,
  INTERIM = 0xf,
};

enum class Opcode : uint8_t {
  VENDOR = 0x00,
  UNIT_INFO = 0x30,
  SUBUNIT_INFO = 0x31,
  PASS_THROUGH = 0x7c,
};

// Found in AVRCP_v1.6.1 Section 4.5 Table 4.5
// Searching can be done in the spec by Camel Casing the constant name
enum class CommandPdu : uint8_t {
  GET_CAPABILITIES = 0x10,
  LIST_APPLICATION_SETTING_ATTRIBUTES = 0x11,
  GET_ELEMENT_ATTRIBUTES = 0x20,
  GET_PLAY_STATUS = 0x30,
  REGISTER_NOTIFICATION = 0x31,
  SET_ABSOLUTE_VOLUME = 0x50,
  SET_ADDRESSED_PLAYER = 0x60,
  PLAY_ITEM = 0x74,
};

enum class PacketType : uint8_t {
  SINGLE = 0x00,
};

enum class Capability : uint8_t {
  COMPANY_ID = 0x02,
  EVENTS_SUPPORTED = 0x03,
};

// Found in AVRCP_v1.6.1 Section 28 Appendix H
enum class Event : uint8_t {
  PLAYBACK_STATUS_CHANGED = 0x01,
  TRACK_CHANGED = 0x02,
  PLAYBACK_POS_CHANGED = 0x05,
  PLAYER_APPLICATION_SETTING_CHANGED = 0x08,
  NOW_PLAYING_CONTENT_CHANGED = 0x09,
  AVAILABLE_PLAYERS_CHANGED = 0x0a,
  ADDRESSED_PLAYER_CHANGED = 0x0b,
  UIDS_CHANGED = 0x0c,
  VOLUME_CHANGED = 0x0d,
};

enum class Attribute : uint32_t {
  TITLE = 0x01,
  ARTIST_NAME = 0x02,
  ALBUM_NAME = 0x03,
  TRACK_NUMBER = 0x04,
  TOTAL_NUMBER_OF_TRACKS = 0x05,
  GENRE = 0x06,
  PLAYING_TIME = 0x07,
  DEFAULT_COVER_ART = 0x08,
};

enum class Status : uint8_t {
  INVALID_COMMAND = 0x00,
  INVALID_PARAMETER = 0x01,
  PARAMETER_CONTENT_ERROR = 0x02,
  INTERNAL_ERROR = 0x03,
  NO_ERROR = 0x04,
  UIDS_CHANGED = 0x05,
  RESERVED = 0x06,
  INVALID_DIRECTION = 0x07,
  NOT_A_DIRECTORY = 0x08,
  DOES_NOT_EXIST = 0x09,
  INVALID_SCOPE = 0x0a,
  RANGE_OUT_OF_BOUNDS = 0xb,
  FOLDER_ITEM_NOT_PLAYABLE = 0x0c,
  MEDIA_IN_USE = 0x0d,
  NOW_PLAYING_LIST_FULL = 0x0e,
  SEARCH_NOT_SUPPORTED = 0x0f,
  SEARCH_IN_PROGRESS = 0x10,
  INVALID_PLAYER_ID = 0x11,
  PLAYER_NOT_BROWSABLE = 0x12,
  PLAYER_NOT_ADDRESSED = 0x13,
  NO_VALID_SEARCH_RESULTS = 0x14,
  NO_AVAILABLE_PLAYERS = 0x15,
  ADDRESSED_PLAYER_CHANGED = 0x16,
};

enum class BrowsePdu : uint8_t {
  SET_BROWSED_PLAYER = 0x70,
  GET_FOLDER_ITEMS = 0x71,
  CHANGE_PATH = 0x72,
  GET_ITEM_ATTRIBUTES = 0x73,
  GET_TOTAL_NUMBER_OF_ITEMS = 0x75,
  GENERAL_REJECT = 0xa0,
};

enum class Scope : uint8_t {
  MEDIA_PLAYER_LIST = 0x00,
  VFS = 0x01,
  SEARCH = 0x02,
  NOW_PLAYING = 0x03,
};

enum class Direction : uint8_t {
  UP = 0x00,
  DOWN = 0x01,
};

enum class KeyState : uint8_t {
  PUSHED = 0x00,
  RELEASED = 0x01,
};

class AttributeEntry {
 public:
  AttributeEntry(const Attribute& attribute, const std::string& value)
      : attribute_(attribute), value_(value) {}

  AttributeEntry(const Attribute& attribute) : attribute_(attribute) {}

  AttributeEntry(const AttributeEntry&) = default;

  Attribute attribute() const { return attribute_; }

  std::string value() const { return value_; }

  static constexpr size_t kHeaderSize() {
    size_t ret = 0;
    ret += 4;  // Size of attribute field
    ret += 2;  // Size of length field
    ret += 2;  // Size of character encoding field
    return ret;
  }

  size_t size() const { return kHeaderSize() + value_.size(); }

  void resize(size_t new_size) {
    new_size = new_size < kHeaderSize() ? 0 : new_size - kHeaderSize();
    if (value_.size() > new_size) {
      value_.resize(new_size);
    }
  }

  bool empty() { return value_.empty(); }

  bool operator<(const AttributeEntry& rhs) const {
    return attribute_ < rhs.attribute_;
  }

 private:
  Attribute attribute_;
  std::string value_;
};

constexpr size_t MAX_FIELD_LEN = 100;

struct MediaPlayerItem {
  uint16_t id_;
  std::string name_;
  bool browsable_;

  MediaPlayerItem(uint16_t id, const std::string& name, bool browsable)
      : id_(id), name_(name), browsable_(browsable) {
    if (name_.size() > MAX_FIELD_LEN) {
      name_.resize(MAX_FIELD_LEN);
    }
  }

  MediaPlayerItem(const MediaPlayerItem&) = default;

  static constexpr size_t kHeaderSize() {
    size_t ret = 0;
    ret += 1;   // Media Player Type
    ret += 2;   // Item Length
    ret += 2;   // Player Id
    ret += 1;   // Player Type
    ret += 4;   // Player Subtype
    ret += 1;   // Play Status
    ret += 16;  // Features
    ret += 2;   // UTF-8 character set
    ret += 2;   // Name Length
    return ret;
  }

  size_t size() const { return kHeaderSize() + name_.size(); }
};

struct FolderItem {
  uint64_t uid_;
  uint8_t folder_type_;
  bool is_playable_;
  std::string name_;

  FolderItem(uint64_t uid, uint8_t folder_type, bool is_playable,
             const std::string& name)
      : uid_(uid),
        folder_type_(folder_type),
        is_playable_(is_playable),
        name_(name) {
    if (name_.size() > MAX_FIELD_LEN) {
      name_.resize(MAX_FIELD_LEN);
    }
  }

  FolderItem(const FolderItem&) = default;

  static constexpr size_t kHeaderSize() {
    size_t ret = 0;
    ret += 1;  // Folder Item Type
    ret += 2;  // Item Length
    ret += 8;  // Folder UID
    ret += 1;  // Folder Type
    ret += 1;  // Is Playable byte
    ret += 2;  // UTF-8 Character Set
    ret += 2;  // Name Length
    return ret;
  }

  size_t size() const { return kHeaderSize() + name_.size(); }
};

// NOTE: We never use media type field because we only support audio types
struct MediaElementItem {
  uint64_t uid_ = 0;
  std::string name_;
  std::set<AttributeEntry> attributes_;

  // Truncate the name and attribute fields so that we don't have a single item
  // that can exceed the Browsing MTU
  MediaElementItem(uint64_t uid, const std::string& name,
                   std::set<AttributeEntry> attributes)
      : uid_(uid), name_(name) {
    if (name_.size() > MAX_FIELD_LEN) {
      name_.resize(MAX_FIELD_LEN);
    }

    for (AttributeEntry val : attributes) {
      val.resize(MAX_FIELD_LEN);
      attributes_.insert(val);
    }
  }

  MediaElementItem(const MediaElementItem&) = default;

  size_t size() const {
    size_t ret = 0;
    ret += 1;  // Media Element Item Type
    ret += 2;  // Item Length
    ret += 8;  // Item UID
    ret += 1;  // Media Type
    ret += 2;  // UTF-8 Character Set
    ret += 2;  // Name Length
    ret += name_.size();
    ret += 1;  // Number of Attributes
    for (const auto& entry : attributes_) {
      ret += entry.size();
    }

    return ret;
  }
};

struct MediaListItem {
  enum : uint8_t { PLAYER = 0x01, FOLDER = 0x02, SONG = 0x03 } type_;

  union {
    MediaPlayerItem player_;
    FolderItem folder_;
    MediaElementItem song_;
  };

  MediaListItem(MediaPlayerItem item) : type_(PLAYER), player_(item) {}

  MediaListItem(FolderItem item) : type_(FOLDER), folder_(item) {}

  MediaListItem(MediaElementItem item) : type_(SONG), song_(item) {}

  MediaListItem(const MediaListItem& item) {
    type_ = item.type_;
    switch (item.type_) {
      case PLAYER:
        new (&player_) MediaPlayerItem(item.player_);
        return;
      case FOLDER:
        new (&folder_) FolderItem(item.folder_);
        return;
      case SONG:
        new (&song_) MediaElementItem(item.song_);
        return;
    }
  }

  ~MediaListItem() {
    switch (type_) {
      case PLAYER:
        player_.~MediaPlayerItem();
        return;
      case FOLDER:
        folder_.~FolderItem();
        return;
      case SONG:
        song_.~MediaElementItem();
        return;
    }
  }

  size_t size() const {
    switch (type_) {
      case PLAYER:
        return player_.size();
      case FOLDER:
        return folder_.size();
      case SONG:
        return song_.size();
    }
  }
};

constexpr size_t AVCT_HDR_LEN = 3;

}  // namespace avrcp
}  // namespace bluetooth