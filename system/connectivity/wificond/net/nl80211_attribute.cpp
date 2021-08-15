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

#include "wificond/net/nl80211_attribute.h"

using std::string;
using std::vector;

namespace android {
namespace wificond {

// Explicit instantiation
template class NL80211Attr<uint8_t>;
template class NL80211Attr<uint16_t>;
template class NL80211Attr<uint32_t>;
template class NL80211Attr<uint64_t>;
template class NL80211Attr<vector<uint8_t>>;
template class NL80211Attr<string>;

// For BaseNL80211Attr

BaseNL80211Attr::BaseNL80211Attr(int id,
    const vector<uint8_t>& raw_buffer) {
  size_t size = raw_buffer.size();
  InitHeaderAndResize(id, size);
  memcpy(data_.data() + NLA_HDRLEN, raw_buffer.data(), raw_buffer.size());
}

void BaseNL80211Attr::InitHeaderAndResize(int attribute_id,
                                          int payload_length) {
  data_.resize(NLA_HDRLEN + NLA_ALIGN(payload_length), 0);
  nlattr* header = reinterpret_cast<nlattr*>(data_.data());
  header->nla_type = attribute_id;
  header->nla_len = NLA_HDRLEN + payload_length;
}

int BaseNL80211Attr::GetAttributeId() const {
  const nlattr* header = reinterpret_cast<const nlattr*>(data_.data());
  return header->nla_type;
}

bool BaseNL80211Attr::IsValid() const {
  if (data_.size() < NLA_HDRLEN) {
    return false;
  }
  const nlattr* header = reinterpret_cast<const nlattr*>(data_.data());
  return NLA_ALIGN(header->nla_len) == data_.size();
}

const vector<uint8_t>& BaseNL80211Attr::GetConstData() const {
  return data_;
}

bool BaseNL80211Attr::GetAttributeImpl(const uint8_t* buf,
                                       size_t len,
                                       int attr_id,
                                       uint8_t** attr_start,
                                       uint8_t** attr_end) {
  // Skip the top level attribute header.
  const uint8_t* ptr = buf;
  const uint8_t* end_ptr = buf + len;
  while (ptr + NLA_HDRLEN <= end_ptr) {
    const nlattr* header = reinterpret_cast<const nlattr*>(ptr);
    if (header->nla_type == attr_id) {
      if (ptr + NLA_ALIGN(header->nla_len) > end_ptr) {
        LOG(ERROR) << "Failed to get attribute: broken nl80211 atrribute.";
        return false;
      }
      if (attr_start != nullptr && attr_end != nullptr) {
        *attr_start = const_cast<uint8_t*>(ptr);
        *attr_end = const_cast<uint8_t*>(ptr + NLA_ALIGN(header->nla_len));
      }
      return true;
    }
    ptr += NLA_ALIGN(header->nla_len);
  }
  return false;
}


bool BaseNL80211Attr::Merge(const BaseNL80211Attr& other_attr) {
  if (!other_attr.IsValid()) {
    LOG(ERROR) << "Can not merge invalid attribute";
    return false;
  }
  if (GetAttributeId() != other_attr.GetAttributeId()) {
    LOG(ERROR) << "Can not merge attributes with different ids";
    return false;
  }

  auto our_header = reinterpret_cast<nlattr*>(data_.data());
  int our_len_without_padding = our_header->nla_len;
  auto other_header =
      reinterpret_cast<const nlattr*>(other_attr.GetConstData().data());
  int other_len_without_padding = other_header->nla_len;
  // Update the length to include the content of |other_attr|.
  int total_len_without_padding =
      our_len_without_padding + other_len_without_padding - NLA_HDRLEN;
  our_header->nla_len = total_len_without_padding;

  // Remove padding 0s.
  data_.resize(our_len_without_padding);
  // Insert content of |other_attr|.
  data_.insert(
      data_.end(),
      reinterpret_cast<const uint8_t*>(other_header) + NLA_HDRLEN,
      reinterpret_cast<const uint8_t*>(other_header) +
          other_len_without_padding);
  // Add padding 0s.
  data_.resize(NLA_ALIGN(total_len_without_padding), 0);
  return true;
}

// For NL80211Attr<std::vector<uint8_t>>
NL80211Attr<vector<uint8_t>>::NL80211Attr(int id,
    const vector<uint8_t>& raw_buffer) : BaseNL80211Attr(id, raw_buffer) {
}

NL80211Attr<vector<uint8_t>>::NL80211Attr(
    const vector<uint8_t>& data) {
  data_ = data;
}

vector<uint8_t> NL80211Attr<vector<uint8_t>>::GetValue() const {
  const nlattr* header = reinterpret_cast<const nlattr*>(data_.data());
  return vector<uint8_t>(
      data_.data() + NLA_HDRLEN,
      data_.data() + header->nla_len);
}

// For NL80211Attr<std::string>
NL80211Attr<string>::NL80211Attr(int id, const string& str) {
  size_t size = str.size();
  // This string is storaged as a null-terminated string.
  // Buffer is initialized with 0s so we only need to make a space for
  // the null terminator.
  InitHeaderAndResize(id, size + 1);
  char* storage = reinterpret_cast<char*>(data_.data() + NLA_HDRLEN);
  str.copy(storage, size);
}

NL80211Attr<string>::NL80211Attr(const vector<uint8_t>& data) {
  data_ = data;
}

string NL80211Attr<string>::GetValue() const {
  const nlattr* header = reinterpret_cast<const nlattr*>(data_.data());
  size_t str_length = header->nla_len - NLA_HDRLEN;
  // Remove trailing zeros.
  while (str_length > 0 &&
         *(data_.data() + NLA_HDRLEN + str_length - 1) == 0) {
    str_length--;
  }
  return string(reinterpret_cast<const char*>(data_.data() + NLA_HDRLEN),
                str_length);
}

// For NL80211NestedAttr
NL80211NestedAttr::NL80211NestedAttr(int id) {
  InitHeaderAndResize(id, 0);
}

NL80211NestedAttr::NL80211NestedAttr(const vector<uint8_t>& data) {
  data_ = data;
}

void NL80211NestedAttr::AddAttribute(const BaseNL80211Attr& attribute) {
  const vector<uint8_t>& append_data = attribute.GetConstData();
  // Append the data of |attribute| to |this|.
  data_.insert(data_.end(), append_data.begin(), append_data.end());
  nlattr* header = reinterpret_cast<nlattr*>(data_.data());
  // We don't need to worry about padding for nested attribute.
  // Because as long as all sub attributes have padding, the payload is aligned.
  header->nla_len += append_data.size();
}

void NL80211NestedAttr::AddFlagAttribute(int attribute_id) {
  // We only need to append a header for flag attribute.
  // Make space for the new attribute.
  data_.resize(data_.size() + NLA_HDRLEN, 0);
  nlattr* flag_header =
      reinterpret_cast<nlattr*>(data_.data() + data_.size() - NLA_HDRLEN);
  flag_header->nla_type = attribute_id;
  flag_header->nla_len = NLA_HDRLEN;
  nlattr* nl_header = reinterpret_cast<nlattr*>(data_.data());
  nl_header->nla_len += NLA_HDRLEN;
}

bool NL80211NestedAttr::HasAttribute(int id) const {
  return BaseNL80211Attr::GetAttributeImpl(data_.data() + NLA_HDRLEN,
                                           data_.size() - NLA_HDRLEN,
                                           id, nullptr, nullptr);
}

bool NL80211NestedAttr::GetAttribute(int id,
    NL80211NestedAttr* attribute) const {
  uint8_t* start = nullptr;
  uint8_t* end = nullptr;
  if (!BaseNL80211Attr::GetAttributeImpl(data_.data() + NLA_HDRLEN,
                                         data_.size() - NLA_HDRLEN,
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

bool NL80211NestedAttr::GetListOfNestedAttributes(
    vector<NL80211NestedAttr>* value) const {
  const uint8_t* ptr = data_.data() + NLA_HDRLEN;
  const uint8_t* end_ptr = data_.data() + data_.size();
  vector<NL80211NestedAttr> nested_attr_list;
  while (ptr + NLA_HDRLEN <= end_ptr) {
    const nlattr* header = reinterpret_cast<const nlattr*>(ptr);
    if (ptr + NLA_ALIGN(header->nla_len) > end_ptr) {
      LOG(ERROR) << "Failed to get list of nested attributes: invalid nla_len.";
      return false;
    }
    nested_attr_list.emplace_back(
        NL80211NestedAttr(vector<uint8_t>(ptr,
                                          ptr + NLA_ALIGN(header->nla_len))));
    if (!nested_attr_list.back().IsValid()) {
      return false;
    }
    ptr += NLA_ALIGN(header->nla_len);
  }
  *value = std::move(nested_attr_list);
  return true;
}


void NL80211NestedAttr::DebugLog() const {
  const uint8_t* ptr = data_.data() + NLA_HDRLEN;
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
