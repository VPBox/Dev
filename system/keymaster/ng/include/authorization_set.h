/*
 * Copyright 2014 The Android Open Source Project
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

#ifndef SYSTEM_SECURITY_KEYSTORE_AUTHORIZATION_SET_H_
#define SYSTEM_SECURITY_KEYSTORE_AUTHORIZATION_SET_H_

#include "keymaster_tags.h"
#include <vector>

namespace keymaster {
namespace ng {

class AuthorizationSetBuilder;

/**
 * An ordered collection of KeyParameters. It provides memory ownership and some convenient
 * functionality for sorting, deduplicating, joining, and subtracting sets of KeyParameters.
 * For serialization, wrap the backing store of this structure in a hidl_vec<KeyParameter>.
 */
class AuthorizationSet {
  public:
    /**
     * Construct an empty, dynamically-allocated, growable AuthorizationSet.
     */
    AuthorizationSet() {};

    // Copy constructor.
    AuthorizationSet(const AuthorizationSet& other) : data_(other.data_) {}

    // Move constructor.
    AuthorizationSet(AuthorizationSet&& other) : data_(std::move(other.data_)) {}

    // Constructor from hidl_vec<KeyParameter>
    explicit AuthorizationSet(const hidl_vec<KeyParameter>& other) { *this = other; }

    // Copy assignment.
    AuthorizationSet& operator=(const AuthorizationSet& other) {
        data_ = other.data_;
        return *this;
    }

    // Move assignment.
    AuthorizationSet& operator=(AuthorizationSet&& other) {
        data_ = std::move(other.data_);
        return *this;
    }

    AuthorizationSet& operator=(const hidl_vec<KeyParameter>& other) {
        if (other.size() > 0) {
            data_.resize(other.size());
            for (size_t i = 0; i < data_.size(); ++i) {
                /* This makes a deep copy even of embedded blobs.
                 * See assignment operator/copy constructor of hidl_vec.*/
                data_[i] = other[i];
            }
        }
        return *this;
    }

    /**
     * Clear existing authorization set data
     */
    void Clear();

    ~AuthorizationSet() = default;

    /**
     * Returns the size of the set.
     */
    size_t size() const { return data_.size(); }

    /**
     * Returns true if the set is empty.
     */
    bool empty() const { return size() == 0; }

    /**
     * Returns the data in the set, directly. Be careful with this.
     */
    const KeyParameter* data() const { return data_.data(); }

    /**
     * Sorts the set
     */
    void Sort();

    /**
     * Sorts the set and removes duplicates (inadvertently duplicating tags is easy to do with the
     * AuthorizationSetBuilder).
     */
    void Deduplicate();

    /**
     * Adds all elements from \p set that are not already present in this AuthorizationSet.  As a
     * side-effect, if \p set is not null this AuthorizationSet will end up sorted.
     */
    void Union(const AuthorizationSet& set);

    /**
     * Removes all elements in \p set from this AuthorizationSet.
     */
    void Subtract(const AuthorizationSet& set);

    /**
     * Returns the offset of the next entry that matches \p tag, starting from the element after \p
     * begin.  If not found, returns -1.
     */
    int find(Tag tag, int begin = -1) const;

    /**
     * Removes the entry at the specified index. Returns true if successful, false if the index was
     * out of bounds.
     */
    bool erase(int index);

    /**
     * Returns iterator (pointer) to beginning of elems array, to enable STL-style iteration
     */
    std::vector<KeyParameter>::const_iterator begin() const { return data_.begin(); }

    /**
     * Returns iterator (pointer) one past end of elems array, to enable STL-style iteration
     */
    std::vector<KeyParameter>::const_iterator end() const { return data_.end(); }

    /**
     * Returns the nth element of the set.
     * Like for std::vector::operator[] there is no range check performed. Use of out of range
     * indices is undefined.
     */
    KeyParameter& operator[](int n);

    /**
     * Returns the nth element of the set.
     * Like for std::vector::operator[] there is no range check performed. Use of out of range
     * indices is undefined.
     */
    const KeyParameter& operator[](int n) const;

    /**
     * Returns true if the set contains at least one instance of \p tag
     */
    bool Contains(Tag tag) const {
        return find(tag) != -1;
    }

    template <TagType tag_type, Tag tag, typename ValueT>
    bool Contains(TypedTag<tag_type, tag> ttag, const ValueT& value) const {
        for (const auto& param: data_) {
            auto entry = authorizationValue(ttag, param);
            if (entry.isOk() && entry.value() == value) return true;
        }
        return false;
    }
    /**
     * Returns the number of \p tag entries.
     */
    size_t GetTagCount(Tag tag) const;

    template <typename T>
    inline NullOr<const typename TypedTag2ValueType<T>::type&> GetTagValue(T tag) const {
        auto entry = GetEntry(tag);
        if (entry.isOk()) return authorizationValue(tag, entry.value());
        return {};
    }

    void push_back(const KeyParameter& param) {
        data_.push_back(param);
    }
    void push_back(KeyParameter&& param) {
        data_.push_back(std::move(param));
    }

    /**
     * Append the tag and enumerated value to the set.
     * "val" may be exactly one parameter unless a boolean parameter is added.
     * In this case "val" is omitted. This condition is checked at compile time by Authorization()
     */
    template <typename TypedTagT, typename... Value>
    void push_back(TypedTagT tag, Value&&... val) {
        push_back(Authorization(tag, std::forward<Value>(val)...));
    }

