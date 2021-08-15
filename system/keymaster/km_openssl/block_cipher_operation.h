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

#ifndef SYSTEM_KEYMASTER_BLOCK_CIPHER_OPERATION_H_
#define SYSTEM_KEYMASTER_BLOCK_CIPHER_OPERATION_H_

#include <openssl/evp.h>

#include <keymaster/operation.h>

namespace keymaster {

/**
 * EvpCipherDescription is an abstract interface that provides information about a block cipher.
 */
class EvpCipherDescription {
  public:
    virtual ~EvpCipherDescription() {}
    virtual keymaster_algorithm_t algorithm() const = 0;

    virtual const keymaster_block_mode_t* SupportedBlockModes(size_t* block_mode_count) const = 0;

    virtual const EVP_CIPHER* GetCipherInstance(size_t key_size, keymaster_block_mode_t block_mode,
                                                keymaster_error_t* error) const = 0;

    virtual size_t block_size_bytes() const = 0;
};

/**
 * Abstract base for block cipher operation factories.  This class does all of the work to create
 * block cipher operations.
 */
class BlockCipherOperationFactory : public OperationFactory {
  public:
    explicit BlockCipherOperationFactory(keymaster_purpose_t purpose) : purpose_(purpose) {}

    KeyType registry_key() const override {
        return KeyType(GetCipherDescription().algorithm(), purpose_);
    }

    OperationPtr CreateOperation(Key&& key, const AuthorizationSet& begin_params,
                                 keymaster_error_t* error) const override;

    const keymaster_block_mode_t* SupportedBlockModes(size_t* block_mode_count) const override {
        return GetCipherDescription().SupportedBlockModes(block_mode_count);
    }

    const keymaster_padding_t* SupportedPaddingModes(size_t* padding_count) const override;

    virtual const EvpCipherDescription& GetCipherDescription() const = 0;

  private:
    const keymaster_purpose_t purpose_;
};

class BlockCipherEvpOperation : public Operation {
  public:
    BlockCipherEvpOperation(keymaster_purpose_t purpose, keymaster_block_mode_t block_mode,
                            keymaster_padding_t padding, bool caller_iv, size_t tag_length,
                            Key&& key, const EvpCipherDescription& cipher_description);
    ~BlockCipherEvpOperation();

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Update(const AuthorizationSet& additional_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override;
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;
    keymaster_error_t Abort() override;

  protected:
    virtual int evp_encrypt_mode() = 0;

    bool need_iv() const;
    keymaster_error_t InitializeCipher(KeymasterKeyBlob key);
    keymaster_error_t GetIv(const AuthorizationSet& input_params);
    bool HandleAad(const AuthorizationSet& input_params, const Buffer& input,
                   keymaster_error_t* error);
    bool ProcessAadBlocks(const uint8_t* data, size_t blocks, keymaster_error_t* error);
    void FillBufferedAadBlock(keymaster_blob_t* aad);
    bool ProcessBufferedAadBlock(keymaster_error_t* error);
    bool InternalUpdate(const uint8_t* input, size_t input_length, Buffer* output,
                        keymaster_error_t* error);
    bool UpdateForFinish(const AuthorizationSet& additional_params, const Buffer& input,
                         AuthorizationSet* output_params, Buffer* output, keymaster_error_t* error);
    size_t block_size_bytes() const { return cipher_description_.block_size_bytes(); }

    const keymaster_block_mode_t block_mode_;
    EVP_CIPHER_CTX ctx_;
    KeymasterBlob iv_;
    const bool caller_iv_;
    const size_t tag_length_;

  private:
    UniquePtr<uint8_t[]> aad_block_buf_;
    size_t aad_block_buf_len_;
    bool data_started_;
    const keymaster_padding_t padding_;
    KeymasterKeyBlob key_;
    const EvpCipherDescription& cipher_description_;
};

class BlockCipherEvpEncryptOperation : public BlockCipherEvpOperation {
  public:
    BlockCipherEvpEncryptOperation(keymaster_block_mode_t block_mode, keymaster_padding_t padding,
                                   bool caller_iv, size_t tag_length, Key&& key,
                                   const EvpCipherDescription& cipher_description)
        : BlockCipherEvpOperation(KM_PURPOSE_ENCRYPT, block_mode, padding, caller_iv, tag_length,
                                  move(key), cipher_description) {}

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;

    int evp_encrypt_mode() override { return 1; }

  private:
    keymaster_error_t GenerateIv();
};

class BlockCipherEvpDecryptOperation : public BlockCipherEvpOperation {
  public:
    BlockCipherEvpDecryptOperation(keymaster_block_mode_t block_mode, keymaster_padding_t padding,
                                   size_t tag_length, Key&& key,
                                   const EvpCipherDescription& cipher_description)
        : BlockCipherEvpOperation(KM_PURPOSE_DECRYPT, block_mode, padding,
                                  false /* caller_iv -- don't care */, tag_length, move(key),
                                  cipher_description) {}

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Update(const AuthorizationSet& additional_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override;
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;

    int evp_encrypt_mode() override { return 0; }

  private:
    size_t tag_buf_unused() { return tag_length_ - tag_buf_len_; }

    keymaster_error_t ProcessAllButTagLengthBytes(const Buffer& input, Buffer* output);
    bool ProcessTagBufContentsAsData(size_t to_process, Buffer* output, keymaster_error_t* error);
    void BufferCandidateTagData(const uint8_t* data, size_t data_length);

    UniquePtr<uint8_t[]> tag_buf_;
    size_t tag_buf_len_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_BLOCK_CIPHER_OPERATION_H_
