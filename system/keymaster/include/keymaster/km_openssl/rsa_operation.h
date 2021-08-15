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

#ifndef SYSTEM_KEYMASTER_RSA_OPERATION_H_
#define SYSTEM_KEYMASTER_RSA_OPERATION_H_

#include <keymaster/UniquePtr.h>

#include <openssl/evp.h>
#include <openssl/rsa.h>

#include <keymaster/operation.h>

namespace keymaster {

/**
 * Base class for all RSA operations.
 *
 * This class provides RSA key management, plus buffering of data for non-digesting modes.
 */
class RsaOperation : public Operation {
  public:
    RsaOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                 keymaster_purpose_t purpose, keymaster_digest_t digest,
                 keymaster_padding_t padding, EVP_PKEY* key)
        : Operation(purpose, move(hw_enforced), move(sw_enforced)), rsa_key_(key),
          padding_(padding), digest_(digest), digest_algorithm_(nullptr) {}
    ~RsaOperation();

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Update(const AuthorizationSet& additional_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override;
    keymaster_error_t Abort() override { return KM_ERROR_OK; }

    keymaster_padding_t padding() const { return padding_; }
    keymaster_digest_t digest() const { return digest_; }

  protected:
    virtual int GetOpensslPadding(keymaster_error_t* error) = 0;
    virtual bool require_digest() const = 0;

    keymaster_error_t StoreData(const Buffer& input, size_t* input_consumed);
    keymaster_error_t SetRsaPaddingInEvpContext(EVP_PKEY_CTX* pkey_ctx, bool signing);
    keymaster_error_t InitDigest();

    EVP_PKEY* rsa_key_;
    const keymaster_padding_t padding_;
    Buffer data_;
    const keymaster_digest_t digest_;
    const EVP_MD* digest_algorithm_;
};

/**
 * Base class for all digesting RSA operations.
 *
 * This class adds digesting support, for digesting modes.  For non-digesting modes, it falls back
 * on the RsaOperation input buffering.
 */
class RsaDigestingOperation : public RsaOperation {
  public:
    RsaDigestingOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                          keymaster_purpose_t purpose, keymaster_digest_t digest,
                          keymaster_padding_t padding, EVP_PKEY* key);
    ~RsaDigestingOperation();

  protected:
    int GetOpensslPadding(keymaster_error_t* error) override;
    bool require_digest() const override { return padding_ == KM_PAD_RSA_PSS; }
    EVP_MD_CTX digest_ctx_;
};

/**
 * RSA private key signing operation.
 */
class RsaSignOperation : public RsaDigestingOperation {
  public:
    RsaSignOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                     keymaster_digest_t digest, keymaster_padding_t padding, EVP_PKEY* key)
        : RsaDigestingOperation(move(hw_enforced), move(sw_enforced), KM_PURPOSE_SIGN, digest,
                                padding, key) {}

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Update(const AuthorizationSet& additional_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override;
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;

  private:
    keymaster_error_t SignUndigested(Buffer* output);
    keymaster_error_t SignDigested(Buffer* output);
};

/**
 * RSA public key verification operation.
 */
class RsaVerifyOperation : public RsaDigestingOperation {
  public:
    RsaVerifyOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                       keymaster_digest_t digest, keymaster_padding_t padding, EVP_PKEY* key)
        : RsaDigestingOperation(move(hw_enforced), move(sw_enforced), KM_PURPOSE_VERIFY, digest,
                                padding, key) {}

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override;
    keymaster_error_t Update(const AuthorizationSet& additional_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override;
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;

  private:
    keymaster_error_t VerifyUndigested(const Buffer& signature);
    keymaster_error_t VerifyDigested(const Buffer& signature);
};

/**
 * Base class for RSA crypting operations.
 */
class RsaCryptOperation : public RsaOperation {
  public:
    RsaCryptOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                      keymaster_purpose_t purpose, keymaster_digest_t digest,
                      keymaster_padding_t padding, EVP_PKEY* key)
        : RsaOperation(move(hw_enforced), move(sw_enforced), purpose, digest, padding, key) {}

  protected:
    keymaster_error_t SetOaepDigestIfRequired(EVP_PKEY_CTX* pkey_ctx);

  private:
    int GetOpensslPadding(keymaster_error_t* error) override;
    bool require_digest() const override { return padding_ == KM_PAD_RSA_OAEP; }
};

/**
 * RSA public key encryption operation.
 */
class RsaEncryptOperation : public RsaCryptOperation {
  public:
    RsaEncryptOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                        keymaster_digest_t digest, keymaster_padding_t padding, EVP_PKEY* key)
        : RsaCryptOperation(move(hw_enforced), move(sw_enforced), KM_PURPOSE_ENCRYPT, digest,
                            padding, key) {}
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;
};

/**
 * RSA private key decryption operation.
 */
