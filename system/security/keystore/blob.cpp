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

#define LOG_TAG "keystore"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <log/log.h>

#include "blob.h"

#include "keystore_utils.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <istream>
#include <ostream>
#include <streambuf>
#include <string>

#include <android-base/logging.h>

namespace {

constexpr size_t kGcmIvSizeBytes = 96 / 8;

template <typename T, void (*FreeFunc)(T*)> struct OpenSslObjectDeleter {
    void operator()(T* p) { FreeFunc(p); }
};

#define DEFINE_OPENSSL_OBJECT_POINTER(name)                                                        \
    typedef OpenSslObjectDeleter<name, name##_free> name##_Delete;                                 \
    typedef std::unique_ptr<name, name##_Delete> name##_Ptr;

DEFINE_OPENSSL_OBJECT_POINTER(EVP_CIPHER_CTX);

#if defined(__clang__)
#define OPTNONE __attribute__((optnone))
#elif defined(__GNUC__)
#define OPTNONE __attribute__((optimize("O0")))
#else
#error Need a definition for OPTNONE
#endif

class ArrayEraser {
  public:
    ArrayEraser(uint8_t* arr, size_t size) : mArr(arr), mSize(size) {}
    OPTNONE ~ArrayEraser() { std::fill(mArr, mArr + mSize, 0); }

  private:
    volatile uint8_t* mArr;
    size_t mSize;
};

/**
 * Returns a EVP_CIPHER appropriate for the given key, based on the key's size.
 */
const EVP_CIPHER* getAesCipherForKey(const std::vector<uint8_t>& key) {
    const EVP_CIPHER* cipher = EVP_aes_256_gcm();
    if (key.size() == kAes128KeySizeBytes) {
        cipher = EVP_aes_128_gcm();
    }
    return cipher;
}

/*
 * Encrypt 'len' data at 'in' with AES-GCM, using 128-bit or 256-bit key at 'key', 96-bit IV at
 * 'iv' and write output to 'out' (which may be the same location as 'in') and 128-bit tag to
 * 'tag'.
 */
ResponseCode AES_gcm_encrypt(const uint8_t* in, uint8_t* out, size_t len,
                             const std::vector<uint8_t>& key, const uint8_t* iv, uint8_t* tag) {

    // There can be 128-bit and 256-bit keys
    const EVP_CIPHER* cipher = getAesCipherForKey(key);

    EVP_CIPHER_CTX_Ptr ctx(EVP_CIPHER_CTX_new());

    EVP_EncryptInit_ex(ctx.get(), cipher, nullptr /* engine */, key.data(), iv);
    EVP_CIPHER_CTX_set_padding(ctx.get(), 0 /* no padding needed with GCM */);

    std::unique_ptr<uint8_t[]> out_tmp(new uint8_t[len]);
    uint8_t* out_pos = out_tmp.get();
    int out_len;

    EVP_EncryptUpdate(ctx.get(), out_pos, &out_len, in, len);
    out_pos += out_len;
    EVP_EncryptFinal_ex(ctx.get(), out_pos, &out_len);
    out_pos += out_len;
    if (out_pos - out_tmp.get() != static_cast<ssize_t>(len)) {
        ALOGD("Encrypted ciphertext is the wrong size, expected %zu, got %zd", len,
              out_pos - out_tmp.get());
        return ResponseCode::SYSTEM_ERROR;
    }

    std::copy(out_tmp.get(), out_pos, out);
    EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, kGcmTagLength, tag);

    return ResponseCode::NO_ERROR;
}

/*
 * Decrypt 'len' data at 'in' with AES-GCM, using 128-bit or 256-bit key at 'key', 96-bit IV at
 * 'iv', checking 128-bit tag at 'tag' and writing plaintext to 'out'(which may be the same
 * location as 'in').
 */
