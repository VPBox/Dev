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

#include "vendor_packet.h"

namespace bluetooth {
namespace avrcp {

class RegisterNotificationResponse : public VendorPacket {
 public:
  virtual ~RegisterNotificationResponse() = default;

  /**
   *  Register Notificaiton Request Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t param_length;
   *   RegisterNotificationRequestPacket:
   *     uint8_t event_id;
   *     uint8_t[] data;  // Length changes based on the event_id
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 1; }

  // TODO (apanicke): Add other getters when implementing AVRCP Controller
  bool IsInterim() const;
  Event GetEvent() const;
  uint8_t GetVolume() const;

  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

class RegisterNotificationResponseBuilder : public VendorPacketBuilder {
 public:
  virtual ~RegisterNotificationResponseBuilder() = default;

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakePlaybackStatusBuilder(bool interim, uint8_t play_status);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakeTrackChangedBuilder(bool interim, uint64_t track_uid);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakePlaybackPositionBuilder(bool interim, uint32_t playback_pos);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakeNowPlayingBuilder(bool interim);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakeAvailablePlayersBuilder(bool interim);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakeAddressedPlayerBuilder(bool interim, uint16_t player_id,
                             uint16_t uid_counter);

  static std::unique_ptr<RegisterNotificationResponseBuilder>
  MakeUidsChangedBuilder(bool interim, uint16_t uid_counter);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Event event_;
  uint64_t data_;

  RegisterNotificationResponseBuilder(bool interim, Event event)
      : VendorPacketBuilder(interim ? CType::INTERIM : CType::CHANGED,
                            CommandPdu::REGISTER_NOTIFICATION,
                            PacketType::SINGLE),
        event_(event){};
};

class RegisterNotificationRequest : public VendorPacket {
 public:
  virtual ~RegisterNotificationRequest() = default;

  /**
   *  Register Notificaiton Request Packet Layout
   *   AvrcpPacket:
   *     CType c_type_;
   *     uint8_t subunit_type_ : 5;
   *     uint8_t subunit_id_ : 3;
   *     Opcode opcode_;
   *   VendorPacket:
   *     uint8_t company_id[3];
   *     uint8_t command_pdu;
   *     uint8_t packet_type;
   *     uint16_t param_length;
   *   RegisterNotificationRequestPacket:
   *     uint8_t event_id;
   *     uint32_t interval;  // Only used for PLAYBACK_POS_CHANGED
   */
  static constexpr size_t kMinSize() { return VendorPacket::kMinSize() + 5; }

  // Getter Functions
  Event GetEventRegistered() const;
  uint32_t GetInterval() const;

  // Overloaded Functions
  virtual bool IsValid() const override;
  virtual std::string ToString() const override;

 protected:
  using VendorPacket::VendorPacket;
};

class RegisterNotificationRequestBuilder : public VendorPacketBuilder {
 public:
  virtual ~RegisterNotificationRequestBuilder() = default;

  static std::unique_ptr<RegisterNotificationRequestBuilder> MakeBuilder(
      Event event, uint32_t interval);

  virtual size_t size() const override;
  virtual bool Serialize(
      const std::shared_ptr<::bluetooth::Packet>& pkt) override;

 protected:
  Event event_;
  uint32_t interval_;

  RegisterNotificationRequestBuilder(Event event, uint32_t interval)
      : VendorPacketBuilder(CType::NOTIFY, CommandPdu::REGISTER_NOTIFICATION,
                            PacketType::SINGLE),
        event_(event),
        interval_(interval){};
};

}  // namespace avrcp
}  // namespace bluetooth