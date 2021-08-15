/*
 * Copyright 2014, The Android Open Source Project
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

#ifndef ANDROID_AUDIO_AC3_FRAME_SCANNER_H
#define ANDROID_AUDIO_AC3_FRAME_SCANNER_H

#include <stdint.h>
#include <system/audio.h>
#include <audio_utils/spdif/FrameScanner.h>

namespace android {

#define AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES          3
#define AC3_NUM_FRAME_SIZE_TABLE_ENTRIES          38
#define AC3_PCM_FRAMES_PER_BLOCK                 256
#define AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK        6
#define EAC3_RATE_MULTIPLIER                       4
#define EAC3_NUM_SAMPLE_RATE_TABLE_ENTRIES         3
#define EAC3_NUM_BLOCKS_PER_FRAME_TABLE_ENTRIES   38
#define EAC3_MAX_SUBSTREAMS                        8

class AC3FrameScanner : public FrameScanner
{
public:
    explicit AC3FrameScanner(audio_format_t format);
    virtual ~AC3FrameScanner();

    virtual int getMaxChannels()   const { return 5 + 1; } // 5.1 surround

    virtual int getMaxSampleFramesPerSyncFrame() const { return EAC3_RATE_MULTIPLIER
            * AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK * AC3_PCM_FRAMES_PER_BLOCK; }
    virtual int getSampleFramesPerSyncFrame() const;

    virtual bool isFirstInBurst();
    virtual bool isLastInBurst();
    virtual void resetBurst();

    virtual uint16_t convertBytesToLengthCode(uint16_t numBytes) const;

protected:
    // Keep track of how many of each substream blocks have been accumulated.
    // We need all of each substream before sending block data burst.
    uint8_t   mSubstreamBlockCounts[EAC3_MAX_SUBSTREAMS];
    int       mAudioBlocksPerSyncFrame;
    // The type of EAC3 stream as per EAC3 spec paragraph 2.3.1.1
    uint32_t  mStreamType;
    // substream index
    uint32_t  mSubstreamID;
    audio_format_t mFormat;

    // used to recognize the start of an AC3 sync frame
    static const uint8_t  kSyncBytes[];
    // sample rates from AC3 spec table 5.1
    static const uint16_t kAC3SampleRateTable[AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES];
    // frame sizes from AC3 spec table 5.13
    static const uint16_t kAC3FrameSizeTable[AC3_NUM_FRAME_SIZE_TABLE_ENTRIES]
            [AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES];
    // sample rates from EAC3 spec table E2.3
    static const uint16_t kEAC3ReducedSampleRateTable[AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES];
    // audio blocks per frame from EAC3 spec table E2.4
    static const uint16_t kEAC3BlocksPerFrameTable[EAC3_NUM_BLOCKS_PER_FRAME_TABLE_ENTRIES];

    virtual bool parseHeader();
};

}  // namespace android

#endif  // ANDROID_AUDIO_AC3_FRAME_SCANNER_H