ResponseCode AES_gcm_decrypt(const uint8_t* in, uint8_t* out, size_t len,
                             const std::vector<uint8_t> key, const uint8_t* iv,
                             const uint8_t* tag) {

    // There can be 128-bit and 256-bit keys
    const EVP_CIPHER* cipher = getAesCipherForKey(key);

    EVP_CIPHER_CTX_Ptr ctx(EVP_CIPHER_CTX_new());

    EVP_DecryptInit_ex(ctx.get(), cipher, nullptr /* engine */, key.data(), iv);
    EVP_CIPHER_CTX_set_padding(ctx.get(), 0 /* no padding needed with GCM */);
    EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, kGcmTagLength, const_cast<uint8_t*>(tag));

    std::unique_ptr<uint8_t[]> out_tmp(new uint8_t[len]);
    ArrayEraser out_eraser(out_tmp.get(), len);
    uint8_t* out_pos = out_tmp.get();
    int out_len;

    EVP_DecryptUpdate(ctx.get(), out_pos, &out_len, in, len);
    out_pos += out_len;
    if (!EVP_DecryptFinal_ex(ctx.get(), out_pos, &out_len)) {
        ALOGD("Failed to decrypt blob; ciphertext or tag is likely corrupted");
        return ResponseCode::VALUE_CORRUPTED;
    }
    out_pos += out_len;
    if (out_pos - out_tmp.get() != static_cast<ssize_t>(len)) {
        ALOGD("Encrypted plaintext is the wrong size, expected %zu, got %zd", len,
              out_pos - out_tmp.get());
        return ResponseCode::VALUE_CORRUPTED;
    }

    std::copy(out_tmp.get(), out_pos, out);

    return ResponseCode::NO_ERROR;
}

class ArrayStreamBuffer : public std::streambuf {
  public:
    template <typename T, size_t size> explicit ArrayStreamBuffer(const T (&data)[size]) {
        static_assert(sizeof(T) == 1, "Array element size too large");
        std::streambuf::char_type* d = const_cast<std::streambuf::char_type*>(
            reinterpret_cast<const std::streambuf::char_type*>(&data[0]));
        setg(d, d, d + size);
        setp(d, d + size);
    }

  protected:
    pos_type seekoff(off_type off, std::ios_base::seekdir dir,
                     std::ios_base::openmode which = std::ios_base::in |
                                                     std::ios_base::out) override {
        bool in = which & std::ios_base::in;
        bool out = which & std::ios_base::out;
        if ((!in && !out) || (in && out && dir == std::ios_base::cur)) return -1;
        std::streambuf::char_type* newPosPtr;
        switch (dir) {
        case std::ios_base::beg:
            newPosPtr = pbase();
            break;
        case std::ios_base::cur:
            // if dir == cur then in xor out due to
            // if ((!in && !out) || (in && out && dir == std::ios_base::cur)) return -1; above
            if (in)
                newPosPtr = gptr();
            else
                newPosPtr = pptr();
            break;
        case std::ios_base::end:
            // in and out bounds are the same and cannot change, so we can take either range
            // regardless of the value of "which"
            newPosPtr = epptr();
            break;
        }
        newPosPtr += off;
        if (newPosPtr < pbase() || newPosPtr > epptr()) return -1;
        if (in) {
            gbump(newPosPtr - gptr());
        }
        if (out) {
            pbump(newPosPtr - pptr());
        }
        return newPosPtr - pbase();
    }
};

}  // namespace

Blob::Blob(const uint8_t* value, size_t valueLength, const uint8_t* info, uint8_t infoLength,
           BlobType type) {
    mBlob = std::make_unique<blobv3>();
    memset(mBlob.get(), 0, sizeof(blobv3));
    if (valueLength > kValueSize) {
        valueLength = kValueSize;
        ALOGW("Provided blob length too large");
    }
    if (infoLength + valueLength > kValueSize) {
        infoLength = kValueSize - valueLength;
        ALOGW("Provided info length too large");
    }
    mBlob->length = valueLength;
    memcpy(mBlob->value, value, valueLength);

    mBlob->info = infoLength;
    memcpy(mBlob->value + valueLength, info, infoLength);

    mBlob->version = CURRENT_BLOB_VERSION;
    mBlob->type = uint8_t(type);

    if (type == TYPE_MASTER_KEY || type == TYPE_MASTER_KEY_AES256) {
        mBlob->flags = KEYSTORE_FLAG_ENCRYPTED;
    } else {
        mBlob->flags = KEYSTORE_FLAG_NONE;
    }
}

Blob::Blob(blobv3 b) {
    mBlob = std::make_unique<blobv3>(b);
}

Blob::Blob() {
    if (mBlob) *mBlob = {};
}

