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

#ifndef AAUDIO_EXAMPLE_ARGS_PARSER_H
#define AAUDIO_EXAMPLE_ARGS_PARSER_H

#define MAX_CHANNELS                     8

//#include <cctype>
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <aaudio/AAudio.h>
#include <aaudio/AAudioTesting.h>

#include "AAudioExampleUtils.h"


static void (*s_setUsage)(AAudioStreamBuilder* builder, aaudio_usage_t usage) = nullptr;
static void (*s_setContentType)(AAudioStreamBuilder* builder,
                                aaudio_content_type_t contentType) = nullptr;
static void (*s_setInputPreset)(AAudioStreamBuilder* builder,
                                aaudio_input_preset_t inputPreset) = nullptr;
static void (*s_setAllowedCapturePolicy)(AAudioStreamBuilder* builder,
                                          aaudio_allowed_capture_policy_t usage) = nullptr;

static bool s_loadAttempted = false;
static aaudio_usage_t (*s_getUsage)(AAudioStream *stream) = nullptr;
static aaudio_content_type_t (*s_getContentType)(AAudioStream *stream) = nullptr;
static aaudio_input_preset_t (*s_getInputPreset)(AAudioStream *stream) = nullptr;
static aaudio_allowed_capture_policy_t (*s_getAllowedCapturePolicy)(AAudioStream *stream) = nullptr;

// Link to test functions in shared library.
static void loadFutureFunctions() {
    if (s_loadAttempted)  return; // only try once
    s_loadAttempted = true;

    void *handle = dlopen("libaaudio.so", RTLD_NOW);
    if (handle != nullptr) {
        s_setUsage = (void (*)(AAudioStreamBuilder *, aaudio_usage_t))
                dlsym(handle, "AAudioStreamBuilder_setUsage");
        if (s_setUsage == nullptr) goto error;

        s_setContentType = (void (*)(AAudioStreamBuilder *, aaudio_content_type_t))
                dlsym(handle, "AAudioStreamBuilder_setContentType");
        if (s_setContentType == nullptr) goto error;

        s_setInputPreset = (void (*)(AAudioStreamBuilder *, aaudio_input_preset_t))
                dlsym(handle, "AAudioStreamBuilder_setInputPreset");
        if (s_setInputPreset == nullptr) goto error;

        s_setAllowedCapturePolicy = (void (*)(AAudioStreamBuilder *, aaudio_input_preset_t))
                dlsym(handle, "AAudioStreamBuilder_setAllowedCapturePolicy");
        if (s_setAllowedCapturePolicy == nullptr) goto error;

        s_getUsage = (aaudio_usage_t (*)(AAudioStream *))
                dlsym(handle, "AAudioStream_getUsage");
        if (s_getUsage == nullptr) goto error;

        s_getContentType = (aaudio_content_type_t (*)(AAudioStream *))
                dlsym(handle, "AAudioStream_getContentType");
        if (s_getContentType == nullptr) goto error;

        s_getInputPreset = (aaudio_input_preset_t (*)(AAudioStream *))
                dlsym(handle, "AAudioStream_getInputPreset");
        if (s_getInputPreset == nullptr) goto error;

        s_getAllowedCapturePolicy = (aaudio_input_preset_t (*)(AAudioStream *))
                dlsym(handle, "AAudioStream_getAllowedCapturePolicy");
        if (s_getAllowedCapturePolicy == nullptr) goto error;
    }
    return;

error:
    // prevent any calls to these functions
    s_setUsage = nullptr;
    s_setContentType = nullptr;
    s_setInputPreset = nullptr;
    s_getUsage = nullptr;
    s_getContentType = nullptr;
    s_getInputPreset = nullptr;
    dlclose(handle);
    return;
}

class AAudioParameters {
public:

    /**
     * This is also known as samplesPerFrame.
     */
    int32_t getChannelCount() const {
        return mChannelCount;
    }

