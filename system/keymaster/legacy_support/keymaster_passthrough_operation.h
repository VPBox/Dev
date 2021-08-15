/*
**
** Copyright 2017, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_OPERATION_H_
#define SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_OPERATION_H_

#include <hardware/keymaster1.h>
#include <hardware/keymaster2.h>

#include <keymaster/legacy_support/keymaster_passthrough_key.h>
#include <keymaster/operation.h>

namespace keymaster {

class AuthorizationSet;
class Key;
class Operation;

/**
 * Template implementation for KM1 and KM2 operations
 */
template <typename KeymasterDeviceType> class KeymasterPassthroughOperation : public Operation {
  public:
    explicit KeymasterPassthroughOperation(keymaster_purpose_t purpose,
                                           const KeymasterDeviceType* km_device, Key&& key)
        : Operation(purpose, key.hw_enforced_move(), key.sw_enforced_move()),
          key_blob_(key.key_material_move()), km_device_(km_device) {
        operation_handle_ = 0;
    }
    virtual ~KeymasterPassthroughOperation() {}

    keymaster_error_t Begin(const AuthorizationSet& input_params,
                            AuthorizationSet* output_params) override {
        keymaster_key_param_set_t out_params = {};
        keymaster_error_t rc;
        rc = km_device_->begin(km_device_, purpose(), &key_blob_, &input_params, &out_params,
                               &operation_handle_);
        if (rc == KM_ERROR_OK && output_params) output_params->Reinitialize(out_params);
        keymaster_free_param_set(&out_params);
        return rc;
    }
    keymaster_error_t Update(const AuthorizationSet& input_params, const Buffer& input,
                             AuthorizationSet* output_params, Buffer* output,
                             size_t* input_consumed) override {
        keymaster_key_param_set_t out_params = {};
        keymaster_blob_t in{input.peek_read(), input.available_read()};
        keymaster_blob_t out = {};
        keymaster_error_t rc;
        rc = km_device_->update(km_device_, operation_handle_, &input_params, &in, input_consumed,
                                &out_params, &out);
        if (rc == KM_ERROR_OK) {
            if (output) output->Reinitialize(out.data, out.data_length);
            if (output_params) output_params->Reinitialize(out_params);
        }
        keymaster_free_param_set(&out_params);
        free(const_cast<uint8_t*>(out.data));
        return rc;
    }
    keymaster_error_t Finish(const AuthorizationSet& input_params, const Buffer& input,
                             const Buffer& signature, AuthorizationSet* output_params,
                             Buffer* output) override;
    keymaster_error_t Abort() { return km_device_->abort(km_device_, operation_handle_); }

  private:
    KeymasterKeyBlob key_blob_;
    const KeymasterDeviceType* km_device_;
};

template <>
keymaster_error_t KeymasterPassthroughOperation<keymaster1_device_t>::Finish(
    const AuthorizationSet& input_params, const Buffer& input, const Buffer& signature,
    AuthorizationSet* output_params, Buffer* output);
template <>
keymaster_error_t KeymasterPassthroughOperation<keymaster2_device_t>::Finish(
    const AuthorizationSet& input_params, const Buffer& input, const Buffer& signature,
    AuthorizationSet* output_params, Buffer* output);

template <typename KeymasterDeviceType>
class KeymasterPassthroughOperationFactory : public OperationFactory {
  public:
    KeymasterPassthroughOperationFactory(keymaster_algorithm_t algorithm,
                                         keymaster_purpose_t purpose,
                                         const KeymasterDeviceType* km_device)
        : key_type_(algorithm, purpose), km_device_(km_device) {}
    virtual ~KeymasterPassthroughOperationFactory() {}

    KeyType registry_key() const override { return key_type_; }

    // Factory methods
    OperationPtr CreateOperation(Key&& key, const AuthorizationSet& /*begin_params*/,
                                 keymaster_error_t* error) const override {
        if (!error) return nullptr;
        *error = KM_ERROR_OK;
        OperationPtr op(new (std::nothrow) KeymasterPassthroughOperation<KeymasterDeviceType>(
            key_type_.purpose, km_device_, std::move(key)));
        if (!op) {
            *error = KM_ERROR_MEMORY_ALLOCATION_FAILED;
        }
        return op;
    }

    // Informational methods.  The returned arrays reference static memory and must not be
    // deallocated or modified.
    const keymaster_padding_t* SupportedPaddingModes(size_t* padding_count) const override {
        *padding_count = 0;
        return nullptr;
    }
    const keymaster_block_mode_t* SupportedBlockModes(size_t* block_mode_count) const override {
        *block_mode_count = 0;
        return nullptr;
    }
    const keymaster_digest_t* SupportedDigests(size_t* digest_count) const override {
        *digest_count = 0;
        return nullptr;
    }

  private:
    KeyType key_type_;
    const KeymasterDeviceType* km_device_;
};

}  // namespace keymaster

#endif  // SYSTEM_KEYMASTER_KEYMASTER_PASSTHROUGH_OPERATION_H_
