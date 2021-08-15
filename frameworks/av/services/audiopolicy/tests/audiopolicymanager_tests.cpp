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

#include <memory>
#include <set>
#include <sys/wait.h>
#include <unistd.h>

#include <gtest/gtest.h>

#define LOG_TAG "APM_Test"
#include <log/log.h>
#include <media/PatchBuilder.h>

#include "AudioPolicyTestClient.h"
#include "AudioPolicyTestManager.h"

using namespace android;

TEST(AudioPolicyManagerTestInit, Failure) {
    AudioPolicyTestClient client;
    AudioPolicyTestManager manager(&client);
    manager.getConfig().setDefault();
    // Since the default client fails to open anything,
    // APM should indicate that the initialization didn't succeed.
    ASSERT_EQ(NO_INIT, manager.initialize());
    ASSERT_EQ(NO_INIT, manager.initCheck());
}


class AudioPolicyManagerTestClient : public AudioPolicyTestClient {
  public:
    // AudioPolicyClientInterface implementation
    audio_module_handle_t loadHwModule(const char* /*name*/) override {
        return mNextModuleHandle++;
    }

    status_t openOutput(audio_module_handle_t module,
                        audio_io_handle_t* output,
                        audio_config_t* /*config*/,
                        audio_devices_t* /*devices*/,
                        const String8& /*address*/,
                        uint32_t* /*latencyMs*/,
                        audio_output_flags_t /*flags*/) override {
        if (module >= mNextModuleHandle) {
            ALOGE("%s: Module handle %d has not been allocated yet (next is %d)",
                    __func__, module, mNextModuleHandle);
            return BAD_VALUE;
        }
        *output = mNextIoHandle++;
        return NO_ERROR;
    }

    status_t openInput(audio_module_handle_t module,
                       audio_io_handle_t* input,
                       audio_config_t* /*config*/,
                       audio_devices_t* /*device*/,
                       const String8& /*address*/,
                       audio_source_t /*source*/,
                       audio_input_flags_t /*flags*/) override {
        if (module >= mNextModuleHandle) {
            ALOGE("%s: Module handle %d has not been allocated yet (next is %d)",
                    __func__, module, mNextModuleHandle);
            return BAD_VALUE;
        }
        *input = mNextIoHandle++;
        return NO_ERROR;
    }

    status_t createAudioPatch(const struct audio_patch* /*patch*/,
                              audio_patch_handle_t* handle,
                              int /*delayMs*/) override {
        *handle = mNextPatchHandle++;
        mActivePatches.insert(*handle);
        return NO_ERROR;
    }

    status_t releaseAudioPatch(audio_patch_handle_t handle,
                               int /*delayMs*/) override {
        if (mActivePatches.erase(handle) != 1) {
            if (handle >= mNextPatchHandle) {
                ALOGE("%s: Patch handle %d has not been allocated yet (next is %d)",
                        __func__, handle, mNextPatchHandle);
            } else {
                ALOGE("%s: Attempt to release patch %d twice", __func__, handle);
            }
            return BAD_VALUE;
        }
        return NO_ERROR;
    }

    // Helper methods for tests
    size_t getActivePatchesCount() const { return mActivePatches.size(); }

  private:
    audio_module_handle_t mNextModuleHandle = AUDIO_MODULE_HANDLE_NONE + 1;
    audio_io_handle_t mNextIoHandle = AUDIO_IO_HANDLE_NONE + 1;
    audio_patch_handle_t mNextPatchHandle = AUDIO_PATCH_HANDLE_NONE + 1;
    std::set<audio_patch_handle_t> mActivePatches;
};

class PatchCountCheck {
  public:
    explicit PatchCountCheck(AudioPolicyManagerTestClient *client)
            : mClient{client},
              mInitialCount{mClient->getActivePatchesCount()} {}
    int deltaFromSnapshot() const {
        size_t currentCount = mClient->getActivePatchesCount();
        if (mInitialCount <= currentCount) {
            return currentCount - mInitialCount;
        } else {
            return -(static_cast<int>(mInitialCount - currentCount));
        }
    }
  private:
    const AudioPolicyManagerTestClient *mClient;
    const size_t mInitialCount;
};

class AudioPolicyManagerTest : public testing::Test {
  protected:
    void SetUp() override;
    void TearDown() override;
    virtual void SetUpConfig(AudioPolicyConfig *config) { (void)config; }

    void dumpToLog();
    void getOutputForAttr(
            audio_port_handle_t *selectedDeviceId,
            audio_format_t format,
            int channelMask,
            int sampleRate,
            audio_output_flags_t flags = AUDIO_OUTPUT_FLAG_NONE,
            audio_port_handle_t *portId = nullptr);
    PatchCountCheck snapshotPatchCount() { return PatchCountCheck(mClient.get()); }

