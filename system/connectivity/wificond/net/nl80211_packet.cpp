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

#include "wificond/net/nl80211_packet.h"

#include <android-base/logging.h>

using std::vector;

namespace android {
namespace wificond {

NL80211Packet::NL80211Packet(const vector<uint8_t>& data)
    : data_(data) {
  data_ = data;
}

NL80211Packet::NL80211Packet(const NL80211Packet& packet) {
  data_ = packet.data_;
  LOG(WARNING) << "Copy constructor is only used for unit tests";
}

NL80211Packet::NL80211Packet(uint16_t type,
                             uint8_t command,
                             uint32_t sequence,
                             uint32_t pid) {
  // Initialize the netlink header and generic netlink header.
  // NLMSG_HDRLEN and GENL_HDRLEN already include the padding size.
  data_.resize(NLMSG_HDRLEN + GENL_HDRLEN, 0);
  // Initialize length field.
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_len = data_.size();
  // Add NLM_F_REQUEST flag.
  nl_header->nlmsg_flags = nl_header->nlmsg_flags | NLM_F_REQUEST;
  nl_header->nlmsg_type = type;
  nl_header->nlmsg_seq = sequence;
  nl_header->nlmsg_pid = pid;

  genlmsghdr* genl_header =
      reinterpret_cast<genlmsghdr*>(data_.data() + NLMSG_HDRLEN);
  genl_header->version = 1;
  genl_header->cmd = command;
  // genl_header->reserved is aready 0.
}

bool NL80211Packet::IsValid() const {
  // Verify the size of packet.
  if (data_.size() < NLMSG_HDRLEN) {
    LOG(ERROR) << "Cannot retrieve netlink header.";
    return false;
  }

  const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(data_.data());

  // If type < NLMSG_MIN_TYPE, this should be a reserved control message,
  // which doesn't carry a generic netlink header.
  if (GetMessageType() >= NLMSG_MIN_TYPE) {
    if (data_.size() < NLMSG_HDRLEN + GENL_HDRLEN ||
        nl_header->nlmsg_len < NLMSG_HDRLEN + GENL_HDRLEN) {
      LOG(ERROR) << "Cannot retrieve generic netlink header.";
      return false;
    }
  }
  // If it is an ERROR message, it should be long enough to carry an extra error
  // code field.
  // Kernel uses int for this field.
  if (GetMessageType() == NLMSG_ERROR) {
    if (data_.size() < NLMSG_HDRLEN + sizeof(int) ||
        nl_header->nlmsg_len < NLMSG_HDRLEN + sizeof(int)) {
     LOG(ERROR) << "Broken error message.";
     return false;
    }
  }

  // Verify the netlink header.
  if (data_.size() < nl_header->nlmsg_len ||
      nl_header->nlmsg_len < sizeof(nlmsghdr)) {
    LOG(ERROR) << "Discarding incomplete / invalid message.";
    return false;
  }
  return true;
}

bool NL80211Packet::IsDump() const {
  return GetFlags() & NLM_F_DUMP;
}

bool NL80211Packet::IsMulti() const {
  return GetFlags() & NLM_F_MULTI;
}

uint8_t NL80211Packet::GetCommand() const {
  const genlmsghdr* genl_header = reinterpret_cast<const genlmsghdr*>(
      data_.data() + NLMSG_HDRLEN);
  return genl_header->cmd;
}

uint16_t NL80211Packet::GetFlags() const {
  const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(data_.data());
  return nl_header->nlmsg_flags;
}

uint16_t NL80211Packet::GetMessageType() const {
  const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(data_.data());
  return nl_header->nlmsg_type;
}

uint32_t NL80211Packet::GetMessageSequence() const {
  const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(data_.data());
  return nl_header->nlmsg_seq;
}

uint32_t NL80211Packet::GetPortId() const {
  const nlmsghdr* nl_header = reinterpret_cast<const nlmsghdr*>(data_.data());
  return nl_header->nlmsg_pid;
}

int NL80211Packet::GetErrorCode() const {
  return -*reinterpret_cast<const int*>(data_.data() + NLMSG_HDRLEN);
}

const vector<uint8_t>& NL80211Packet::GetConstData() const {
  return data_;
}

void NL80211Packet::SetCommand(uint8_t command) {
  genlmsghdr* genl_header = reinterpret_cast<genlmsghdr*>(
      data_.data() + NLMSG_HDRLEN);
  genl_header->cmd = command;
}

void NL80211Packet::AddFlag(uint16_t flag) {
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_flags |= flag;
}

void NL80211Packet::SetFlags(uint16_t flags) {
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_flags = flags;
}

void NL80211Packet::SetMessageType(uint16_t message_type) {
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_type = message_type;
}

void NL80211Packet::SetMessageSequence(uint32_t message_sequence) {
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_seq = message_sequence;
}

void NL80211Packet::SetPortId(uint32_t pid) {
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_pid = pid;
}

void NL80211Packet::AddAttribute(const BaseNL80211Attr& attribute) {
  const vector<uint8_t>& append_data = attribute.GetConstData();
  // Append the data of |attribute| to |this|.
  data_.insert(data_.end(), append_data.begin(), append_data.end());
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  // We don't need to worry about padding for a nl80211 packet.
  // Because as long as all sub attributes have padding, the payload is aligned.
  nl_header->nlmsg_len += append_data.size();
}

void NL80211Packet::AddFlagAttribute(int attribute_id) {
  // We only need to append a header for flag attribute.
  // Make space for the new attribute.
  data_.resize(data_.size() + NLA_HDRLEN, 0);
  nlattr* flag_header =
      reinterpret_cast<nlattr*>(data_.data() + data_.size() - NLA_HDRLEN);
  flag_header->nla_type = attribute_id;
  flag_header->nla_len = NLA_HDRLEN;
  nlmsghdr* nl_header = reinterpret_cast<nlmsghdr*>(data_.data());
  nl_header->nlmsg_len += NLA_HDRLEN;
}

bool NL80211Packet::HasAttribute(int id) const {
  return BaseNL80211Attr::GetAttributeImpl(
      data_.data() + NLMSG_HDRLEN + GENL_HDRLEN,
      data_.size() - NLMSG_HDRLEN - GENL_HDRLEN,
      id, nullptr, nullptr);
}

bool NL80211Packet::GetAttribute(int id,
    NL80211NestedAttr* attribute) const {
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
  *attribute = NL80211NestedAttr(vector<uint8_t>(start, end));
  if (!attribute->IsValid()) {
    return false;
  }
  return true;
}

bool NL80211Packet::GetAllAttributes(
    vector<BaseNL80211Attr>* attributes) const {
  const uint8_t* ptr = data_.data() + NLMSG_HDRLEN + GENL_HDRLEN;
  const uint8_t* end_ptr = data_.data() + data_.size();
  while (ptr + NLA_HDRLEN <= end_ptr) {
    auto header = reinterpret_cast<const nlattr*>(ptr);
    if (ptr + NLA_ALIGN(header->nla_len) > end_ptr ||
      header->nla_len == 0) {
      LOG(ERROR) << "broken nl80211 atrribute.";
      return false;
    }
    attributes->emplace_back(
        header->nla_type,
        vector<uint8_t>(ptr + NLA_HDRLEN, ptr + header->nla_len));
    ptr += NLA_ALIGN(header->nla_len);
  }
  return true;
}

void NL80211Packet::DebugLog() const {
  const uint8_t* ptr = data_.data() + NLMSG_HDRLEN + GENL_HDRLEN;
  const uint8_t* end_ptr = data_.data() + data_.size();
  while (ptr + NLA_HDRLEN <= end_ptr) {
    const nlattr* header = reinterpret_cast<const nlattr*>(ptr);
    if (ptr + NLA_ALIGN(header->nla_len) > end_ptr) {
      LOG(ERROR) << "broken nl80211 atrribute.";
      return;
    }
    LOG(INFO) << "Have attribute with nla_type=" << header->nla_type
              << " and nla_len=" << header->nla_len;
    if (header->nla_len == 0) {
      LOG(ERROR) << "0 is a bad nla_len";
      return;
    }
    ptr += NLA_ALIGN(header->nla_len);
  }
}

}  // namespace wificond
}  // namespace android
