/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <gtest/gtest.h>

#define LOG_TAG "SysAudio_Test"
#include <log/log.h>
#include <media/PatchBuilder.h>
#include <system/audio.h>

using namespace android;

TEST(SystemAudioTest, PatchInvalid) {
    audio_patch patch{};
    ASSERT_FALSE(audio_patch_is_valid(&patch));
    patch.num_sources = AUDIO_PATCH_PORTS_MAX + 1;
    patch.num_sinks = 1;
    ASSERT_FALSE(audio_patch_is_valid(&patch));
    patch.num_sources = 1;
    patch.num_sinks = AUDIO_PATCH_PORTS_MAX + 1;
    ASSERT_FALSE(audio_patch_is_valid(&patch));
    patch.num_sources = 0;
    patch.num_sinks = 1;
    ASSERT_FALSE(audio_patch_is_valid(&patch));
}

TEST(SystemAudioTest, PatchValid) {
    const audio_port_config src = {
        .id = 1, .role = AUDIO_PORT_ROLE_SOURCE, .type = AUDIO_PORT_TYPE_DEVICE };
    // It's OK not to have sinks.
    ASSERT_TRUE(audio_patch_is_valid((PatchBuilder{}).addSource(src).patch()));
    const audio_port_config sink = {
        .id = 2, .role = AUDIO_PORT_ROLE_SINK, .type = AUDIO_PORT_TYPE_DEVICE };
    ASSERT_TRUE(audio_patch_is_valid((PatchBuilder{}).addSource(src).addSink(sink).patch()));
    ASSERT_TRUE(audio_patch_is_valid(
                    (PatchBuilder{}).addSource(src).addSource(src).addSink(sink).patch()));
    ASSERT_TRUE(audio_patch_is_valid(
                    (PatchBuilder{}).addSource(src).addSink(sink).addSink(sink).patch()));
    ASSERT_TRUE(audio_patch_is_valid(
                    (PatchBuilder{}).addSource(src).addSource(src).
                    addSink(sink).addSink(sink).patch()));
}

TEST(SystemAudioTest, PatchHwAvSync) {
    audio_port_config device_src_cfg = {
        .id = 1, .role = AUDIO_PORT_ROLE_SOURCE, .type = AUDIO_PORT_TYPE_DEVICE };
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&device_src_cfg));
    device_src_cfg.config_mask |= AUDIO_PORT_CONFIG_FLAGS;
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&device_src_cfg));
    device_src_cfg.flags.input = AUDIO_INPUT_FLAG_HW_AV_SYNC;
    ASSERT_TRUE(audio_port_config_has_hw_av_sync(&device_src_cfg));

    audio_port_config device_sink_cfg = {
        .id = 1, .role = AUDIO_PORT_ROLE_SINK, .type = AUDIO_PORT_TYPE_DEVICE };
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&device_sink_cfg));
    device_sink_cfg.config_mask |= AUDIO_PORT_CONFIG_FLAGS;
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&device_sink_cfg));
    device_sink_cfg.flags.output = AUDIO_OUTPUT_FLAG_HW_AV_SYNC;
    ASSERT_TRUE(audio_port_config_has_hw_av_sync(&device_sink_cfg));

    audio_port_config mix_sink_cfg = {
        .id = 1, .role = AUDIO_PORT_ROLE_SINK, .type = AUDIO_PORT_TYPE_MIX };
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&mix_sink_cfg));
    mix_sink_cfg.config_mask |= AUDIO_PORT_CONFIG_FLAGS;
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&mix_sink_cfg));
    mix_sink_cfg.flags.input = AUDIO_INPUT_FLAG_HW_AV_SYNC;
    ASSERT_TRUE(audio_port_config_has_hw_av_sync(&mix_sink_cfg));

    audio_port_config mix_src_cfg = {
        .id = 1, .role = AUDIO_PORT_ROLE_SOURCE, .type = AUDIO_PORT_TYPE_MIX };
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&mix_src_cfg));
    mix_src_cfg.config_mask |= AUDIO_PORT_CONFIG_FLAGS;
    ASSERT_FALSE(audio_port_config_has_hw_av_sync(&mix_src_cfg));
    mix_src_cfg.flags.output = AUDIO_OUTPUT_FLAG_HW_AV_SYNC;
    ASSERT_TRUE(audio_port_config_has_hw_av_sync(&mix_src_cfg));
}

TEST(SystemAudioTest, PatchEqual) {
    const audio_patch patch1{}, patch2{};
    // Invalid patches are not equal.
    ASSERT_FALSE(audio_patches_are_equal(&patch1, &patch2));
    const audio_port_config src = {
        .id = 1, .role = AUDIO_PORT_ROLE_SOURCE, .type = AUDIO_PORT_TYPE_DEVICE };
    const audio_port_config sink = {
        .id = 2, .role = AUDIO_PORT_ROLE_SINK, .type = AUDIO_PORT_TYPE_DEVICE };
    ASSERT_FALSE(audio_patches_are_equal(
                    (PatchBuilder{}).addSource(src).patch(),
                    (PatchBuilder{}).addSource(src).addSink(sink).patch()));
    ASSERT_TRUE(audio_patches_are_equal(
                    (PatchBuilder{}).addSource(src).addSink(sink).patch(),
                    (PatchBuilder{}).addSource(src).addSink(sink).patch()));
    ASSERT_FALSE(audio_patches_are_equal(
                    (PatchBuilder{}).addSource(src).addSink(sink).patch(),
                    (PatchBuilder{}).addSource(src).addSource(src).addSink(sink).patch()));
    audio_port_config sink_hw_av_sync = sink;
    sink_hw_av_sync.config_mask |= AUDIO_PORT_CONFIG_FLAGS;
    sink_hw_av_sync.flags.output = AUDIO_OUTPUT_FLAG_HW_AV_SYNC;
    ASSERT_FALSE(audio_patches_are_equal(
                    (PatchBuilder{}).addSource(src).addSink(sink).patch(),
                    (PatchBuilder{}).addSource(src).addSink(sink_hw_av_sync).patch()));
    ASSERT_TRUE(audio_patches_are_equal(
                    (PatchBuilder{}).addSource(src).addSink(sink_hw_av_sync).patch(),
                    (PatchBuilder{}).addSource(src).addSink(sink_hw_av_sync).patch()));
}