Blob::Blob(const Blob& rhs) {
    if (rhs.mBlob) {
        mBlob = std::make_unique<blobv3>(*rhs.mBlob);
    }
}

Blob::Blob(Blob&& rhs) : mBlob(std::move(rhs.mBlob)) {}

Blob& Blob::operator=(const Blob& rhs) {
    if (&rhs != this) {
        if (mBlob) *mBlob = {};
        if (rhs) {
            mBlob = std::make_unique<blobv3>(*rhs.mBlob);
        } else {
            mBlob = {};
        }
    }
    return *this;
}

Blob& Blob::operator=(Blob&& rhs) {
    if (mBlob) *mBlob = {};
    mBlob = std::move(rhs.mBlob);
    return *this;
}

template <typename BlobType> static bool rawBlobIsEncrypted(const BlobType& blob) {
    if (blob.version < 2) return true;

    return blob.flags & (KEYSTORE_FLAG_ENCRYPTED | KEYSTORE_FLAG_SUPER_ENCRYPTED);
}

bool Blob::isEncrypted() const {
    if (mBlob->version < 2) {
        return true;
    }

    return mBlob->flags & KEYSTORE_FLAG_ENCRYPTED;
}

bool Blob::isSuperEncrypted() const {
    return mBlob->flags & KEYSTORE_FLAG_SUPER_ENCRYPTED;
}

bool Blob::isCriticalToDeviceEncryption() const {
    return mBlob->flags & KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION;
}

inline uint8_t setFlag(uint8_t flags, bool set, KeyStoreFlag flag) {
    return set ? (flags | flag) : (flags & ~flag);
}

void Blob::setEncrypted(bool encrypted) {
    mBlob->flags = setFlag(mBlob->flags, encrypted, KEYSTORE_FLAG_ENCRYPTED);
}

void Blob::setSuperEncrypted(bool superEncrypted) {
    mBlob->flags = setFlag(mBlob->flags, superEncrypted, KEYSTORE_FLAG_SUPER_ENCRYPTED);
}

void Blob::setCriticalToDeviceEncryption(bool critical) {
    mBlob->flags = setFlag(mBlob->flags, critical, KEYSTORE_FLAG_CRITICAL_TO_DEVICE_ENCRYPTION);
}

void Blob::setFallback(bool fallback) {
    if (fallback) {
        mBlob->flags |= KEYSTORE_FLAG_FALLBACK;
    } else {
        mBlob->flags &= ~KEYSTORE_FLAG_FALLBACK;
    }
}

static ResponseCode writeBlob(const std::string& filename, Blob blob, blobv3* rawBlob,
                              const std::vector<uint8_t>& aes_key, State state) {
    ALOGV("writing blob %s", filename.c_str());

    const size_t dataLength = rawBlob->length;
    rawBlob->length = htonl(rawBlob->length);

    if (blob.isEncrypted() || blob.isSuperEncrypted()) {
        if (state != STATE_NO_ERROR) {
            ALOGD("couldn't insert encrypted blob while not unlocked");
            return ResponseCode::LOCKED;
        }

        memset(rawBlob->initialization_vector, 0, AES_BLOCK_SIZE);
        if (!RAND_bytes(rawBlob->initialization_vector, kGcmIvSizeBytes)) {
            ALOGW("Could not read random data for: %s", filename.c_str());
            return ResponseCode::SYSTEM_ERROR;
        }

        auto rc = AES_gcm_encrypt(rawBlob->value /* in */, rawBlob->value /* out */, dataLength,
                                  aes_key, rawBlob->initialization_vector, rawBlob->aead_tag);
        if (rc != ResponseCode::NO_ERROR) return rc;
    }

    size_t fileLength = offsetof(blobv3, value) + dataLength + rawBlob->info;

    int out =
        TEMP_FAILURE_RETRY(open(filename.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR));
    if (out < 0) {
        ALOGW("could not open file: %s: %s", filename.c_str(), strerror(errno));
        return ResponseCode::SYSTEM_ERROR;
    }

    const size_t writtenBytes = writeFully(out, reinterpret_cast<uint8_t*>(rawBlob), fileLength);
    if (close(out) != 0) {
        return ResponseCode::SYSTEM_ERROR;
    }
    if (writtenBytes != fileLength) {
        ALOGW("blob not fully written %zu != %zu", writtenBytes, fileLength);
        unlink(filename.c_str());
        return ResponseCode::SYSTEM_ERROR;
    }
    return ResponseCode::NO_ERROR;
}

