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

// Test AAudio attributes such as Usage, ContentType and InputPreset.

#include <stdio.h>
#include <unistd.h>

#include <aaudio/AAudio.h>
#include <gtest/gtest.h>

constexpr int64_t kNanosPerSecond = 1000000000;
constexpr int kNumFrames = 256;
constexpr int kChannelCount = 2;

constexpr int32_t DONT_SET = -1000;

static void checkAttributes(aaudio_performance_mode_t perfMode,
                            aaudio_usage_t usage,
                            aaudio_content_type_t contentType,
                            aaudio_input_preset_t preset = DONT_SET,
                            aaudio_allowed_capture_policy_t capturePolicy = DONT_SET,
                            aaudio_direction_t direction = AAUDIO_DIRECTION_OUTPUT) {

    float *buffer = new float[kNumFrames * kChannelCount];

    AAudioStreamBuilder *aaudioBuilder = nullptr;
    AAudioStream *aaudioStream = nullptr;

    // Use an AAudioStreamBuilder to contain requested parameters.
    ASSERT_EQ(AAUDIO_OK, AAudio_createStreamBuilder(&aaudioBuilder));

    // Request stream properties.
    AAudioStreamBuilder_setPerformanceMode(aaudioBuilder, perfMode);
    AAudioStreamBuilder_setDirection(aaudioBuilder, direction);

    // Set the attribute in the builder.
    if (usage != DONT_SET) {
        AAudioStreamBuilder_setUsage(aaudioBuilder, usage);
    }
    if (contentType != DONT_SET) {
        AAudioStreamBuilder_setContentType(aaudioBuilder, contentType);
    }
    if (preset != DONT_SET) {
        AAudioStreamBuilder_setInputPreset(aaudioBuilder, preset);
    }
    if (capturePolicy != DONT_SET) {
        AAudioStreamBuilder_setAllowedCapturePolicy(aaudioBuilder, capturePolicy);
    }

    // Create an AAudioStream using the Builder.
    ASSERT_EQ(AAUDIO_OK, AAudioStreamBuilder_openStream(aaudioBuilder, &aaudioStream));
    AAudioStreamBuilder_delete(aaudioBuilder);

    // Make sure we get the same attributes back from the stream.
    aaudio_usage_t expectedUsage =
            (usage == DONT_SET || usage == AAUDIO_UNSPECIFIED)
            ? AAUDIO_USAGE_MEDIA // default
            : usage;
    EXPECT_EQ(expectedUsage, AAudioStream_getUsage(aaudioStream));

    aaudio_content_type_t expectedContentType =
            (contentType == DONT_SET || contentType == AAUDIO_UNSPECIFIED)
            ? AAUDIO_CONTENT_TYPE_MUSIC // default
            : contentType;
    EXPECT_EQ(expectedContentType, AAudioStream_getContentType(aaudioStream));

    aaudio_input_preset_t expectedPreset =
            (preset == DONT_SET || preset == AAUDIO_UNSPECIFIED)
            ? AAUDIO_INPUT_PRESET_VOICE_RECOGNITION // default
            : preset;
    EXPECT_EQ(expectedPreset, AAudioStream_getInputPreset(aaudioStream));

    aaudio_allowed_capture_policy_t expectedCapturePolicy =
            (capturePolicy == DONT_SET || capturePolicy == AAUDIO_UNSPECIFIED)
            ? AAUDIO_ALLOW_CAPTURE_BY_ALL // default
            : preset;
    EXPECT_EQ(expectedCapturePolicy, AAudioStream_getAllowedCapturePolicy(aaudioStream));

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStart(aaudioStream));

    if (direction == AAUDIO_DIRECTION_INPUT) {
        EXPECT_EQ(kNumFrames,
                  AAudioStream_read(aaudioStream, buffer, kNumFrames, kNanosPerSecond));
    } else {
        EXPECT_EQ(kNumFrames,
                  AAudioStream_write(aaudioStream, buffer, kNumFrames, kNanosPerSecond));
    }

    EXPECT_EQ(AAUDIO_OK, AAudioStream_requestStop(aaudioStream));

    EXPECT_EQ(AAUDIO_OK, AAudioStream_close(aaudioStream));
    delete[] buffer;
}