    std::unique_ptr<AudioPolicyManagerTestClient> mClient;
    std::unique_ptr<AudioPolicyTestManager> mManager;
};

void AudioPolicyManagerTest::SetUp() {
    mClient.reset(new AudioPolicyManagerTestClient);
    mManager.reset(new AudioPolicyTestManager(mClient.get()));
    mManager->getConfig().setDefault();
    SetUpConfig(&mManager->getConfig());  // Subclasses may want to customize the config.
    ASSERT_EQ(NO_ERROR, mManager->initialize());
    ASSERT_EQ(NO_ERROR, mManager->initCheck());
}

void AudioPolicyManagerTest::TearDown() {
    mManager.reset();
    mClient.reset();
}

void AudioPolicyManagerTest::dumpToLog() {
    int pipefd[2];
    ASSERT_NE(-1, pipe(pipefd));
    pid_t cpid = fork();
    ASSERT_NE(-1, cpid);
    if (cpid == 0) {
        // Child process reads from the pipe and logs.
        close(pipefd[1]);
        std::string line;
        char buf;
        while (read(pipefd[0], &buf, sizeof(buf)) > 0) {
            if (buf != '\n') {
                line += buf;
            } else {
                ALOGI("%s", line.c_str());
                line = "";
            }
        }
        if (!line.empty()) ALOGI("%s", line.c_str());
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    } else {
        // Parent does the dump and checks the status code.
        close(pipefd[0]);
        ASSERT_EQ(NO_ERROR, mManager->dump(pipefd[1]));
        close(pipefd[1]);
        wait(NULL);  // Wait for the child to exit.
    }
}

void AudioPolicyManagerTest::getOutputForAttr(
        audio_port_handle_t *selectedDeviceId,
        audio_format_t format,
        int channelMask,
        int sampleRate,
        audio_output_flags_t flags,
        audio_port_handle_t *portId) {
    audio_attributes_t attr = {};
    audio_io_handle_t output = AUDIO_PORT_HANDLE_NONE;
    audio_stream_type_t stream = AUDIO_STREAM_DEFAULT;
    audio_config_t config = AUDIO_CONFIG_INITIALIZER;
    config.sample_rate = sampleRate;
    config.channel_mask = channelMask;
    config.format = format;
    *selectedDeviceId = AUDIO_PORT_HANDLE_NONE;
    audio_port_handle_t localPortId;
    if (!portId) portId = &localPortId;
    *portId = AUDIO_PORT_HANDLE_NONE;
    ASSERT_EQ(OK, mManager->getOutputForAttr(
                    &attr, &output, AUDIO_SESSION_NONE, &stream, 0 /*uid*/, &config, &flags,
                    selectedDeviceId, portId, {}));
    ASSERT_NE(AUDIO_PORT_HANDLE_NONE, *portId);
}


TEST_F(AudioPolicyManagerTest, InitSuccess) {
    // SetUp must finish with no assertions.
}

TEST_F(AudioPolicyManagerTest, Dump) {
    dumpToLog();
}

