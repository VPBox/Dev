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

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "avrcp_common.h"
#include "bt_trace.h"

namespace bluetooth {
namespace avrcp {

#define CASE_RETURN_TEXT(code) \
  case code:                   \
    return #code

inline std::string CTypeText(const CType& type) {
  switch (type) {
    CASE_RETURN_TEXT(CType::CONTROL);
    CASE_RETURN_TEXT(CType::STATUS);
    CASE_RETURN_TEXT(CType::NOTIFY);
    CASE_RETURN_TEXT(CType::ACCEPTED);
    CASE_RETURN_TEXT(CType::REJECTED);
    CASE_RETURN_TEXT(CType::STABLE);
    CASE_RETURN_TEXT(CType::CHANGED);
    CASE_RETURN_TEXT(CType::INTERIM);
    default:
      return "Unknown CType: " + loghex((uint8_t)type);
  }
}

inline std::ostream& operator<<(std::ostream& os, const CType& type) {
  return os << CTypeText(type);
}

inline std::string OpcodeText(const Opcode& opcode) {
  switch (opcode) {
    CASE_RETURN_TEXT(Opcode::VENDOR);
    CASE_RETURN_TEXT(Opcode::UNIT_INFO);
    CASE_RETURN_TEXT(Opcode::SUBUNIT_INFO);
    CASE_RETURN_TEXT(Opcode::PASS_THROUGH);
    default:
      return "Unknown Opcode: " + loghex((uint8_t)opcode);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Opcode& opcode) {
  return os << OpcodeText(opcode);
}

inline std::string CommandPduText(const CommandPdu& pdu) {
  switch (pdu) {
    CASE_RETURN_TEXT(CommandPdu::GET_CAPABILITIES);
    CASE_RETURN_TEXT(CommandPdu::LIST_APPLICATION_SETTING_ATTRIBUTES);
    CASE_RETURN_TEXT(CommandPdu::GET_ELEMENT_ATTRIBUTES);
    CASE_RETURN_TEXT(CommandPdu::GET_PLAY_STATUS);
    CASE_RETURN_TEXT(CommandPdu::REGISTER_NOTIFICATION);
    CASE_RETURN_TEXT(CommandPdu::SET_ABSOLUTE_VOLUME);
    CASE_RETURN_TEXT(CommandPdu::SET_ADDRESSED_PLAYER);
    CASE_RETURN_TEXT(CommandPdu::PLAY_ITEM);
    default:
      return "Unknown Command PDU: " + loghex((uint8_t)pdu);
  }
}

inline std::ostream& operator<<(std::ostream& os, const CommandPdu& pdu) {
  return os << CommandPduText(pdu);
}

inline std::string PacketTypeText(const PacketType& type) {
  switch (type) {
    CASE_RETURN_TEXT(PacketType::SINGLE);
    default:
      return "Unknown Packet Type: " + loghex((uint8_t)type);
  }
}

inline std::ostream& operator<<(std::ostream& os, const PacketType& type) {
  return os << PacketTypeText(type);
}

inline std::string CapabilityText(const Capability& cap) {
  switch (cap) {
    CASE_RETURN_TEXT(Capability::COMPANY_ID);
    CASE_RETURN_TEXT(Capability::EVENTS_SUPPORTED);
    default:
      return "Unknown Capability: " + loghex((uint8_t)cap);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Capability& cap) {
  return os << CapabilityText(cap);
}

inline std::string EventText(const Event& event) {
  switch (event) {
    CASE_RETURN_TEXT(Event::PLAYBACK_STATUS_CHANGED);
    CASE_RETURN_TEXT(Event::TRACK_CHANGED);
    CASE_RETURN_TEXT(Event::PLAYBACK_POS_CHANGED);
    CASE_RETURN_TEXT(Event::PLAYER_APPLICATION_SETTING_CHANGED);
    CASE_RETURN_TEXT(Event::NOW_PLAYING_CONTENT_CHANGED);
    CASE_RETURN_TEXT(Event::AVAILABLE_PLAYERS_CHANGED);
    CASE_RETURN_TEXT(Event::ADDRESSED_PLAYER_CHANGED);
    CASE_RETURN_TEXT(Event::UIDS_CHANGED);
    CASE_RETURN_TEXT(Event::VOLUME_CHANGED);
    default:
      return "Unknown Event: " + loghex((uint8_t)event);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
  return os << EventText(event);
}

inline std::string AttributeText(const Attribute& attr) {
  switch (attr) {
    CASE_RETURN_TEXT(Attribute::TITLE);
    CASE_RETURN_TEXT(Attribute::ARTIST_NAME);
    CASE_RETURN_TEXT(Attribute::ALBUM_NAME);
    CASE_RETURN_TEXT(Attribute::TRACK_NUMBER);
    CASE_RETURN_TEXT(Attribute::TOTAL_NUMBER_OF_TRACKS);
    CASE_RETURN_TEXT(Attribute::GENRE);
    CASE_RETURN_TEXT(Attribute::PLAYING_TIME);
    CASE_RETURN_TEXT(Attribute::DEFAULT_COVER_ART);
    default:
      return "Unknown Attribute Value: " + loghex((uint32_t)attr);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Attribute& attr) {
  return os << AttributeText(attr);
}

inline std::string StatusText(const Status& status) {
  switch (status) {
    CASE_RETURN_TEXT(Status::INVALID_COMMAND);
    CASE_RETURN_TEXT(Status::INVALID_PARAMETER);
    CASE_RETURN_TEXT(Status::PARAMETER_CONTENT_ERROR);
    CASE_RETURN_TEXT(Status::INTERNAL_ERROR);
    CASE_RETURN_TEXT(Status::NO_ERROR);
    CASE_RETURN_TEXT(Status::UIDS_CHANGED);
    CASE_RETURN_TEXT(Status::RESERVED);
    CASE_RETURN_TEXT(Status::INVALID_DIRECTION);
    CASE_RETURN_TEXT(Status::NOT_A_DIRECTORY);
    CASE_RETURN_TEXT(Status::DOES_NOT_EXIST);
    CASE_RETURN_TEXT(Status::INVALID_SCOPE);
    CASE_RETURN_TEXT(Status::RANGE_OUT_OF_BOUNDS);
    CASE_RETURN_TEXT(Status::FOLDER_ITEM_NOT_PLAYABLE);
    CASE_RETURN_TEXT(Status::MEDIA_IN_USE);
    CASE_RETURN_TEXT(Status::NOW_PLAYING_LIST_FULL);
    CASE_RETURN_TEXT(Status::SEARCH_NOT_SUPPORTED);
    CASE_RETURN_TEXT(Status::SEARCH_IN_PROGRESS);
    CASE_RETURN_TEXT(Status::INVALID_PLAYER_ID);
    CASE_RETURN_TEXT(Status::PLAYER_NOT_BROWSABLE);
    CASE_RETURN_TEXT(Status::PLAYER_NOT_ADDRESSED);
    CASE_RETURN_TEXT(Status::NO_VALID_SEARCH_RESULTS);
    CASE_RETURN_TEXT(Status::NO_AVAILABLE_PLAYERS);
    CASE_RETURN_TEXT(Status::ADDRESSED_PLAYER_CHANGED);
    default:
      return "Unknown Status: " + loghex((uint8_t)status);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Status& status) {
  return os << StatusText(status);
}

inline std::string BrowsePduText(const BrowsePdu& pdu) {
  switch (pdu) {
    CASE_RETURN_TEXT(BrowsePdu::SET_BROWSED_PLAYER);
    CASE_RETURN_TEXT(BrowsePdu::GET_FOLDER_ITEMS);
    CASE_RETURN_TEXT(BrowsePdu::CHANGE_PATH);
    CASE_RETURN_TEXT(BrowsePdu::GET_ITEM_ATTRIBUTES);
    default:
      return "Unknown Browse PDU: " + loghex((uint8_t)pdu);
  }
}

inline std::ostream& operator<<(std::ostream& os, const BrowsePdu& pdu) {
  return os << BrowsePduText(pdu);
}

inline std::string ScopeText(const Scope& scope) {
  switch (scope) {
    CASE_RETURN_TEXT(Scope::MEDIA_PLAYER_LIST);
    CASE_RETURN_TEXT(Scope::VFS);
    CASE_RETURN_TEXT(Scope::SEARCH);
    CASE_RETURN_TEXT(Scope::NOW_PLAYING);
    default:
      return "Unknown Scope: " + loghex((uint8_t)scope);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Scope& pdu) {
  return os << ScopeText(pdu);
}

inline std::string DirectionText(const Direction& dir) {
  switch (dir) {
    CASE_RETURN_TEXT(Direction::UP);
    CASE_RETURN_TEXT(Direction::DOWN);
    default:
      return "Unknown Direction: " + loghex((uint8_t)dir);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Direction& dir) {
  return os << DirectionText(dir);
}

inline std::string KeyStateText(const KeyState& state) {
  switch (state) {
    CASE_RETURN_TEXT(KeyState::PUSHED);
    CASE_RETURN_TEXT(KeyState::RELEASED);
    default:
      return "Unknown KeyState: " + loghex((uint8_t)state);
  }
}

inline std::ostream& operator<<(std::ostream& os, const KeyState& dir) {
  return os << KeyStateText(dir);
}

}  // namespace avrcp
}  // namespace bluetooth