ResponseCode LockedKeyBlobEntry::writeBlobs(Blob keyBlob, Blob characteristicsBlob,
                                            const std::vector<uint8_t>& aes_key,
                                            State state) const {
    if (entry_ == nullptr) {
        return ResponseCode::SYSTEM_ERROR;
    }
    ResponseCode rc;
    if (keyBlob) {
        blobv3* rawBlob = keyBlob.mBlob.get();
        rc = writeBlob(entry_->getKeyBlobPath(), std::move(keyBlob), rawBlob, aes_key, state);
        if (rc != ResponseCode::NO_ERROR) {
            return rc;
        }
    }

    if (characteristicsBlob) {
        blobv3* rawBlob = characteristicsBlob.mBlob.get();
        rc = writeBlob(entry_->getCharacteristicsBlobPath(), std::move(characteristicsBlob),
                       rawBlob, aes_key, state);
    }
    return rc;
}

ResponseCode Blob::readBlob(const std::string& filename, const std::vector<uint8_t>& aes_key,
                            State state) {
    ResponseCode rc;
    ALOGV("reading blob %s", filename.c_str());
    std::unique_ptr<blobv3> rawBlob = std::make_unique<blobv3>();

    const int in = TEMP_FAILURE_RETRY(open(filename.c_str(), O_RDONLY));
    if (in < 0) {
        return (errno == ENOENT) ? ResponseCode::KEY_NOT_FOUND : ResponseCode::SYSTEM_ERROR;
    }

    // fileLength may be less than sizeof(mBlob)
    const size_t fileLength = readFully(in, (uint8_t*)rawBlob.get(), sizeof(blobv3));
    if (close(in) != 0) {
        return ResponseCode::SYSTEM_ERROR;
    }

    if (fileLength == 0) {
        return ResponseCode::VALUE_CORRUPTED;
    }

    if (rawBlobIsEncrypted(*rawBlob)) {
        if (state == STATE_LOCKED) {
            mBlob = std::move(rawBlob);
            return ResponseCode::LOCKED;
        }
        if (state == STATE_UNINITIALIZED) return ResponseCode::UNINITIALIZED;
    }

    if (fileLength < offsetof(blobv3, value)) return ResponseCode::VALUE_CORRUPTED;

    if (rawBlob->version == 3) {
        const ssize_t encryptedLength = ntohl(rawBlob->length);

        if (rawBlobIsEncrypted(*rawBlob)) {
            rc = AES_gcm_decrypt(rawBlob->value /* in */, rawBlob->value /* out */, encryptedLength,
                                 aes_key, rawBlob->initialization_vector, rawBlob->aead_tag);
            if (rc != ResponseCode::NO_ERROR) {
                // If the blob was superencrypted and decryption failed, it is
                // almost certain that decryption is failing due to a user's
                // changed master key.
                if ((rawBlob->flags & KEYSTORE_FLAG_SUPER_ENCRYPTED) &&
                    (rc == ResponseCode::VALUE_CORRUPTED)) {
                    return ResponseCode::KEY_PERMANENTLY_INVALIDATED;
                }
                return rc;
            }
        }
    } else if (rawBlob->version < 3) {
        blobv2& v2blob = reinterpret_cast<blobv2&>(*rawBlob);
        const size_t headerLength = offsetof(blobv2, encrypted);
        const ssize_t encryptedLength = fileLength - headerLength - v2blob.info;
        if (encryptedLength < 0) return ResponseCode::VALUE_CORRUPTED;

        if (rawBlobIsEncrypted(*rawBlob)) {
            if (encryptedLength % AES_BLOCK_SIZE != 0) {
                return ResponseCode::VALUE_CORRUPTED;
            }

            AES_KEY key;
            AES_set_decrypt_key(aes_key.data(), kAesKeySize * 8, &key);
            AES_cbc_encrypt(v2blob.encrypted, v2blob.encrypted, encryptedLength, &key,
                            v2blob.vector, AES_DECRYPT);
            key = {};  // clear key

            uint8_t computedDigest[MD5_DIGEST_LENGTH];
            ssize_t digestedLength = encryptedLength - MD5_DIGEST_LENGTH;
            MD5(v2blob.digested, digestedLength, computedDigest);
            if (memcmp(v2blob.digest, computedDigest, MD5_DIGEST_LENGTH) != 0) {
                return ResponseCode::VALUE_CORRUPTED;
            }
        }
    }

    const ssize_t maxValueLength = fileLength - offsetof(blobv3, value) - rawBlob->info;
    rawBlob->length = ntohl(rawBlob->length);
    if (rawBlob->length < 0 || rawBlob->length > maxValueLength ||
        rawBlob->length + rawBlob->info + AES_BLOCK_SIZE >
            static_cast<ssize_t>(sizeof(rawBlob->value))) {
        return ResponseCode::VALUE_CORRUPTED;
    }

    if (rawBlob->info != 0 && rawBlob->version < 3) {
        // move info from after padding to after data
        memmove(rawBlob->value + rawBlob->length, rawBlob->value + maxValueLength, rawBlob->info);
    }

    mBlob = std::move(rawBlob);
    return ResponseCode::NO_ERROR;
}