TEST_F(AudioPolicyManagerTest, CreateAudioPatchFailure) {
    audio_patch patch{};
    audio_patch_handle_t handle = AUDIO_PATCH_HANDLE_NONE;
    const PatchCountCheck patchCount = snapshotPatchCount();
    ASSERT_EQ(BAD_VALUE, mManager->createAudioPatch(nullptr, &handle, 0));
    ASSERT_EQ(BAD_VALUE, mManager->createAudioPatch(&patch, nullptr, 0));
    ASSERT_EQ(BAD_VALUE, mManager->createAudioPatch(&patch, &handle, 0));
    patch.num_sources = AUDIO_PATCH_PORTS_MAX + 1;
    patch.num_sinks = 1;
    ASSERT_EQ(BAD_VALUE, mManager->createAudioPatch(&patch, &handle, 0));
    patch.num_sources = 1;
    patch.num_sinks = AUDIO_PATCH_PORTS_MAX + 1;
    ASSERT_EQ(BAD_VALUE, mManager->createAudioPatch(&patch, &handle, 0));
    patch.num_sources = 2;
    patch.num_sinks = 1;
    ASSERT_EQ(INVALID_OPERATION, mManager->createAudioPatch(&patch, &handle, 0));
    patch = {};
    patch.num_sources = 1;
    patch.sources[0].role = AUDIO_PORT_ROLE_SINK;
    patch.num_sinks = 1;
    patch.sinks[0].role = AUDIO_PORT_ROLE_SINK;
    ASSERT_EQ(INVALID_OPERATION, mManager->createAudioPatch(&patch, &handle, 0));
    patch = {};
    patch.num_sources = 1;
    patch.sources[0].role = AUDIO_PORT_ROLE_SOURCE;
    patch.num_sinks = 1;
    patch.sinks[0].role = AUDIO_PORT_ROLE_SOURCE;
    ASSERT_EQ(INVALID_OPERATION, mManager->createAudioPatch(&patch, &handle, 0));
    // Verify that the handle is left unchanged.
    ASSERT_EQ(AUDIO_PATCH_HANDLE_NONE, handle);
    ASSERT_EQ(0, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTest, CreateAudioPatchFromMix) {
    audio_patch_handle_t handle = AUDIO_PATCH_HANDLE_NONE;
    uid_t uid = 42;
    const PatchCountCheck patchCount = snapshotPatchCount();
    ASSERT_FALSE(mManager->getConfig().getAvailableInputDevices().isEmpty());
    PatchBuilder patchBuilder;
    patchBuilder.addSource(mManager->getConfig().getAvailableInputDevices()[0]).
            addSink(mManager->getConfig().getDefaultOutputDevice());
    ASSERT_EQ(NO_ERROR, mManager->createAudioPatch(patchBuilder.patch(), &handle, uid));
    ASSERT_NE(AUDIO_PATCH_HANDLE_NONE, handle);
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
}

// TODO: Add patch creation tests that involve already existing patch

class AudioPolicyManagerTestMsd : public AudioPolicyManagerTest {
  protected:
    void SetUpConfig(AudioPolicyConfig *config) override;
    void TearDown() override;

    sp<DeviceDescriptor> mMsdOutputDevice;
    sp<DeviceDescriptor> mMsdInputDevice;
};

void AudioPolicyManagerTestMsd::SetUpConfig(AudioPolicyConfig *config) {
    // TODO: Consider using Serializer to load part of the config from a string.
    mMsdOutputDevice = new DeviceDescriptor(AUDIO_DEVICE_OUT_BUS);
    sp<AudioProfile> pcmOutputProfile = new AudioProfile(
            AUDIO_FORMAT_PCM_16_BIT, AUDIO_CHANNEL_OUT_STEREO, 48000);
    sp<AudioProfile> ac3OutputProfile = new AudioProfile(
            AUDIO_FORMAT_AC3, AUDIO_CHANNEL_OUT_5POINT1, 48000);
    mMsdOutputDevice->addAudioProfile(pcmOutputProfile);
    mMsdOutputDevice->addAudioProfile(ac3OutputProfile);
    mMsdInputDevice = new DeviceDescriptor(AUDIO_DEVICE_IN_BUS);
    // Match output profile from AudioPolicyConfig::setDefault.
    sp<AudioProfile> pcmInputProfile = new AudioProfile(
            AUDIO_FORMAT_PCM_16_BIT, AUDIO_CHANNEL_IN_STEREO, 44100);
    mMsdInputDevice->addAudioProfile(pcmInputProfile);
    config->addAvailableDevice(mMsdOutputDevice);
    config->addAvailableDevice(mMsdInputDevice);

    sp<HwModule> msdModule = new HwModule(AUDIO_HARDWARE_MODULE_ID_MSD, 2 /*halVersionMajor*/);
    HwModuleCollection modules = config->getHwModules();
    modules.add(msdModule);
    config->setHwModules(modules);
    mMsdOutputDevice->attach(msdModule);
    mMsdInputDevice->attach(msdModule);

    sp<OutputProfile> msdOutputProfile = new OutputProfile(String8("msd input"));
    msdOutputProfile->addAudioProfile(pcmOutputProfile);
    msdOutputProfile->addSupportedDevice(mMsdOutputDevice);
    msdModule->addOutputProfile(msdOutputProfile);
    sp<OutputProfile> msdCompressedOutputProfile =
            new OutputProfile(String8("msd compressed input"));
    msdCompressedOutputProfile->addAudioProfile(ac3OutputProfile);
    msdCompressedOutputProfile->setFlags(
            AUDIO_OUTPUT_FLAG_DIRECT | AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD |
            AUDIO_OUTPUT_FLAG_NON_BLOCKING);
    msdCompressedOutputProfile->addSupportedDevice(mMsdOutputDevice);
    msdModule->addOutputProfile(msdCompressedOutputProfile);

    sp<InputProfile> msdInputProfile = new InputProfile(String8("msd output"));
    msdInputProfile->addAudioProfile(pcmInputProfile);
    msdInputProfile->addSupportedDevice(mMsdInputDevice);
    msdModule->addInputProfile(msdInputProfile);

    // Add a profile with another encoding to the default device to test routing
    // of streams that are not supported by MSD.
    sp<AudioProfile> dtsOutputProfile = new AudioProfile(
            AUDIO_FORMAT_DTS, AUDIO_CHANNEL_OUT_5POINT1, 48000);
    config->getDefaultOutputDevice()->addAudioProfile(dtsOutputProfile);
    sp<OutputProfile> primaryEncodedOutputProfile = new OutputProfile(String8("encoded"));
    primaryEncodedOutputProfile->addAudioProfile(dtsOutputProfile);
    primaryEncodedOutputProfile->setFlags(AUDIO_OUTPUT_FLAG_DIRECT);
    primaryEncodedOutputProfile->addSupportedDevice(config->getDefaultOutputDevice());
    config->getHwModules().getModuleFromName(AUDIO_HARDWARE_MODULE_ID_PRIMARY)->
            addOutputProfile(primaryEncodedOutputProfile);
}

void AudioPolicyManagerTestMsd::TearDown() {
    mMsdOutputDevice.clear();
    mMsdInputDevice.clear();
    AudioPolicyManagerTest::TearDown();
}

TEST_F(AudioPolicyManagerTestMsd, InitSuccess) {
    ASSERT_TRUE(mMsdOutputDevice);
    ASSERT_TRUE(mMsdInputDevice);
}

TEST_F(AudioPolicyManagerTestMsd, Dump) {
    dumpToLog();
}

TEST_F(AudioPolicyManagerTestMsd, PatchCreationOnSetForceUse) {
    const PatchCountCheck patchCount = snapshotPatchCount();
    mManager->setForceUse(AUDIO_POLICY_FORCE_FOR_ENCODED_SURROUND,
            AUDIO_POLICY_FORCE_ENCODED_SURROUND_ALWAYS);
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTestMsd, GetOutputForAttrEncodedRoutesToMsd) {
    const PatchCountCheck patchCount = snapshotPatchCount();
    audio_port_handle_t selectedDeviceId;
    getOutputForAttr(&selectedDeviceId,
            AUDIO_FORMAT_AC3, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT);
    ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTestMsd, GetOutputForAttrPcmRoutesToMsd) {
    const PatchCountCheck patchCount = snapshotPatchCount();
    audio_port_handle_t selectedDeviceId;
    getOutputForAttr(&selectedDeviceId,
            AUDIO_FORMAT_PCM_16_BIT, AUDIO_CHANNEL_OUT_STEREO, 48000);
    ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTestMsd, GetOutputForAttrEncodedPlusPcmRoutesToMsd) {
    const PatchCountCheck patchCount = snapshotPatchCount();
    audio_port_handle_t selectedDeviceId;
    getOutputForAttr(&selectedDeviceId,
            AUDIO_FORMAT_AC3, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT);
    ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
    getOutputForAttr(&selectedDeviceId,
            AUDIO_FORMAT_PCM_16_BIT, AUDIO_CHANNEL_OUT_STEREO, 48000);
    ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
    ASSERT_EQ(1, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTestMsd, GetOutputForAttrUnsupportedFormatBypassesMsd) {
    const PatchCountCheck patchCount = snapshotPatchCount();
    audio_port_handle_t selectedDeviceId;
    getOutputForAttr(&selectedDeviceId,
            AUDIO_FORMAT_DTS, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT);
    ASSERT_NE(selectedDeviceId, mMsdOutputDevice->getId());
    ASSERT_EQ(0, patchCount.deltaFromSnapshot());
}

TEST_F(AudioPolicyManagerTestMsd, GetOutputForAttrFormatSwitching) {
    // Switch between formats that are supported and not supported by MSD.
    {
        const PatchCountCheck patchCount = snapshotPatchCount();
        audio_port_handle_t selectedDeviceId, portId;
        getOutputForAttr(&selectedDeviceId,
                AUDIO_FORMAT_AC3, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT,
                &portId);
        ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
        ASSERT_EQ(1, patchCount.deltaFromSnapshot());
        mManager->releaseOutput(portId);
        ASSERT_EQ(1, patchCount.deltaFromSnapshot());
    }
    {
        const PatchCountCheck patchCount = snapshotPatchCount();
        audio_port_handle_t selectedDeviceId, portId;
        getOutputForAttr(&selectedDeviceId,
                AUDIO_FORMAT_DTS, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT,
                &portId);
        ASSERT_NE(selectedDeviceId, mMsdOutputDevice->getId());
        ASSERT_EQ(-1, patchCount.deltaFromSnapshot());
        mManager->releaseOutput(portId);
        ASSERT_EQ(0, patchCount.deltaFromSnapshot());
    }
    {
        const PatchCountCheck patchCount = snapshotPatchCount();
        audio_port_handle_t selectedDeviceId;
        getOutputForAttr(&selectedDeviceId,
                AUDIO_FORMAT_AC3, AUDIO_CHANNEL_OUT_5POINT1, 48000, AUDIO_OUTPUT_FLAG_DIRECT);
        ASSERT_EQ(selectedDeviceId, mMsdOutputDevice->getId());
        ASSERT_EQ(0, patchCount.deltaFromSnapshot());
    }
}
