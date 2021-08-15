/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef KEYSTORE_BLOB_H_
#define KEYSTORE_BLOB_H_

#include <stdint.h>

#include <openssl/aes.h>
#include <openssl/md5.h>

#include <condition_variable>
#include <functional>
#include <keystore/keymaster_types.h>
#include <keystore/keystore.h>
#include <list>
#include <mutex>
#include <set>
#include <sstream>
#include <vector>

constexpr size_t kValueSize = 32768;
constexpr size_t kAesKeySize = 128 / 8;
constexpr size_t kGcmTagLength = 128 / 8;
constexpr size_t kGcmIvLength = 96 / 8;
constexpr size_t kAes128KeySizeBytes = 128 / 8;
constexpr size_t kAes256KeySizeBytes = 256 / 8;

/* Here is the file format. There are two parts in blob.value, the secret and
 * the description. The secret is stored in ciphertext, and its original size
 * can be found in blob.length. The description is stored after the secret in
 * plaintext, and its size is specified in blob.info. The total size of the two
 * parts must be no more than kValueSize bytes. The first field is the version,
 * the second is the blob's type, and the third byte is flags. Fields other
 * than blob.info, blob.length, and blob.value are modified by encryptBlob()
 * and decryptBlob(). Thus they should not be accessed from outside. */

struct __attribute__((packed)) blobv3 {
    uint8_t version;
    uint8_t type;
    uint8_t flags;
    uint8_t info;
    uint8_t initialization_vector[AES_BLOCK_SIZE];  // Only 96 bits is used, rest is zeroed.
    uint8_t aead_tag[kGcmTagLength];
    int32_t length;  // in network byte order, only for backward compatibility
    uint8_t value[kValueSize + AES_BLOCK_SIZE];
};

struct __attribute__((packed)) blobv2 {
    uint8_t version;
    uint8_t type;
    uint8_t flags;
    uint8_t info;
    uint8_t vector[AES_BLOCK_SIZE];
    uint8_t encrypted[0];  // Marks offset to encrypted data.
    uint8_t digest[MD5_DIGEST_LENGTH];
    uint8_t digested[0];  // Marks offset to digested data.
    int32_t length;       // in network byte order
    uint8_t value[kValueSize + AES_BLOCK_SIZE];
};

static_assert(sizeof(blobv3) == sizeof(blobv2) &&
                  offsetof(blobv3, initialization_vector) == offsetof(blobv2, vector) &&
                  offsetof(blobv3, aead_tag) == offsetof(blobv2, digest) &&
                  offsetof(blobv3, aead_tag) == offsetof(blobv2, encrypted) &&
                  offsetof(blobv3, length) == offsetof(blobv2, length) &&
                  offsetof(blobv3, value) == offsetof(blobv2, value),
              "Oops.  Blob layout changed.");

static const uint8_t CURRENT_BLOB_VERSION = 3;

typedef enum {
    TYPE_ANY = 0,  // meta type that matches anything
    TYPE_GENERIC = 1,
    TYPE_MASTER_KEY = 2,
    TYPE_KEY_PAIR = 3,
    TYPE_KEYMASTER_10 = 4,
    TYPE_KEY_CHARACTERISTICS = 5,
    TYPE_KEY_CHARACTERISTICS_CACHE = 6,
    TYPE_MASTER_KEY_AES256 = 7,
} BlobType;

class LockedKeyBlobEntry;

/**
 * The Blob represents the content of a KeyBlobEntry.
 *
 * BEWARE: It is only save to call any member function of a Blob b if bool(b) yields true.
 *         Exceptions are putKeyCharacteristics(), the assignment operators and operator bool.
 */
class Blob {
    friend LockedKeyBlobEntry;

  public:
    Blob(const uint8_t* value, size_t valueLength, const uint8_t* info, uint8_t infoLength,
         BlobType type);
    explicit Blob(blobv3 b);
    Blob();
    Blob(const Blob& rhs);
    Blob(Blob&& rhs);