    void setChannelCount(int32_t channelCount) {
        if (channelCount > MAX_CHANNELS) {
            printf("Sorry, MAX of %d channels!\n", MAX_CHANNELS);
            channelCount = MAX_CHANNELS;
        }
        mChannelCount = channelCount;
    }

    int32_t getSampleRate() const {
        return mSampleRate;
    }

    void setSampleRate(int32_t sampleRate) {
        mSampleRate = sampleRate;
    }

    aaudio_format_t getFormat() const {
        return mFormat;
    }

    void setFormat(aaudio_format_t format) {
        mFormat = format;
    }

    aaudio_sharing_mode_t getSharingMode() const {
        return mSharingMode;
    }

    void setSharingMode(aaudio_sharing_mode_t sharingMode) {
        mSharingMode = sharingMode;
    }

    int32_t getBufferCapacity() const {
        return mBufferCapacity;
    }

    void setBufferCapacity(int32_t frames) {
        mBufferCapacity = frames;
    }

    int32_t getPerformanceMode() const {
        return mPerformanceMode;
    }

    void setPerformanceMode(aaudio_performance_mode_t performanceMode) {
        mPerformanceMode = performanceMode;
    }

    aaudio_usage_t getUsage() const {
        return mUsage;
    }

    void setUsage(aaudio_usage_t usage) {
        mUsage = usage;
    }

    aaudio_content_type_t getContentType() const {
        return mContentType;
    }

    void setContentType(aaudio_content_type_t contentType) {
        mContentType = contentType;
    }

    aaudio_input_preset_t getInputPreset() const {
        return mInputPreset;
    }

    void setInputPreset(aaudio_input_preset_t inputPreset) {
        mInputPreset = inputPreset;
    }

    aaudio_allowed_capture_policy_t getAllowedCapturePolicy() const {
        return mAllowedCapturePolicy;
    }

    void setAllowedCapturePolicy(aaudio_allowed_capture_policy_t policy) {
        mAllowedCapturePolicy = policy;
    }

    int32_t getDeviceId() const {
        return mDeviceId;
    }

    void setDeviceId(int32_t deviceId) {
        mDeviceId = deviceId;
    }

    int32_t getNumberOfBursts() const {
        return mNumberOfBursts;
    }

    void setNumberOfBursts(int32_t numBursts) {
        mNumberOfBursts = numBursts;
    }

    int32_t getFramesPerCallback() const {
        return mFramesPerCallback;
    }
    void setFramesPerCallback(int32_t size) {
        mFramesPerCallback = size;
    }

    /**
     * Apply these parameters to a stream builder.
     * @param builder
     */
    void applyParameters(AAudioStreamBuilder *builder) const {
        AAudioStreamBuilder_setBufferCapacityInFrames(builder, getBufferCapacity());
        AAudioStreamBuilder_setChannelCount(builder, mChannelCount);
        AAudioStreamBuilder_setDeviceId(builder, mDeviceId);
        AAudioStreamBuilder_setFormat(builder, mFormat);
        AAudioStreamBuilder_setFramesPerDataCallback(builder, mFramesPerCallback);
        AAudioStreamBuilder_setPerformanceMode(builder, mPerformanceMode);
        AAudioStreamBuilder_setSampleRate(builder, mSampleRate);
        AAudioStreamBuilder_setSharingMode(builder, mSharingMode);

        // Call P functions if supported.
        loadFutureFunctions();
        if (s_setUsage != nullptr) {
            s_setUsage(builder, mUsage);
        } else if (mUsage != AAUDIO_UNSPECIFIED){
            printf("WARNING: setUsage not supported");
        }
        if (s_setContentType != nullptr) {
            s_setContentType(builder, mContentType);
        } else if (mUsage != AAUDIO_UNSPECIFIED){
            printf("WARNING: setContentType not supported");
        }
        if (s_setInputPreset != nullptr) {
            s_setInputPreset(builder, mInputPreset);
        } else if (mUsage != AAUDIO_UNSPECIFIED){
            printf("WARNING: setInputPreset not supported");
        }

        // Call Q functions if supported.
        if (s_setAllowedCapturePolicy != nullptr) {
            s_setAllowedCapturePolicy(builder, mAllowedCapturePolicy);
        } else if (mAllowedCapturePolicy != AAUDIO_UNSPECIFIED){
            printf("WARNING: setAllowedCapturePolicy not supported");
        }
    }