std::tuple<ResponseCode, Blob, Blob>
LockedKeyBlobEntry::readBlobs(const std::vector<uint8_t>& aes_key, State state) const {
    std::tuple<ResponseCode, Blob, Blob> result;
    auto& [rc, keyBlob, characteristicsBlob] = result;
    if (entry_ == nullptr) return rc = ResponseCode::SYSTEM_ERROR, result;

    rc = keyBlob.readBlob(entry_->getKeyBlobPath(), aes_key, state);
    if (rc != ResponseCode::NO_ERROR && rc != ResponseCode::UNINITIALIZED) {
        return result;
    }

    if (entry_->hasCharacteristicsBlob()) {
        characteristicsBlob.readBlob(entry_->getCharacteristicsBlobPath(), aes_key, state);
    }
    return result;
}

ResponseCode LockedKeyBlobEntry::deleteBlobs() const {
    if (entry_ == nullptr) return ResponseCode::NO_ERROR;

    // always try to delete both
    ResponseCode rc1 = (unlink(entry_->getKeyBlobPath().c_str()) && errno != ENOENT)
                           ? ResponseCode::SYSTEM_ERROR
                           : ResponseCode::NO_ERROR;
    if (rc1 != ResponseCode::NO_ERROR) {
        ALOGW("Failed to delete key blob file \"%s\"", entry_->getKeyBlobPath().c_str());
    }
    ResponseCode rc2 = (unlink(entry_->getCharacteristicsBlobPath().c_str()) && errno != ENOENT)
                           ? ResponseCode::SYSTEM_ERROR
                           : ResponseCode::NO_ERROR;
    if (rc2 != ResponseCode::NO_ERROR) {
        ALOGW("Failed to delete key characteristics file \"%s\"",
              entry_->getCharacteristicsBlobPath().c_str());
    }
    // then report the first error that occured
    if (rc1 != ResponseCode::NO_ERROR) return rc1;
    return rc2;
}

keystore::SecurityLevel Blob::getSecurityLevel() const {
    return keystore::flagsToSecurityLevel(mBlob->flags);
}

void Blob::setSecurityLevel(keystore::SecurityLevel secLevel) {
    mBlob->flags &= ~(KEYSTORE_FLAG_FALLBACK | KEYSTORE_FLAG_STRONGBOX);
    mBlob->flags |= keystore::securityLevelToFlags(secLevel);
}

std::tuple<bool, keystore::AuthorizationSet, keystore::AuthorizationSet>
Blob::getKeyCharacteristics() const {
    std::tuple<bool, keystore::AuthorizationSet, keystore::AuthorizationSet> result;
    auto& [success, hwEnforced, swEnforced] = result;
    success = false;
    ArrayStreamBuffer buf(mBlob->value);
    std::istream in(&buf);

    // only the characteristics cache has both sets
    if (getType() == TYPE_KEY_CHARACTERISTICS_CACHE) {
        hwEnforced.Deserialize(&in);
    } else if (getType() != TYPE_KEY_CHARACTERISTICS) {
        // if its not the cache and not the legacy characteristics file we have no business
        // here
        return result;
    }
    swEnforced.Deserialize(&in);
    success = !in.bad();

    return result;
}
bool Blob::putKeyCharacteristics(const keystore::AuthorizationSet& hwEnforced,
                                 const keystore::AuthorizationSet& swEnforced) {
    if (!mBlob) mBlob = std::make_unique<blobv3>();
    mBlob->version = CURRENT_BLOB_VERSION;
    ArrayStreamBuffer buf(mBlob->value);
    std::ostream out(&buf);
    hwEnforced.Serialize(&out);
    swEnforced.Serialize(&out);
    if (out.bad()) return false;
    setType(TYPE_KEY_CHARACTERISTICS_CACHE);
    mBlob->length = out.tellp();
    return true;
}

