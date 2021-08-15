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

#ifndef WIFICOND_NET_NL80211_ATTRIBUTE_H_
#define WIFICOND_NET_NL80211_ATTRIBUTE_H_

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <linux/netlink.h>

#include <android-base/logging.h>
#include <android-base/macros.h>

namespace android {
namespace wificond {

class BaseNL80211Attr {
 public:
  BaseNL80211Attr(int id, const std::vector<uint8_t>& raw_buffer);
  virtual ~BaseNL80211Attr() = default;

  const std::vector<uint8_t>& GetConstData() const;
  int GetAttributeId() const;
  // This is used when we initialize a NL80211 attribute from an existing
  // buffer.
  virtual bool IsValid() const;
  // A util helper function to find a specific sub attribute from a buffer.
  // This buffer is supposed to be from a nested attribute or a nl80211 packet.
  // |*start| and |*end| are the start and end pointers of buffer where
  // |id| atrribute locates.
  static bool GetAttributeImpl(const uint8_t* buf,
                              size_t len,
                              int attr_id,
                              uint8_t** attr_start,
                              uint8_t** attr_end);
  // Merge the payload of |attr| to current attribute.
  // This is only used for merging attribute from the response of split dump.
  // Returns true on success.
  bool Merge(const BaseNL80211Attr& attr);

 protected:
  BaseNL80211Attr() = default;
  void InitHeaderAndResize(int attribute_id, int payload_length);

  std::vector<uint8_t> data_;
};

template <typename T>
class NL80211Attr : public BaseNL80211Attr {
 public:
  NL80211Attr(int id, T value) {
    static_assert(
        std::is_integral<T>::value,
        "Failed to create NL80211Attr class with non-integral type");
    InitHeaderAndResize(id, sizeof(T));
    T* storage = reinterpret_cast<T*>(data_.data() + NLA_HDRLEN);
    *storage = value;
  }
  // Caller is responsible for ensuring that |data| is:
  //   1) Is at least NLA_HDRLEN long.
  //   2) That *data when interpreted as a nlattr is internally consistent.
  // (e.g. data.size() == NLA_ALIGN(nlattr.nla_len)
  // and nla_len == NLA_HDRLEN + payload size
  explicit NL80211Attr(const std::vector<uint8_t>& data) {
    data_ = data;
  }

  ~NL80211Attr() override = default;

  bool IsValid() const override {
    if (!BaseNL80211Attr::IsValid()) {
      return false;
    }
    // If BaseNL80211Attr::IsValid() == true, at least we have enough valid
    // buffer for header.
    const nlattr* header = reinterpret_cast<const nlattr*>(data_.data());
    // Buffer size = header size +  payload size + padding size
    // nla_len  =  header size + payload size
    if (NLA_ALIGN(sizeof(T)) + NLA_HDRLEN != data_.size() ||
        sizeof(T) + NLA_HDRLEN != header->nla_len ) {
      return false;
    }
    return true;
  }

  T GetValue() const {
    return *reinterpret_cast<const T*>(data_.data() + NLA_HDRLEN);
  }
};  // class NL80211Attr for POD-types

template <>
class NL80211Attr<std::vector<uint8_t>> : public BaseNL80211Attr {
 public:
  NL80211Attr(int id, const std::vector<uint8_t>& raw_buffer);
  explicit NL80211Attr(const std::vector<uint8_t>& data);
  ~NL80211Attr() override = default;
  std::vector<uint8_t> GetValue() const;
}; // class NL80211Attr for raw data

template <size_t N>
class NL80211Attr<std::array<uint8_t, N>> : public BaseNL80211Attr {
 public:
  NL80211Attr(int id, const std::array<uint8_t, N>& raw_buffer)
      : BaseNL80211Attr(
          id, std::vector<uint8_t>(raw_buffer.begin(), raw_buffer.end())) {}
  explicit NL80211Attr(const std::vector<uint8_t>& data) {
    data_ = data;
  }
  ~NL80211Attr() override = default;
  std::array<uint8_t, N> GetValue() const {
    std::array<uint8_t, N> arr;
    std::copy_n(data_.data() + NLA_HDRLEN, N, arr.begin());
    return arr;
  }
}; // class NL80211Attr for fixed size array

template <>
class NL80211Attr<std::string> : public BaseNL80211Attr {
 public:
  NL80211Attr(int id, const std::string& str);
  // We parse string attribute buffer in the same way kernel does.
  // All trailing zeros are trimmed when retrieving a std::string from
  // byte array.
  explicit NL80211Attr(const std::vector<uint8_t>& data);
  ~NL80211Attr() override = default;
  std::string GetValue() const;
};  // class NL80211Attr for string

// Force the compiler not to instantiate these templates because
// they will be instantiated in nl80211_attribute.cpp file. This helps
// reduce compile time as well as object file size.
extern template class NL80211Attr<uint8_t>;
extern template class NL80211Attr<uint16_t>;
extern template class NL80211Attr<uint32_t>;
extern template class NL80211Attr<uint64_t>;
extern template class NL80211Attr<std::vector<uint8_t>>;
extern template class NL80211Attr<std::string>;

class NL80211NestedAttr : public BaseNL80211Attr {
 public:
  explicit NL80211NestedAttr(int id);
  explicit NL80211NestedAttr(const std::vector<uint8_t>& data);
  ~NL80211NestedAttr() override = default;