    static constexpr int32_t   kDefaultNumberOfBursts = 2;

private:
    int32_t                    mChannelCount    = AAUDIO_UNSPECIFIED;
    aaudio_format_t            mFormat          = AAUDIO_FORMAT_UNSPECIFIED;
    int32_t                    mSampleRate      = AAUDIO_UNSPECIFIED;

    int32_t                    mBufferCapacity  = AAUDIO_UNSPECIFIED;
    int32_t                    mDeviceId        = AAUDIO_UNSPECIFIED;
    aaudio_sharing_mode_t      mSharingMode     = AAUDIO_SHARING_MODE_SHARED;
    aaudio_performance_mode_t  mPerformanceMode = AAUDIO_PERFORMANCE_MODE_NONE;

    aaudio_usage_t             mUsage           = AAUDIO_UNSPECIFIED;
    aaudio_content_type_t      mContentType     = AAUDIO_UNSPECIFIED;
    aaudio_input_preset_t      mInputPreset     = AAUDIO_UNSPECIFIED;
    aaudio_allowed_capture_policy_t mAllowedCapturePolicy     = AAUDIO_UNSPECIFIED;

    int32_t                    mNumberOfBursts  = kDefaultNumberOfBursts;
    int32_t                    mFramesPerCallback = AAUDIO_UNSPECIFIED;
};

class AAudioArgsParser : public AAudioParameters {
public:
    AAudioArgsParser() = default;
    ~AAudioArgsParser() = default;

    enum {
        DEFAULT_DURATION_SECONDS = 5
    };

    /**
     * @param arg
     * @return true if the argument was not handled
     */
    bool parseArg(const char *arg) {
        bool unrecognized = false;
        if (arg[0] == '-') {
            char option = arg[1];
            switch (option) {
                case 'b':
                    setBufferCapacity(atoi(&arg[2]));
                    break;
                case 'c':
                    setChannelCount(atoi(&arg[2]));
                    break;
                case 'C':
                    setAllowedCapturePolicy(parseAllowedCapturePolicy(arg[2]));
                    break;
                case 'd':
                    setDeviceId(atoi(&arg[2]));
                    break;
                case 'f':
                    setFormat(atoi(&arg[2]));
                    break;
                case 'i':
                    setInputPreset(atoi(&arg[2]));
                    break;
                case 'm': {
                    aaudio_policy_t policy = AAUDIO_POLICY_AUTO;
                    if (strlen(arg) > 2) {
                        policy = atoi(&arg[2]);
                    }
                    if (AAudio_setMMapPolicy(policy) != AAUDIO_OK) {
                        printf("ERROR: invalid MMAP policy mode %i\n", policy);
                    }
                } break;
                case 'n':
                    setNumberOfBursts(atoi(&arg[2]));
                    break;
                case 'p':
                    setPerformanceMode(parsePerformanceMode(arg[2]));
                    break;
                case 'r':
                    setSampleRate(atoi(&arg[2]));
                    break;
                case 's':
                    mDurationSeconds = atoi(&arg[2]);
                    break;
                case 'u':
                    setUsage(atoi(&arg[2]));
                    break;
                case 'x':
                    setSharingMode(AAUDIO_SHARING_MODE_EXCLUSIVE);
                    break;
                case 'y':
                    setContentType(atoi(&arg[2]));
                    break;
                case 'z':
                    setFramesPerCallback(atoi(&arg[2]));
                    break;
                default:
                    unrecognized = true;
                    break;
            }
        }
        return unrecognized;
    }

