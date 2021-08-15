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
#include <media/AudioRecord.h>

#include "test_create_utils.h"

#define NUM_ARGUMENTS 8
#define VERSION_VALUE "1.0"
#define PACKAGE_NAME  "AudioRecord test"

namespace android {

int testRecord(FILE *inputFile, int outputFileFd)
{
    char line[MAX_INPUT_FILE_LINE_LENGTH];
    uint32_t testCount = 0;
    Vector<String16> args;
    int ret = 0;

    if (inputFile == nullptr) {
        sp<AudioRecord> record = new AudioRecord(AUDIO_SOURCE_DEFAULT,
                                              0 /* sampleRate */,
                                              AUDIO_FORMAT_DEFAULT,
                                              AUDIO_CHANNEL_IN_MONO,
                                              String16(PACKAGE_NAME));
        if (record == 0 || record->initCheck() != NO_ERROR) {
            write(outputFileFd, "Error creating AudioRecord\n",
                  sizeof("Error creating AudioRecord\n"));
        } else {
            record->dump(outputFileFd, args);
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
        audio_input_flags_t flags;
        audio_session_t sessionId;
        audio_source_t inputSource;
        audio_attributes_t attributes;
        status_t status;
        char statusStr[MAX_OUTPUT_FILE_LINE_LENGTH];
        bool fast = false;

        if (sscanf(line, " %u %x %x %zu %d %x %u %u",
                   &sampleRate, &format, &channelMask,
                   &frameCount, &notificationFrames,
                   &flags, &sessionId, &inputSource) != NUM_ARGUMENTS) {
            fprintf(stderr, "Malformed line for test #%u in input file\n", testCount+1);
            ret = 1;
            continue;
        }
        testCount++;

        if ((flags & AUDIO_INPUT_FLAG_FAST) != 0) {
            fast = true;
        }

        memset(&attributes, 0, sizeof(attributes));
        attributes.source = inputSource;

        sp<AudioRecord> record = new AudioRecord(String16(PACKAGE_NAME));

        record->set(AUDIO_SOURCE_DEFAULT,
                   sampleRate,
                   format,
                   channelMask,
                   frameCount,
                   fast ? callback : nullptr,
                   nullptr,
                   notificationFrames,
                   false,
                   sessionId,
                   fast ? AudioRecord::TRANSFER_CALLBACK : AudioRecord::TRANSFER_DEFAULT,
                   flags,
                   getuid(),
                   getpid(),
                   &attributes,
                   AUDIO_PORT_HANDLE_NONE);
        status = record->initCheck();
        sprintf(statusStr, "\n#### Test %u status %d\n", testCount, status);
        write(outputFileFd, statusStr, strlen(statusStr));
        if (status != NO_ERROR) {
            continue;
        }
        record->dump(outputFileFd, args);
    }
    return ret;
}

}; // namespace android


int main(int argc, char **argv)
{
    return android::main(argc, argv, android::testRecord);
}

