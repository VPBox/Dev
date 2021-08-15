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

#include <keymaster/legacy_support/keymaster_passthrough_key.h>

namespace keymaster {

keymaster_error_t
KeymasterPassthroughKeyFactory::LoadKey(KeymasterKeyBlob&& key_material,
                          const AuthorizationSet& additional_params,
                          AuthorizationSet&& hw_enforced,
                          AuthorizationSet&& sw_enforced,
                          UniquePtr<Key>* key) const {
    keymaster_error_t error = KM_ERROR_OK;
    if (!key)
        return KM_ERROR_OUTPUT_PARAMETER_NULL;

    key->reset(new (std::nothrow) KeymasterPassthroughKey(move(key_material), move(hw_enforced),
                                                          move(sw_enforced), this, &error,
                                                          additional_params, engine_));
    if (!key->get())
        error = KM_ERROR_MEMORY_ALLOCATION_FAILED;

    return error;
}

const keymaster_key_format_t*
KeymasterPassthroughKeyFactory::SupportedImportFormats(size_t* format_count) const {
    if (format_count) *format_count = 0;
    return nullptr;
}
const keymaster_key_format_t*
KeymasterPassthroughKeyFactory::SupportedExportFormats(size_t* format_count) const {
    if (format_count) *format_count = 0;
    return nullptr;
}


keymaster_error_t
KeymasterPassthroughKey::formatted_key_material(keymaster_key_format_t format,
                                                UniquePtr<uint8_t[]>* material,
                                                size_t* size) const {
    if (!material || !size) {
        return KM_ERROR_OUTPUT_PARAMETER_NULL;
    }
    keymaster_blob_t km_app_data = {};
    KeymasterBlob app_data;
    if (additional_parameters_.GetTagValue(TAG_APPLICATION_DATA, &km_app_data)) {
        app_data = KeymasterBlob(km_app_data);
    }

    keymaster_blob_t km_client_id = {};
    KeymasterBlob client_id;
    if (additional_parameters_.GetTagValue(TAG_APPLICATION_ID, &km_client_id)) {
        client_id = KeymasterBlob(km_client_id);
    }

    KeymasterBlob export_data;

    keymaster_error_t error = engine_->ExportKey(format, key_material(), client_id, app_data,
                                                 &export_data);
    if (error == KM_ERROR_OK) {
        keymaster_blob_t export_blob = export_data.release();
        material->reset(const_cast<uint8_t*>(export_blob.data));
        *size = export_blob.data_length;
    }
    return error;
}

}  // namespace keymaster