    /**
     *
     * @param argc
     * @param argv
     * @return true if an unrecognized argument was passed
     */
    bool parseArgs(int argc, const char **argv) {
        for (int i = 1; i < argc; i++) {
            const char *arg = argv[i];
            if (parseArg(arg)) {
                usage();
                return true;
            }

        }
        return false;
    }

    static void usage() {
        printf("-c{channels} -d{deviceId} -m{mmapPolicy} -n{burstsPerBuffer} -p{perfMode}");
        printf(" -r{rate} -s{seconds} -x\n");
        printf("      Default values are UNSPECIFIED unless otherwise stated.\n");
        printf("      -b{bufferCapacity} frames\n");
        printf("      -c{channels} for example 2 for stereo\n");
        printf("      -C{a|s|n} set playback capture policy\n");
        printf("          a = _ALL (default)\n");
        printf("          s = _SYSTEM\n");
        printf("          n = _NONE\n");
        printf("      -d{deviceId} default is %d\n", AAUDIO_UNSPECIFIED);
        printf("      -f{0|1|2} set format\n");
        printf("          0 = UNSPECIFIED\n");
        printf("          1 = PCM_I16\n");
        printf("          2 = FLOAT\n");
        printf("      -i{inputPreset} eg. 5 for AAUDIO_INPUT_PRESET_CAMCORDER\n");
        printf("      -m{0|1|2|3} set MMAP policy\n");
        printf("          0 = _UNSPECIFIED, use aaudio.mmap_policy system property, default\n");
        printf("          1 = _NEVER, never use MMAP\n");
        printf("          2 = _AUTO, use MMAP if available, default for -m with no number\n");
        printf("          3 = _ALWAYS, use MMAP or fail\n");
        printf("      -n{numberOfBursts} for setBufferSize, default %d\n", kDefaultNumberOfBursts);
        printf("      -p{performanceMode} set output AAUDIO_PERFORMANCE_MODE*, default NONE\n");
        printf("          n for _NONE\n");
        printf("          l for _LATENCY\n");
        printf("          p for _POWER_SAVING;\n");
        printf("      -r{sampleRate} for example 44100\n");
        printf("      -s{duration} in seconds, default is %d\n", DEFAULT_DURATION_SECONDS);
        printf("      -u{usage} eg. 14 for AAUDIO_USAGE_GAME\n");
        printf("      -x to use EXCLUSIVE mode\n");
        printf("      -y{contentType} eg. 1 for AAUDIO_CONTENT_TYPE_SPEECH\n");
        printf("      -z{callbackSize} or block size, in frames, default = 0\n");
    }

    static aaudio_performance_mode_t parseAllowedCapturePolicy(char c) {
        aaudio_allowed_capture_policy_t policy = AAUDIO_ALLOW_CAPTURE_BY_ALL;
        switch (c) {
            case 'a':
                policy = AAUDIO_ALLOW_CAPTURE_BY_ALL;
                break;
            case 's':
                policy = AAUDIO_ALLOW_CAPTURE_BY_SYSTEM;
                break;
            case 'n':
                policy = AAUDIO_ALLOW_CAPTURE_BY_NONE;
                break;
            default:
                printf("ERROR: invalid playback capture policy %c\n", c);
                break;
        }
        return policy;
    }

    static aaudio_performance_mode_t parsePerformanceMode(char c) {
        aaudio_performance_mode_t mode = AAUDIO_PERFORMANCE_MODE_NONE;
        switch (c) {
            case 'n':
                mode = AAUDIO_PERFORMANCE_MODE_NONE;
                break;
            case 'l':
                mode = AAUDIO_PERFORMANCE_MODE_LOW_LATENCY;
                break;
            case 'p':
                mode = AAUDIO_PERFORMANCE_MODE_POWER_SAVING;
                break;
            default:
                printf("ERROR: invalid performance mode %c\n", c);
                break;
        }
        return mode;
    }