    ~Blob() {
        if (mBlob) *mBlob = {};
    }

    Blob& operator=(const Blob& rhs);
    Blob& operator=(Blob&& rhs);
    explicit operator bool() const { return bool(mBlob); }

    const uint8_t* getValue() const { return mBlob->value; }

    int32_t getLength() const { return mBlob->length; }

    const uint8_t* getInfo() const { return mBlob->value + mBlob->length; }
    uint8_t getInfoLength() const { return mBlob->info; }

    uint8_t getVersion() const { return mBlob->version; }

    bool isEncrypted() const;
    void setEncrypted(bool encrypted);

    bool isSuperEncrypted() const;
    void setSuperEncrypted(bool superEncrypted);

    bool isCriticalToDeviceEncryption() const;
    void setCriticalToDeviceEncryption(bool critical);

    bool isFallback() const { return mBlob->flags & KEYSTORE_FLAG_FALLBACK; }
    void setFallback(bool fallback);

    void setVersion(uint8_t version) { mBlob->version = version; }
    BlobType getType() const { return BlobType(mBlob->type); }
    void setType(BlobType type) { mBlob->type = uint8_t(type); }

    keystore::SecurityLevel getSecurityLevel() const;
    void setSecurityLevel(keystore::SecurityLevel);

    std::tuple<bool, keystore::AuthorizationSet, keystore::AuthorizationSet>
    getKeyCharacteristics() const;

    bool putKeyCharacteristics(const keystore::AuthorizationSet& hwEnforced,
                               const keystore::AuthorizationSet& swEnforced);

  private:
    std::unique_ptr<blobv3> mBlob;

    ResponseCode readBlob(const std::string& filename, const std::vector<uint8_t>& aes_key,
                          State state);
};

/**
 * A KeyBlobEntry represents a full qualified key blob as known by Keystore. The key blob
 * is given by the uid of the owning app and the alias used by the app to refer to this key.
 * The user_dir_ is technically implied by the uid, but computation of the user directory is
 * done in the user state database. Which is why we also cache it here.
 *
 * The KeyBlobEntry knows the location of the key blob files (which may include a characteristics
 * cache file) but does not allow read or write access to the content. It also does not imply
 * the existence of the files.
 *
 * KeyBlobEntry abstracts, to some extent, from the the file system based storage of key blobs.
 * An evolution of KeyBlobEntry may be used for key blob storage based on a back end other than
 * file system, e.g., SQL database or other.
 *
 * For access to the key blob content the programmer has to acquire a LockedKeyBlobEntry (see
 * below).
 */
class KeyBlobEntry {
  private:
    std::string alias_;
    std::string user_dir_;
    uid_t uid_;
    bool masterkey_;

  public:
    KeyBlobEntry(std::string alias, std::string user_dir, uid_t uid, bool masterkey = false)
        : alias_(std::move(alias)), user_dir_(std::move(user_dir)), uid_(uid),
          masterkey_(masterkey) {}

    std::string getKeyBlobBaseName() const;
    std::string getKeyBlobPath() const;

    std::string getCharacteristicsBlobBaseName() const;
    std::string getCharacteristicsBlobPath() const;

    bool hasKeyBlob() const;
    bool hasCharacteristicsBlob() const;

    bool operator<(const KeyBlobEntry& rhs) const {
        return std::tie(uid_, alias_, user_dir_) < std::tie(rhs.uid_, rhs.alias_, rhs.user_dir_);
    }
    bool operator==(const KeyBlobEntry& rhs) const {
        return std::tie(uid_, alias_, user_dir_) == std::tie(rhs.uid_, rhs.alias_, rhs.user_dir_);
    }
    bool operator!=(const KeyBlobEntry& rhs) const { return !(*this == rhs); }

    inline const std::string& alias() const { return alias_; }
    inline const std::string& user_dir() const { return user_dir_; }
    inline uid_t uid() const { return uid_; }
};