void LockedKeyBlobEntry::put(const KeyBlobEntry& entry) {
    std::unique_lock<std::mutex> lock(locked_blobs_mutex_);
    locked_blobs_.erase(entry);
    lock.unlock();
    locked_blobs_mutex_cond_var_.notify_all();
}

LockedKeyBlobEntry::~LockedKeyBlobEntry() {
    if (entry_ != nullptr) put(*entry_);
}

LockedKeyBlobEntry LockedKeyBlobEntry::get(KeyBlobEntry entry) {
    std::unique_lock<std::mutex> lock(locked_blobs_mutex_);
    locked_blobs_mutex_cond_var_.wait(
        lock, [&] { return locked_blobs_.find(entry) == locked_blobs_.end(); });
    auto [iterator, success] = locked_blobs_.insert(std::move(entry));
    if (!success) return {};
    return LockedKeyBlobEntry(*iterator);
}

std::set<KeyBlobEntry> LockedKeyBlobEntry::locked_blobs_;
std::mutex LockedKeyBlobEntry::locked_blobs_mutex_;
std::condition_variable LockedKeyBlobEntry::locked_blobs_mutex_cond_var_;

/* Here is the encoding of key names. This is necessary in order to allow arbitrary
 * characters in key names. Characters in [0-~] are not encoded. Others are encoded
 * into two bytes. The first byte is one of [+-.] which represents the first
 * two bits of the character. The second byte encodes the rest of the bits into
 * [0-o]. Therefore in the worst case the length of a key gets doubled. Note
 * that Base64 cannot be used here due to the need of prefix match on keys. */

std::string encodeKeyName(const std::string& keyName) {
    std::string encodedName;
    encodedName.reserve(keyName.size() * 2);
    auto in = keyName.begin();
    while (in != keyName.end()) {
        // Input character needs to be encoded.
        if (*in < '0' || *in > '~') {
            // Encode the two most-significant bits of the input char in the first
            // output character, by counting up from 43 ('+').
            encodedName.append(1, '+' + (uint8_t(*in) >> 6));
            // Encode the six least-significant bits of the input char in the second
            // output character, by counting up from 48 ('0').
            // This is safe because the maximum value is 112, which is the
            // character 'p'.
            encodedName.append(1, '0' + (*in & 0x3F));
        } else {
            // No need to encode input char - append as-is.
            encodedName.append(1, *in);
        }
        ++in;
    }
    return encodedName;
}

std::string decodeKeyName(const std::string& encodedName) {
    std::string decodedName;
    decodedName.reserve(encodedName.size());
    auto in = encodedName.begin();
    bool multichar = false;
    char c;
    while (in != encodedName.end()) {
        if (multichar) {
            // Second part of a multi-character encoding. Turn off the multichar
            // flag and set the six least-significant bits of c to the value originally
            // encoded by counting up from '0'.
            multichar = false;
            decodedName.append(1, c | (uint8_t(*in) - '0'));
        } else if (*in >= '+' && *in <= '.') {
            // First part of a multi-character encoding. Set the multichar flag
            // and set the two most-significant bits of c to be the two bits originally
            // encoded by counting up from '+'.
            multichar = true;
            c = (*in - '+') << 6;
        } else {
            // Regular character, append as-is.
            decodedName.append(1, *in);
        }
        ++in;
    }
    // mulitchars at the end get truncated
    return decodedName;
}

std::string KeyBlobEntry::getKeyBlobBaseName() const {
    std::stringstream s;
    if (masterkey_) {
        s << alias_;
    } else {
        s << uid_ << "_" << encodeKeyName(alias_);
    }
    return s.str();
}

