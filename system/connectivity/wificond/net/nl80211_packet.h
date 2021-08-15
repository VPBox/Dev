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

#ifndef WIFICOND_NET_NL80211_PACKET_H_
#define WIFICOND_NET_NL80211_PACKET_H_

#include <memory>
#include <vector>

#include <linux/genetlink.h>
#include <linux/netlink.h>

#include <android-base/macros.h>

#include "wificond/net/nl80211_attribute.h"

namespace android {
namespace wificond {

// NL80211Packets are used to communicate with the cfg80211 kernel subsystem
// (via the nl80211 interface).  An NL80211 packet is a type of generic netlink
// packet (i.e. it includes netlink and generic netlink headers).
// In order to simplify the design, we also allow this class to represent a
// few types of netlink control messages. In this way the API user is supposed to
// call IsValid() and GetMessageType() in the first place to avoid misuse of
// this class.
class NL80211Packet {
 public:
  // This is used for creating a NL80211Packet from buffer.
  explicit NL80211Packet(const std::vector<uint8_t>& data);
  // This is used for creating an empty NL80211Packet to be filled later.
  // See comment of SetMessageType() for |type|.
  // See comment of SetCommand() for |command|.
  // See comment of SetMessageSequence() for |sequence|.
  // See comment of SetPortId() for |pid|.
  NL80211Packet(uint16_t type,
                uint8_t command,
                uint32_t sequence,
                uint32_t pid);
  // We don't copy NL80211Packet for performance reasons.
  // However we keep this copy constructor because it makes unit tests easy.
  // It prints WARNING log when this copy constructor is called.
  NL80211Packet(const NL80211Packet& packet);
  // Explicitly specify the move constructor. Otherwise, copy constructor will
  // be called on if we move a NL80211Packet object.
  NL80211Packet(NL80211Packet&& packet) = default;
  ~NL80211Packet() = default;

  // Returns whether a packet has consistent header fields.
  bool IsValid() const;

  // Helper functions for Netlink flags.

  // Requesting a dump of a kernel obejct.
  bool IsDump() const;
  // Multipart messages are used to send lists or trees of objects.
  // They are supposed to be parsed independently, and must not be reassembled.
  // Multipart messages are terminated by NLMSG_DONE, which should be returned
  // by GetMessageType().
  bool IsMulti() const;

  // Getter functions.
  uint8_t GetCommand() const;
  uint16_t GetFlags() const;
  uint16_t GetMessageType() const;
  uint32_t GetMessageSequence() const;
  uint32_t GetPortId() const;
  // Caller is responsible for checking that this is a valid
  // NLMSG_ERROR message before calling GetErrorCode().
  // Returns an error number defined in errno.h
  int GetErrorCode() const;
  const std::vector<uint8_t>& GetConstData() const;

  // Setter functions.

  // In the contexet of nl80211 messages,
  // |command| is one of |enum nl80211_commands| in nl80211.h
  void SetCommand(uint8_t command);
  // |flags| is set of flag bits described by NLM_F_* macros in netlink.h
  void AddFlag(uint16_t flag);
  void SetFlags(uint16_t flags);
  // In the context of nl80211 messages,
  // message type is a nl80211 message family id dynamiclly allocated by kernel.
  // If this is a control message, it could be one of the following value:
  // NLMSG_NOOP, NLMSG_ERROR, NLMSG_DONE, NLMSG_OVERRUN
  void SetMessageType(uint16_t message_type);
  // Requests should carry a sequence number incremented for each request sent.
  // For reply message, the sequence number is used to allow referring to a
  // previous message with the same sequence number.
  void SetMessageSequence(uint32_t message_sequemce);
  // Set nlmsg_pid in netlink header.
  // nlmsg_pid is the sender process port ID.
  // It is *not* associated with a process but a netlink socket.
  // We should use sockaddr_nl.nl_pid from sender socket.
  // This value should be 0 if message is from kernel.
  // See man 7 netlink for details.
  void SetPortId(uint32_t pid);

  void AddAttribute(const BaseNL80211Attr& attribute);
  // For NLA_FLAG attribute
  void AddFlagAttribute(int attribute_id);

  bool HasAttribute(int id) const;
  bool GetAttribute(int id, NL80211NestedAttr* attribute) const;
  // Get all attributes to |*attribute| as a vector.
  // In case of failure, attributes up until the first invalid attribute
  // actually will be present in |attributes|.
  bool GetAllAttributes(
      std::vector<BaseNL80211Attr>* attributes) const;

  template <typename T>
  bool GetAttributeValue(int id, T* value) const {
    std::vector<uint8_t> empty_vec;
    // All data in |attribute| created here will be overwritten by
    // GetAttribute(). So we use an empty vector to initialize it,
    // regardless of the fact that an empty buffer is not qualified
    // for creating a valid attribute.
    NL80211Attr<T> attribute(empty_vec);
    if (!GetAttribute(id, &attribute)) {
      return false;
    }
    *value = attribute.GetValue();
    return true;
  }

  template <typename T>
  bool GetAttribute(int id, NL80211Attr<T>* attribute) const {
    uint8_t* start = nullptr;
    uint8_t* end = nullptr;
    if (!BaseNL80211Attr::GetAttributeImpl(
            data_.data() + NLMSG_HDRLEN + GENL_HDRLEN,
            data_.size() - NLMSG_HDRLEN - GENL_HDRLEN,
            id, &start, &end) ||
        start == nullptr ||
        end == nullptr) {
      return false;
    }
    *attribute = NL80211Attr<T>(std::vector<uint8_t>(start, end));
    if (!attribute->IsValid()) {
      return false;
    }
    return true;
  }

  void DebugLog() const;

 private:
  std::vector<uint8_t> data_;
};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_NL80211_PACKET_H_