/**
 * The LockedKeyBlobEntry is a proxy object to KeyBlobEntry that expresses exclusive ownership
 * of a KeyBlobEntry. LockedKeyBlobEntries can be acquired by calling
 * LockedKeyBlobEntry::get() or LockedKeyBlobEntry::list().
 *
 * LockedKeyBlobEntries are movable but not copyable. By convention they can only
 * be taken by the dispatcher thread of keystore, but not by any keymaster worker thread.
 * The dispatcher thread may transfer ownership of a locked entry to a keymaster worker thread.
 *
 * Locked entries are tracked on the stack or as members of movable functor objects passed to the
 * keymaster worker request queues. Locks are relinquished as the locked entry gets destroyed, e.g.,
 * when it goes out of scope or when the owning request functor gets destroyed.
 *
 * LockedKeyBlobEntry::list(), which must only be called by the dispatcher, blocks until all
 * LockedKeyBlobEntries have been destroyed. Thereby list acts as a fence to make sure it gets a
 * consistent view of the key blob database. Under the assumption that keymaster worker requests
 * cannot run or block indefinitely and cannot grab new locked entries, progress is guaranteed.
 * It then grabs locked entries in accordance with the given filter rule.
 *
 * LockedKeyBlobEntry allow access to the proxied KeyBlobEntry interface through the operator->.
 * They add additional functionality to access and modify the key blob's content on disk.
 * LockedKeyBlobEntry ensures atomic operations on the persistently stored key blobs on a per
 * entry granularity.
 */
class LockedKeyBlobEntry {
  private:
    static std::set<KeyBlobEntry> locked_blobs_;
    static std::mutex locked_blobs_mutex_;
    static std::condition_variable locked_blobs_mutex_cond_var_;

    const KeyBlobEntry* entry_;
    // NOLINTNEXTLINE(google-explicit-constructor)
    LockedKeyBlobEntry(const KeyBlobEntry& entry) : entry_(&entry) {}

    static void put(const KeyBlobEntry& entry);
    LockedKeyBlobEntry(const LockedKeyBlobEntry&) = delete;
    LockedKeyBlobEntry& operator=(const LockedKeyBlobEntry&) = delete;

  public:
    LockedKeyBlobEntry() : entry_(nullptr){};
    ~LockedKeyBlobEntry();
    LockedKeyBlobEntry(LockedKeyBlobEntry&& rhs) : entry_(rhs.entry_) { rhs.entry_ = nullptr; }
    LockedKeyBlobEntry& operator=(LockedKeyBlobEntry&& rhs) {
        // as dummy goes out of scope it relinquishes the lock on this
        LockedKeyBlobEntry dummy(std::move(*this));
        entry_ = rhs.entry_;
        rhs.entry_ = nullptr;
        return *this;
    }
    static LockedKeyBlobEntry get(KeyBlobEntry entry);
    static std::tuple<ResponseCode, std::list<LockedKeyBlobEntry>>
    list(const std::string& user_dir,
         std::function<bool(uid_t, const std::string&)> filter =
             [](uid_t, const std::string&) -> bool { return true; });

    ResponseCode writeBlobs(Blob keyBlob, Blob characteristicsBlob,
                            const std::vector<uint8_t>& aes_key, State state) const;
    std::tuple<ResponseCode, Blob, Blob> readBlobs(const std::vector<uint8_t>& aes_key,
                                                   State state) const;
    ResponseCode deleteBlobs() const;

    inline explicit operator bool() const { return entry_ != nullptr; }
    inline const KeyBlobEntry& operator*() const { return *entry_; }
    inline const KeyBlobEntry* operator->() const { return entry_; }
};

// Visible for testing
std::string encodeKeyName(const std::string& keyName);
std::string decodeKeyName(const std::string& encodedName);

#endif  // KEYSTORE_BLOB_H_