std::string KeyBlobEntry::getKeyBlobPath() const {
    std::stringstream s;
    if (masterkey_) {
        s << user_dir_ << "/" << alias_;
    } else {
        s << user_dir_ << "/" << uid_ << "_" << encodeKeyName(alias_);
    }
    return s.str();
}

std::string KeyBlobEntry::getCharacteristicsBlobBaseName() const {
    std::stringstream s;
    if (!masterkey_) s << "." << uid_ << "_chr_" << encodeKeyName(alias_);
    return s.str();
}

std::string KeyBlobEntry::getCharacteristicsBlobPath() const {
    std::stringstream s;
    if (!masterkey_)
        s << user_dir_ << "/"
          << "." << uid_ << "_chr_" << encodeKeyName(alias_);
    return s.str();
}

bool KeyBlobEntry::hasKeyBlob() const {
    int trys = 3;
    while (trys--) {
        if (!access(getKeyBlobPath().c_str(), R_OK | W_OK)) return true;
        if (errno == ENOENT) return false;
        LOG(WARNING) << "access encountered " << strerror(errno) << " (" << errno << ")"
                     << " while checking for key blob";
        if (errno != EAGAIN) break;
    }
    return false;
}

bool KeyBlobEntry::hasCharacteristicsBlob() const {
    int trys = 3;
    while (trys--) {
        if (!access(getCharacteristicsBlobPath().c_str(), R_OK | W_OK)) return true;
        if (errno == ENOENT) return false;
        LOG(WARNING) << "access encountered " << strerror(errno) << " (" << errno << ")"
                     << " while checking for key characteristics blob";
        if (errno != EAGAIN) break;
    }
    return false;
}

static std::tuple<bool, uid_t, std::string> filename2UidAlias(const std::string& filepath) {
    std::tuple<bool, uid_t, std::string> result;

    auto& [success, uid, alias] = result;

    success = false;

    auto filenamebase = filepath.find_last_of('/');
    std::string filename =
        filenamebase == std::string::npos ? filepath : filepath.substr(filenamebase + 1);

    if (filename[0] == '.') return result;

    auto sep = filename.find('_');
    if (sep == std::string::npos) return result;

    std::stringstream s(filename.substr(0, sep));
    s >> uid;
    if (!s) return result;

    alias = decodeKeyName(filename.substr(sep + 1));
    success = true;
    return result;
}

std::tuple<ResponseCode, std::list<LockedKeyBlobEntry>>
LockedKeyBlobEntry::list(const std::string& user_dir,
                         std::function<bool(uid_t, const std::string&)> filter) {
    std::list<LockedKeyBlobEntry> matches;

    // This is a fence against any concurrent database accesses during database iteration.
    // Only the keystore thread can lock entries. So it cannot be starved
    // by workers grabbing new individual locks. We just wait here until all
    // workers have relinquished their locked files.
    std::unique_lock<std::mutex> lock(locked_blobs_mutex_);
    locked_blobs_mutex_cond_var_.wait(lock, [&] { return locked_blobs_.empty(); });

    DIR* dir = opendir(user_dir.c_str());
    if (!dir) {
        ALOGW("can't open directory for user: %s", strerror(errno));
        return std::tuple<ResponseCode, std::list<LockedKeyBlobEntry>&&>{ResponseCode::SYSTEM_ERROR,
                                                                         std::move(matches)};
    }

    struct dirent* file;
    while ((file = readdir(dir)) != nullptr) {
        // We only care about files.
        if (file->d_type != DT_REG) {
            continue;
        }

        // Skip anything that starts with a "."
        if (file->d_name[0] == '.') {
            continue;
        }

        auto [success, uid, alias] = filename2UidAlias(file->d_name);

        if (!success) {
            ALOGW("could not parse key filename \"%s\"", file->d_name);
            continue;
        }

        if (!filter(uid, alias)) continue;

        auto [iterator, dummy] = locked_blobs_.emplace(alias, user_dir, uid);
        matches.push_back(*iterator);
    }
    closedir(dir);
    return std::tuple<ResponseCode, std::list<LockedKeyBlobEntry>&&>{ResponseCode::NO_ERROR,
                                                                     std::move(matches)};
}