class RsaDecryptOperation : public RsaCryptOperation {
  public:
    RsaDecryptOperation(AuthorizationSet&& hw_enforced, AuthorizationSet&& sw_enforced,
                        keymaster_digest_t digest, keymaster_padding_t padding, EVP_PKEY* key)
        : RsaCryptOperation(move(hw_enforced), move(sw_enforced), KM_PURPOSE_DECRYPT, digest,
                            padding, key) {}
    keymaster_error_t Finish(const AuthorizationSet& additional_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;
};

/**
 * Abstract base for all RSA operation factories.  This class exists mainly to centralize some code
 * common to all RSA operation factories.
 */
class RsaOperationFactory : public OperationFactory {
  public:
    KeyType registry_key() const override { return KeyType(KM_ALGORITHM_RSA, purpose()); }
    virtual keymaster_purpose_t purpose() const = 0;

    OperationPtr CreateOperation(Key&& key, const AuthorizationSet& begin_params,
                                 keymaster_error_t* error) const override {
        return OperationPtr(CreateRsaOperation(move(key), begin_params, error));
    }
    const keymaster_digest_t* SupportedDigests(size_t* digest_count) const override;

  protected:
    static EVP_PKEY* GetRsaKey(Key&& key, keymaster_error_t* error);
    virtual RsaOperation* CreateRsaOperation(Key&& key, const AuthorizationSet& begin_params,
                                             keymaster_error_t* error) const;

  private:
    virtual RsaOperation* InstantiateOperation(AuthorizationSet&& hw_enforced,
                                               AuthorizationSet&& sw_enforced,
                                               keymaster_digest_t digest,
                                               keymaster_padding_t padding,
                                               EVP_PKEY* key) const = 0;
};

/**
 * Abstract base for RSA operations that digest their input (signing and verification).
 */
class RsaDigestingOperationFactory : public RsaOperationFactory {
  public:
    const keymaster_padding_t* SupportedPaddingModes(size_t* padding_mode_count) const override;
};

/**
 * Abstract base for en/de-crypting RSA operation factories.  This class does most of the work of
 * creating such operations, delegating only the actual operation instantiation.
 */
class RsaCryptingOperationFactory : public RsaOperationFactory {
  public:
    RsaOperation* CreateRsaOperation(Key&& key, const AuthorizationSet& begin_params,
                                     keymaster_error_t* error) const override;
    const keymaster_padding_t* SupportedPaddingModes(size_t* padding_mode_count) const override;
};

/**
 * Concrete factory for RSA signing operations.
 */
class RsaSigningOperationFactory : public RsaDigestingOperationFactory {
  public:
    keymaster_purpose_t purpose() const override { return KM_PURPOSE_SIGN; }
    RsaOperation* InstantiateOperation(AuthorizationSet&& hw_enforced,
                                       AuthorizationSet&& sw_enforced, keymaster_digest_t digest,
                                       keymaster_padding_t padding, EVP_PKEY* key) const override {
        return new (std::nothrow)
            RsaSignOperation(move(hw_enforced), move(sw_enforced), digest, padding, key);
    }
};

/**
 * Concrete factory for RSA signing operations.
 */
class RsaVerificationOperationFactory : public RsaDigestingOperationFactory {
    keymaster_purpose_t purpose() const override { return KM_PURPOSE_VERIFY; }
    RsaOperation* InstantiateOperation(AuthorizationSet&& hw_enforced,
                                       AuthorizationSet&& sw_enforced, keymaster_digest_t digest,
                                       keymaster_padding_t padding, EVP_PKEY* key) const override {
        return new (std::nothrow)
            RsaVerifyOperation(move(hw_enforced), move(sw_enforced), digest, padding, key);
    }
};

/**
 * Concrete factory for RSA signing operations.
 */
class RsaEncryptionOperationFactory : public RsaCryptingOperationFactory {
    keymaster_purpose_t purpose() const override { return KM_PURPOSE_ENCRYPT; }
    RsaOperation* InstantiateOperation(AuthorizationSet&& hw_enforced,
                                       AuthorizationSet&& sw_enforced, keymaster_digest_t digest,
                                       keymaster_padding_t padding, EVP_PKEY* key) const override {
        return new (std::nothrow)
            RsaEncryptOperation(move(hw_enforced), move(sw_enforced), digest, padding, key);
    }
};

/**
 * Concrete factory for RSA signing operations.
 */
class RsaDecryptionOperationFactory : public RsaCryptingOperationFactory {
    keymaster_purpose_t purpose() const override { return KM_PURPOSE_DECRYPT; }
    RsaOperation* InstantiateOperation(AuthorizationSet&& hw_enforced,
                                       AuthorizationSet&& sw_enforced, keymaster_digest_t digest,
                                       keymaster_padding_t padding, EVP_PKEY* key) const override {
        return new (std::nothrow)
            RsaDecryptOperation(move(hw_enforced), move(sw_enforced), digest, padding, key);
    }
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_RSA_OPERATION_H_
