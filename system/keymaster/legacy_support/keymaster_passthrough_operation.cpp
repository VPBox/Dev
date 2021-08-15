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

#include "keymaster_passthrough_operation.h"
#include <vector>
#include <keymaster/android_keymaster_utils.h>

namespace keymaster {

template<>
keymaster_error_t
KeymasterPassthroughOperation<keymaster1_device_t>::Finish(const AuthorizationSet& input_params,
                                 const Buffer& input,
                                 const Buffer& signature, AuthorizationSet* output_params,
                                 Buffer* output) {
    keymaster_key_param_set_t out_params = {};
    keymaster_blob_t in{ input.peek_read(), input.available_read() };
    keymaster_blob_t out = {};
    keymaster_error_t rc;
    std::vector<KeymasterBlob> accumulate_output;
    size_t accumulated_output_size = 0;
    AuthorizationSet accumulated_out_params;
    AuthorizationSet mutable_input_params = input_params;
    while (in.data_length != 0) {
        size_t consumed = 0;
        rc = km_device_->update(km_device_, operation_handle_, &mutable_input_params, &in, &consumed, &out_params, &out);
        if (rc == KM_ERROR_OK) {
            accumulate_output.push_back(KeymasterBlob(out));
            accumulated_output_size += out.data_length;
            free(const_cast<uint8_t*>(out.data));
            out = {};
            accumulated_out_params.push_back(out_params);
            keymaster_free_param_set(&out_params);
        } else {
            return rc;
        }

        int aad_pos = mutable_input_params.find(TAG_ASSOCIATED_DATA);
        if (aad_pos != -1) {
            mutable_input_params.erase(aad_pos);
        }

        if (consumed == 0) {
            km_device_->abort(km_device_, operation_handle_);
            return KM_ERROR_INVALID_INPUT_LENGTH;
        } else {
            in.data += consumed;
            if (consumed > in.data_length) return KM_ERROR_UNKNOWN_ERROR;
            in.data_length -= consumed;
        }
    }

    keymaster_blob_t sig{ signature.peek_read(), signature.available_read() };

    rc = km_device_->finish(km_device_, operation_handle_, &mutable_input_params, &sig, &out_params, &out);
    if (rc != KM_ERROR_OK) return rc;
    accumulate_output.push_back(KeymasterBlob(out));
    accumulated_output_size += out.data_length;
    free(const_cast<uint8_t*>(out.data));
    out = {};
    accumulated_out_params.push_back(out_params);
    keymaster_free_param_set(&out_params);

    if (output && accumulated_output_size) {
        if (!output->reserve(accumulated_output_size)) {
            return KM_ERROR_MEMORY_ALLOCATION_FAILED;
        }
        for (auto& outBlob: accumulate_output) {
            output->write(outBlob.data, outBlob.data_length);
        }
    }

    accumulated_out_params.Deduplicate();
    if (output_params) *output_params = move(accumulated_out_params);

    return KM_ERROR_OK;
}

template<>
keymaster_error_t
KeymasterPassthroughOperation<keymaster2_device_t>::Finish(const AuthorizationSet& input_params, const Buffer& input,
                                 const Buffer& signature, AuthorizationSet* output_params,
                                 Buffer* output) {
    keymaster_key_param_set_t out_params = {};
    keymaster_blob_t sig{ signature.peek_read(), signature.available_read() };
    keymaster_blob_t in{ input.peek_read(), input.available_read() };
    keymaster_blob_t out = {};
    keymaster_error_t rc;
    rc = km_device_->finish(km_device_, operation_handle_, &input_params, &in, &sig, &out_params, &out);
    if (rc == KM_ERROR_OK) {
        if (output) output->Reinitialize(out.data, out.data_length);
        if (output_params) output_params->Reinitialize(out_params);
    }
    keymaster_free_param_set(&out_params);
    free(const_cast<uint8_t*>(out.data));
    return rc;
}

}  // namespace keymaster
