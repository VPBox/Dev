/*
 * Copyright (C) 2017 The Android Open Source Project
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
#pragma once

#include "AudioPolicyInterface.h"

namespace android {

class AudioPolicyTestClient : public AudioPolicyClientInterface
{
public:
    virtual ~AudioPolicyTestClient() = default;

    // AudioPolicyClientInterface Implementation
    audio_module_handle_t loadHwModule(const char* /*name*/) override {
        return AUDIO_MODULE_HANDLE_NONE;
    }
    status_t openOutput(audio_module_handle_t /*module*/,
                        audio_io_handle_t* /*output*/,
                        audio_config_t* /*config*/,
                        audio_devices_t* /*devices*/,
                        const String8& /*address*/,
                        uint32_t* /*latencyMs*/,
                        audio_output_flags_t /*flags*/) override { return NO_INIT; }
    audio_io_handle_t openDuplicateOutput(audio_io_handle_t /*output1*/,
                                          audio_io_handle_t /*output2*/) override {
        return AUDIO_IO_HANDLE_NONE;
    }
    status_t closeOutput(audio_io_handle_t /*output*/) override { return NO_INIT; }
    status_t suspendOutput(audio_io_handle_t /*output*/) override { return NO_INIT; }
    status_t restoreOutput(audio_io_handle_t /*output*/) override { return NO_INIT; }
    status_t openInput(audio_module_handle_t /*module*/,
                       audio_io_handle_t* /*input*/,
                       audio_config_t* /*config*/,
                       audio_devices_t* /*device*/,
                       const String8& /*address*/,
                       audio_source_t /*source*/,
                       audio_input_flags_t /*flags*/) override { return NO_INIT; }
    status_t closeInput(audio_io_handle_t /*input*/) override { return NO_INIT; }
    status_t setStreamVolume(audio_stream_type_t /*stream*/,
                             float /*volume*/,
                             audio_io_handle_t /*output*/,
                             int /*delayMs*/) override { return NO_INIT; }
    status_t invalidateStream(audio_stream_type_t /*stream*/) override { return NO_INIT; }
    void setParameters(audio_io_handle_t /*ioHandle*/,
                       const String8& /*keyValuePairs*/,
                       int /*delayMs*/) override { }
    String8 getParameters(audio_io_handle_t /*ioHandle*/,
                          const String8& /*keys*/) override { return String8(); }
    status_t setVoiceVolume(float /*volume*/, int /*delayMs*/) override { return NO_INIT; }
    status_t moveEffects(audio_session_t /*session*/,
                         audio_io_handle_t /*srcOutput*/,
                         audio_io_handle_t /*dstOutput*/) override { return NO_INIT; }
    status_t createAudioPatch(const struct audio_patch* /*patch*/,
                              audio_patch_handle_t* /*handle*/,
                              int /*delayMs*/) override { return NO_INIT; }
    status_t releaseAudioPatch(audio_patch_handle_t /*handle*/,
                               int /*delayMs*/) override { return NO_INIT; }
    status_t setAudioPortConfig(const struct audio_port_config* /*config*/,
                                int /*delayMs*/) override { return NO_INIT; }
    void onAudioPortListUpdate() override { }
    void onAudioPatchListUpdate() override { }
    void onAudioVolumeGroupChanged(volume_group_t /*group*/, int /*flags*/) override { }
    audio_unique_id_t newAudioUniqueId(audio_unique_id_use_t /*use*/) override { return 0; }
    void onDynamicPolicyMixStateUpdate(String8 /*regId*/, int32_t /*state*/) override { }
    void onRecordingConfigurationUpdate(int event __unused,
                                        const record_client_info_t *clientInfo __unused,
                                        const audio_config_base_t *clientConfig __unused,
                                        std::vector<effect_descriptor_t> clientEffects __unused,
                                        const audio_config_base_t *deviceConfig __unused,
                                        std::vector<effect_descriptor_t> effects __unused,
                                        audio_patch_handle_t patchHandle __unused,
                                        audio_source_t source __unused) override { }
    void setEffectSuspended(int effectId __unused,
                            audio_session_t sessionId __unused,
                            bool suspended __unused) {}
};

} // namespace android