    /**
     * Print stream parameters in comparison with requested values.
     * @param stream
     */
    void compareWithStream(AAudioStream *stream) const {

        printf("  DeviceId:     requested = %d, actual = %d\n",
               getDeviceId(), AAudioStream_getDeviceId(stream));

        aaudio_stream_state_t state = AAudioStream_getState(stream);
        printf("  State:        %s\n", AAudio_convertStreamStateToText(state));

        // Check to see what kind of stream we actually got.
        printf("  SampleRate:   requested = %d, actual = %d\n",
               getSampleRate(), AAudioStream_getSampleRate(stream));

        printf("  ChannelCount: requested = %d, actual = %d\n",
               getChannelCount(), AAudioStream_getChannelCount(stream));

        printf("  DataFormat:   requested = %d, actual = %d\n",
               getFormat(), AAudioStream_getFormat(stream));

        int32_t framesPerBurst = AAudioStream_getFramesPerBurst(stream);
        printf("  Buffer:       burst     = %d\n", framesPerBurst);

        int32_t sizeFrames = AAudioStream_getBufferSizeInFrames(stream);
        if (framesPerBurst > 0) {
            int32_t requestedSize = getNumberOfBursts() * framesPerBurst;
            printf("  BufferSize:   requested = %4d, actual = %4d = (%d * %d) + %d\n",
                   requestedSize,
                   sizeFrames,
                   (sizeFrames / framesPerBurst),
                   framesPerBurst,
                   (sizeFrames % framesPerBurst));
        } else {
             printf("  BufferSize:    %d\n", sizeFrames);
        }

        int32_t capacityFrames = AAudioStream_getBufferCapacityInFrames(stream);
        printf("  Capacity:     requested = %4d, actual = %4d = (%d * %d) + %d\n",
               getBufferCapacity(),
               capacityFrames,
               (capacityFrames / framesPerBurst),
               framesPerBurst,
               (capacityFrames % framesPerBurst));

        printf("  CallbackSize: requested = %d, actual = %d\n", getFramesPerCallback(),
               AAudioStream_getFramesPerDataCallback(stream));

        printf("  SharingMode:  requested = %s, actual = %s\n",
               getSharingModeText(getSharingMode()),
               getSharingModeText(AAudioStream_getSharingMode(stream)));

        printf("  PerformanceMode: requested = %d, actual = %d\n",
               getPerformanceMode(), AAudioStream_getPerformanceMode(stream));

        loadFutureFunctions();

        if (s_setUsage != nullptr) {
            printf("  Usage:        requested = %d, actual = %d\n",
                   getUsage(), s_getUsage(stream));
        }
        if (s_getContentType != nullptr) {
            printf("  ContentType:  requested = %d, actual = %d\n",
                   getContentType(), s_getContentType(stream));
        }

        if (AAudioStream_getDirection(stream) == AAUDIO_DIRECTION_INPUT
            && s_getInputPreset != nullptr) {
                printf("  InputPreset:  requested = %d, actual = %d\n",
                       getInputPreset(), s_getInputPreset(stream));
        }

        printf("  Is MMAP used? %s\n", AAudioStream_isMMapUsed(stream)
               ? "yes" : "no");

        if (s_getAllowedCapturePolicy != nullptr) {
            printf("  ContentType:  requested = %d, actual = %d\n",
                   getAllowedCapturePolicy(), s_getAllowedCapturePolicy(stream));
        }

    }

    int32_t getDurationSeconds() const {
        return mDurationSeconds;
    }

    void setDurationSeconds(int32_t seconds) {
        mDurationSeconds = seconds;
    }

private:
    int32_t      mDurationSeconds = DEFAULT_DURATION_SECONDS;
};

#endif // AAUDIO_EXAMPLE_ARGS_PARSER_H