static const aaudio_usage_t sUsages[] = {
    DONT_SET,
    AAUDIO_UNSPECIFIED,
    AAUDIO_USAGE_MEDIA,
    AAUDIO_USAGE_VOICE_COMMUNICATION,
    AAUDIO_USAGE_VOICE_COMMUNICATION_SIGNALLING,
    AAUDIO_USAGE_ALARM,
    AAUDIO_USAGE_NOTIFICATION,
    AAUDIO_USAGE_NOTIFICATION_RINGTONE,
    AAUDIO_USAGE_NOTIFICATION_EVENT,
    AAUDIO_USAGE_ASSISTANCE_ACCESSIBILITY,
    AAUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE,
    AAUDIO_USAGE_ASSISTANCE_SONIFICATION,
    AAUDIO_USAGE_GAME,
    AAUDIO_USAGE_ASSISTANT
};

static const aaudio_content_type_t sContentypes[] = {
    DONT_SET,
    AAUDIO_UNSPECIFIED,
    AAUDIO_CONTENT_TYPE_SPEECH,
    AAUDIO_CONTENT_TYPE_MUSIC,
    AAUDIO_CONTENT_TYPE_MOVIE,
    AAUDIO_CONTENT_TYPE_SONIFICATION
};

static const aaudio_input_preset_t sInputPresets[] = {
    DONT_SET,
    AAUDIO_UNSPECIFIED,
    AAUDIO_INPUT_PRESET_GENERIC,
    AAUDIO_INPUT_PRESET_CAMCORDER,
    AAUDIO_INPUT_PRESET_VOICE_RECOGNITION,
    AAUDIO_INPUT_PRESET_VOICE_COMMUNICATION,
    AAUDIO_INPUT_PRESET_UNPROCESSED,
    AAUDIO_INPUT_PRESET_VOICE_PERFORMANCE,
};

static const aaudio_input_preset_t sAllowCapturePolicies[] = {
    DONT_SET,
    AAUDIO_UNSPECIFIED,
    AAUDIO_ALLOW_CAPTURE_BY_ALL,
    AAUDIO_ALLOW_CAPTURE_BY_SYSTEM,
    AAUDIO_ALLOW_CAPTURE_BY_NONE,
};

static void checkAttributesUsage(aaudio_performance_mode_t perfMode) {
    for (aaudio_usage_t usage : sUsages) {
        checkAttributes(perfMode, usage, DONT_SET);
    }
}

static void checkAttributesContentType(aaudio_performance_mode_t perfMode) {
    for (aaudio_content_type_t contentType : sContentypes) {
        checkAttributes(perfMode, DONT_SET, contentType);
    }
}

static void checkAttributesInputPreset(aaudio_performance_mode_t perfMode) {
    for (aaudio_input_preset_t inputPreset : sInputPresets) {
        checkAttributes(perfMode,
                        DONT_SET,
                        DONT_SET,
                        inputPreset,
                        DONT_SET,
                        AAUDIO_DIRECTION_INPUT);
    }
}

static void checkAttributesAllowedCapturePolicy(aaudio_performance_mode_t perfMode) {
    for (aaudio_allowed_capture_policy_t policy : sAllowCapturePolicies) {
        checkAttributes(perfMode,
                        DONT_SET,
                        DONT_SET,
                        DONT_SET,
                        policy,
                        AAUDIO_DIRECTION_INPUT);
    }
}

TEST(test_attributes, aaudio_usage_perfnone) {
    checkAttributesUsage(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_attributes, aaudio_content_type_perfnone) {
    checkAttributesContentType(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_attributes, aaudio_input_preset_perfnone) {
    checkAttributesInputPreset(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_attributes, aaudio_allowed_capture_policy_perfnone) {
    checkAttributesAllowedCapturePolicy(AAUDIO_PERFORMANCE_MODE_NONE);
}

TEST(test_attributes, aaudio_usage_lowlat) {
    checkAttributesUsage(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}

TEST(test_attributes, aaudio_content_type_lowlat) {
    checkAttributesContentType(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}

TEST(test_attributes, aaudio_input_preset_lowlat) {
    checkAttributesInputPreset(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}

TEST(test_attributes, aaudio_allowed_capture_policy_lowlat) {
    checkAttributesAllowedCapturePolicy(AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
}