  void AddAttribute(const BaseNL80211Attr& attribute);
  // For NLA_FLAG attribute
  void AddFlagAttribute(int attribute_id);
  bool HasAttribute(int id) const;

  // Access an attribute nested within |this|.
  // The result is returned by writing the attribute object to |*attribute|.
  // Deeper nested attributes are not included. This means if A is nested within
  // |this|, and B is nested within A, this function can't be used to access B.
  // The reason is that we may have multiple attributes having the same
  // attribute id, nested within different level of |this|.
  bool GetAttribute(int id, NL80211NestedAttr* attribute) const;

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

  // Some of the nested attribute contains a list of same type sub-attributes.
  // This function retrieves a vector of attribute values from a nested
  // attribute.
  //
  // This is for both correctness and performance reasons: Refer to
  // GetListOfAttributes().
  //
  // Returns true on success.
  template <typename T>
  bool GetListOfAttributeValues(std::vector<T>* value) const {
    const uint8_t* ptr = data_.data() + NLA_HDRLEN;
    const uint8_t* end_ptr = data_.data() + data_.size();
    std::vector<T> attr_list;
    while (ptr + NLA_HDRLEN <= end_ptr) {
      const nlattr* header = reinterpret_cast<const nlattr*>(ptr);
      if (ptr + NLA_ALIGN(header->nla_len) > end_ptr) {
        LOG(ERROR) << "Failed to get list of attributes: invalid nla_len.";
        return false;
      }
      NL80211Attr<T> attribute(std::vector<uint8_t>(
          ptr,
          ptr + NLA_ALIGN(header->nla_len)));
      if (!attribute.IsValid()) {
        return false;
      }
      attr_list.emplace_back(attribute.GetValue());
      ptr += NLA_ALIGN(header->nla_len);
    }
    *value = std::move(attr_list);
    return true;
  }

  // Some of the nested attribute contains a list of same type sub-attributes.
  // This function retrieves a vector of attributes from a nested
  // attribute.
  //
  // This is for both correctness and performance reasons:
  // Correctness reason:
  // These sub-attributes have attribute id from '0 to n' or '1 to n'.
  // There is no document defining what the start index should be.
  //
  // Performance reson:
  // Calling GetAttribute() from '0 to n' results a n^2 time complexity.
  // This function get a list of attributes in one pass.
  //
  // Returns true on success.
  template <typename T>
  bool GetListOfAttributes(std::vector<NL80211Attr<T>>* value) const {
    const uint8_t* ptr = data_.data() + NLA_HDRLEN;
    const uint8_t* end_ptr = data_.data() + data_.size();
    std::vector<NL80211Attr<T>> attr_list;
    while (ptr + NLA_HDRLEN <= end_ptr) {
      const nlattr* header = reinterpret_cast<const nlattr*>(ptr);
      if (ptr + NLA_ALIGN(header->nla_len) > end_ptr) {
        LOG(ERROR) << "Failed to get list of attributes: invalid nla_len.";
        return false;
      }
      NL80211Attr<T> attribute(std::vector<uint8_t>(
          ptr,
          ptr + NLA_ALIGN(header->nla_len)));
      if (!attribute.IsValid()) {
        return false;
      }
      attr_list.emplace_back(attribute);
      ptr += NLA_ALIGN(header->nla_len);
    }
    *value = std::move(attr_list);
    return true;
  }

  // This is similar to |GetListOfAttributeValues|, but for the cases where all
  // the sub-attributes are nested attributes.
  bool GetListOfNestedAttributes(std::vector<NL80211NestedAttr>* value) const;

  template <typename T>
  bool GetAttribute(int id, NL80211Attr<T>* attribute) const {
    uint8_t* start = nullptr;
    uint8_t* end = nullptr;
    if (!BaseNL80211Attr::GetAttributeImpl(data_.data() + NLA_HDRLEN,
                                           data_.size() - NLA_HDRLEN,
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

};

}  // namespace wificond
}  // namespace android

#endif  // WIFICOND_NET_NL80211_ATTRIBUTE_H_