    template <typename Iterator>
    void append(Iterator begin, Iterator end) {
        while (begin != end) {
            push_back(*begin);
            ++begin;
        }
    }

    hidl_vec<KeyParameter> hidl_data() const {
        hidl_vec<KeyParameter> result;
        result.setToExternal(const_cast<KeyParameter*>(data()), size());
        return result;
    }

    void Serialize(std::ostream* out) const;
    void Deserialize(std::istream* in);

  private:
    NullOr<const KeyParameter&> GetEntry(Tag tag) const;

    std::vector<KeyParameter> data_;
};

class AuthorizationSetBuilder: public AuthorizationSet {
  public:
    template <typename TagType, typename... ValueType>
    AuthorizationSetBuilder& Authorization(TagType ttag, ValueType&&... value) {
        push_back(ttag, std::forward<ValueType>(value)...);
        return *this;
    }

    template <Tag tag>
    AuthorizationSetBuilder& Authorization(TypedTag<TagType::BYTES, tag> ttag, const uint8_t* data,
                                           size_t data_length) {
        hidl_vec<uint8_t> new_blob;
        new_blob.setToExternal(const_cast<uint8_t*>(data), data_length);
        push_back(ttag, std::move(new_blob));
        return *this;
    }

    template <Tag tag>
    AuthorizationSetBuilder& Authorization(TypedTag<TagType::BYTES, tag> ttag, const char* data,
                                           size_t data_length) {
        return Authorization(ttag, reinterpret_cast<const uint8_t*>(data), data_length);
    }

    AuthorizationSetBuilder& RsaKey(uint32_t key_size, uint64_t public_exponent);
    AuthorizationSetBuilder& EcdsaKey(uint32_t key_size);
    AuthorizationSetBuilder& AesKey(uint32_t key_size);
    AuthorizationSetBuilder& HmacKey(uint32_t key_size);

    AuthorizationSetBuilder& RsaSigningKey(uint32_t key_size, uint64_t public_exponent);
    AuthorizationSetBuilder& RsaEncryptionKey(uint32_t key_size, uint64_t public_exponent);
    AuthorizationSetBuilder& EcdsaSigningKey(uint32_t key_size);
    AuthorizationSetBuilder& AesEncryptionKey(uint32_t key_size);

    AuthorizationSetBuilder& SigningKey();
    AuthorizationSetBuilder& EncryptionKey();
    AuthorizationSetBuilder& NoDigestOrPadding();
    AuthorizationSetBuilder& EcbMode();

    AuthorizationSetBuilder& Digest(Digest digest) {
        return Authorization(TAG_DIGEST, digest);
    }

    AuthorizationSetBuilder& Padding(PaddingMode padding) {
        return Authorization(TAG_PADDING, padding);
    }
};

inline AuthorizationSetBuilder& AuthorizationSetBuilder::RsaKey(uint32_t key_size,
                                                                uint64_t public_exponent) {
    Authorization(TAG_ALGORITHM, Algorithm::RSA);
    Authorization(TAG_KEY_SIZE, key_size);
    Authorization(TAG_RSA_PUBLIC_EXPONENT, public_exponent);
    return *this;
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::EcdsaKey(uint32_t key_size) {
    Authorization(TAG_ALGORITHM, Algorithm::EC);
    Authorization(TAG_KEY_SIZE, key_size);
    return *this;
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::AesKey(uint32_t key_size) {
    Authorization(TAG_ALGORITHM, Algorithm::AES);
    return Authorization(TAG_KEY_SIZE, key_size);
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::HmacKey(uint32_t key_size) {
    Authorization(TAG_ALGORITHM, Algorithm::HMAC);
    Authorization(TAG_KEY_SIZE, key_size);
    return SigningKey();
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::RsaSigningKey(uint32_t key_size,
                                                                       uint64_t public_exponent) {
    RsaKey(key_size, public_exponent);
    return SigningKey();
}

inline AuthorizationSetBuilder&
AuthorizationSetBuilder::RsaEncryptionKey(uint32_t key_size, uint64_t public_exponent) {
    RsaKey(key_size, public_exponent);
    return EncryptionKey();
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::EcdsaSigningKey(uint32_t key_size) {
    EcdsaKey(key_size);
    return SigningKey();
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::AesEncryptionKey(uint32_t key_size) {
    AesKey(key_size);
    return EncryptionKey();
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::SigningKey() {
    Authorization(TAG_PURPOSE, KeyPurpose::SIGN);
    return Authorization(TAG_PURPOSE, KeyPurpose::VERIFY);
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::EncryptionKey() {
    Authorization(TAG_PURPOSE, KeyPurpose::ENCRYPT);
    return Authorization(TAG_PURPOSE, KeyPurpose::DECRYPT);
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::NoDigestOrPadding() {
    Authorization(TAG_DIGEST, Digest::NONE);
    return Authorization(TAG_PADDING, PaddingMode::NONE);
}

inline AuthorizationSetBuilder& AuthorizationSetBuilder::EcbMode() {
    return Authorization(TAG_BLOCK_MODE, BlockMode::ECB);
}

}  // namespace ng
}  // namespace keymaster

#endif  // SYSTEM_SECURITY_KEYSTORE_AUTHORIZATION_SET_H_
