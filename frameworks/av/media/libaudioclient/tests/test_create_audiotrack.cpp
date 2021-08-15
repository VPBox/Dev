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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <binder/MemoryBase.h>
#include <binder/MemoryDealer.h>
#include <binder/MemoryHeapBase.h>
#include <media/AudioTrack.h>

#include "test_create_utils.h"

#define NUM_ARGUMENTS 10
#define VERSION_VALUE "1.0"

namespace android {

int testTrack(FILE *inputFile, int outputFileFd)
{
    char line[MAX_INPUT_FILE_LINE_LENGTH];
    uint32_t testCount = 0;
    Vector<String16> args;
    int ret = 0;

    if (inputFile == nullptr) {
        sp<AudioTrack> track = new AudioTrack(AUDIO_STREAM_DEFAULT,
                                              0 /* sampleRate */,
                                              AUDIO_FORMAT_DEFAULT,
                                              AUDIO_CHANNEL_OUT_STEREO);
        if (track == 0 || track->initCheck() != NO_ERROR) {
            write(outputFileFd, "Error creating AudioTrack\n",
                  sizeof("Error creating AudioTrack\n"));
        } else {
            track->dump(outputFileFd, args);
        }
        return 0;
    }

    // check version
    if (!checkVersion(inputFile, VERSION_VALUE)) {
        return 1;
    }

    while (readLine(inputFile, line, MAX_INPUT_FILE_LINE_LENGTH) == 0) {
        uint32_t sampleRate;
        audio_format_t format;
        audio_channel_mask_t channelMask;
        size_t frameCount;
        int32_t notificationFrames;
        uint32_t useSharedBuffer;
        audio_output_flags_t flags;
        audio_session_t sessionId;
        audio_usage_t usage;
        audio_content_type_t contentType;
        audio_attributes_t attributes;
        sp<IMemory> sharedBuffer;
        sp<MemoryDealer> heap;
        audio_offload_info_t offloadInfo = AUDIO_INFO_INITIALIZER;
        status_t status;
        char statusStr[MAX_OUTPUT_FILE_LINE_LENGTH];
        bool offload = false;
        bool fast = false;

        if (sscanf(line, " %u %x %x %zu %d %u %x %u %u %u",
                   &sampleRate, &format, &channelMask,
                   &frameCount, &notificationFrames, &useSharedBuffer,
                   &flags, &sessionId, &usage, &contentType) != NUM_ARGUMENTS) {
            fprintf(stderr, "Malformed line for test #%u in input file\n", testCount+1);
            ret = 1;
            continue;
        }
        testCount++;

        if (useSharedBuffer != 0) {
            size_t heapSize = audio_channel_count_from_out_mask(channelMask) *
                    audio_bytes_per_sample(format) * frameCount;
            heap = new MemoryDealer(heapSize, "AudioTrack Heap Base");
            sharedBuffer = heap->allocate(heapSize);
            frameCount = 0;
            notificationFrames = 0;
        }
        if ((flags & AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD) != 0) {
            offloadInfo.sample_rate = sampleRate;
            offloadInfo.channel_mask = channelMask;
            offloadInfo.format = format;
            offload = true;
        }
        if ((flags & AUDIO_OUTPUT_FLAG_FAST) != 0) {
            fast = true;
        }

        memset(&attributes, 0, sizeof(attributes));
        attributes.content_type = contentType;
        attributes.usage = usage;

        sp<AudioTrack> track = new AudioTrack();

        track->set(AUDIO_STREAM_DEFAULT,
                   sampleRate,
                   format,
                   channelMask,
                   frameCount,
                   flags,
                   (fast || offload) ? callback : nullptr,
                   nullptr,
                   notificationFrames,
                   sharedBuffer,
                   false,
                   sessionId,
                   ((fast && sharedBuffer == 0) || offload) ?
                           AudioTrack::TRANSFER_CALLBACK : AudioTrack::TRANSFER_DEFAULT,
                   offload ? &offloadInfo : nullptr,
                   getuid(),
                   getpid(),
                   &attributes,
                   false,
                   1.0f,
                   AUDIO_PORT_HANDLE_NONE);
        status = track->initCheck();
        sprintf(statusStr, "\n#### Test %u status %d\n", testCount, status);
        write(outputFileFd, statusStr, strlen(statusStr));
        if (status != NO_ERROR) {
            continue;
        }
        track->dump(outputFileFd, args);
    }
    return ret;
}

}; // namespace android


int main(int argc, char **argv)
{
    return android::main(argc, argv, android::testTrack);
}

