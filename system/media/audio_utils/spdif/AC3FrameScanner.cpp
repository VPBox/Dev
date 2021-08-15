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

#define LOG_TAG "AudioSPDIF"

#include <string.h>

#include <log/log.h>
#include <audio_utils/spdif/FrameScanner.h>

#include "AC3FrameScanner.h"

namespace android {

// These values are from the AC3 spec. Do not change them.

const uint8_t AC3FrameScanner::kSyncBytes[] = { 0x0B, 0x77 };

const uint16_t AC3FrameScanner::kAC3SampleRateTable[AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES]
    = { 48000, 44100, 32000 };

// Table contains number of 16-bit words in an AC3 frame.
// From AC3 spec table 5.13
const uint16_t AC3FrameScanner::kAC3FrameSizeTable[AC3_NUM_FRAME_SIZE_TABLE_ENTRIES]
        [AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES] = {
    { 64, 69, 96 },
    { 64, 70, 96 },
    { 80, 87, 120 },
    { 80, 88, 120 },
    { 96, 104, 144 },
    { 96, 105, 144 },
    { 112, 121, 168 },
    { 112, 122, 168 },
    { 128, 139, 192 },
    { 128, 140, 192 },
    { 160, 174, 240 },
    { 160, 175, 240 },
    { 192, 208, 288 },
    { 192, 209, 288 },
    { 224, 243, 336 },
    { 224, 244, 336 },
    { 256, 278, 384 },
    { 256, 279, 384 },
    { 320, 348, 480 },
    { 320, 349, 480 },
    { 384, 417, 576 },
    { 384, 418, 576 },
    { 448, 487, 672 },
    { 448, 488, 672 },
    { 512, 557, 768 },
    { 512, 558, 768 },
    { 640, 696, 960 },
    { 640, 697, 960 },
    { 768, 835, 1152 },
    { 768, 836, 1152 },
    { 896, 975, 1344 },
    { 896, 976, 1344 },
    { 1024, 1114, 1536 },
    { 1024, 1115, 1536 },
    { 1152, 1253, 1728 },
    { 1152, 1254, 1728 },
    { 1280, 1393, 1920 },
    { 1280, 1394, 1920 }
};

const uint16_t AC3FrameScanner::kEAC3ReducedSampleRateTable[AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES]
        = { 24000, 22050, 16000 };

const uint16_t
        AC3FrameScanner::kEAC3BlocksPerFrameTable[EAC3_NUM_BLOCKS_PER_FRAME_TABLE_ENTRIES]
        = { 1, 2, 3, 6 };

// Defined in IEC61937-2
#define SPDIF_DATA_TYPE_AC3     1
#define SPDIF_DATA_TYPE_E_AC3  21
#define AC3_STREAM_TYPE_0       0
#define AC3_STREAM_TYPE_1       1
#define AC3_STREAM_TYPE_2       2
// -----------------------------------------------------------------------------

// Scanner for AC3 byte streams.
AC3FrameScanner::AC3FrameScanner(audio_format_t format)
 : FrameScanner(SPDIF_DATA_TYPE_AC3,
        AC3FrameScanner::kSyncBytes,
        sizeof(AC3FrameScanner::kSyncBytes), 6)
 , mStreamType(0)
 , mSubstreamID(0)
 , mFormat(format)
{
    mAudioBlocksPerSyncFrame = 6;
    memset(mSubstreamBlockCounts, 0, sizeof(mSubstreamBlockCounts));
}

AC3FrameScanner::~AC3FrameScanner()
{
}

int AC3FrameScanner::getSampleFramesPerSyncFrame() const
{
    return mRateMultiplier
            * AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK * AC3_PCM_FRAMES_PER_BLOCK;
}

void AC3FrameScanner::resetBurst()
{
    for (int i = 0; i < EAC3_MAX_SUBSTREAMS; i++) {
        if (mSubstreamBlockCounts[i] >= AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK) {
            mSubstreamBlockCounts[i] -= AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK;
        } else if (mSubstreamBlockCounts[i] > 0) {
            ALOGW("EAC3 substream[%d] has only %d audio blocks!",
                i, mSubstreamBlockCounts[i]);
            mSubstreamBlockCounts[i] = 0;
        }
    }
}

// Per IEC 61973-3:5.3.3, for E-AC3 burst-length shall be in bytes.
uint16_t AC3FrameScanner::convertBytesToLengthCode(uint16_t numBytes) const
{
    return (mDataType == SPDIF_DATA_TYPE_E_AC3) ? numBytes : numBytes * 8;
}

// per IEC 61973-3 Paragraph 5.3.3
// We have to send 6 audio blocks on all active substreams.
// Substream zero must be the first.
// We don't know if we have all the blocks we need until we see
// the 7th block of substream#0.
bool AC3FrameScanner::isFirstInBurst()
{
    if (mDataType == SPDIF_DATA_TYPE_E_AC3) {
        if (((mStreamType == AC3_STREAM_TYPE_0)
                || (mStreamType == AC3_STREAM_TYPE_2))
                && (mSubstreamID == 0)
                // The ">" is intentional. We have to see the beginning
                // of the block in the next burst before we can send
                // the current burst.
                && (mSubstreamBlockCounts[0] > AC3_MAX_BLOCKS_PER_SYNC_FRAME_BLOCK)) {
            return true;
        }
    }
    return false;
}

bool AC3FrameScanner::isLastInBurst()
{
    // For EAC3 we don't know if we are the end until we see a
    // frame that must be at the beginning. See isFirstInBurst().
    return (mDataType != SPDIF_DATA_TYPE_E_AC3); // Just one AC3 frame per burst.
}

// TODO Use BitFieldParser

// Parse AC3 header.
// Detect whether the stream is AC3 or EAC3. Extract data depending on type.
//
// @return true if valid
bool AC3FrameScanner::parseHeader()
{
    // Interpret bsid based on paragraph E2.3.1.6 of EAC3 spec.
    uint32_t bsid = mHeaderBuffer[5] >> 3; // bitstream ID
    // Check BSID to see if this is EAC3 or regular AC3.
    // These arbitrary BSID numbers do not have any names in the spec.
    if ((bsid > 10) && (bsid <= 16)) {
        mDataType = SPDIF_DATA_TYPE_E_AC3;
    } else if (bsid <= 8) {
        mDataType = SPDIF_DATA_TYPE_AC3;
    } else {
        ALOGW("AC3 bsid = %d not supported", bsid);
        return false;
    }

    // bitstream mode, main, commentary, etc.
    uint32_t bsmod = mHeaderBuffer[5] & 7;
    mDataTypeInfo = bsmod; // as per IEC61937-3, table 3.

    // The names fscod, frmsiz are from the AC3 spec.
    uint32_t fscod = mHeaderBuffer[4] >> 6;
    if (mDataType == SPDIF_DATA_TYPE_E_AC3) {
        mStreamType = mHeaderBuffer[2] >> 6; // strmtyp in spec
        mSubstreamID = (mHeaderBuffer[2] >> 3) & 0x07;

        // Frame size is explicit in EAC3. Paragraph E2.3.1.3
        uint32_t frmsiz = ((mHeaderBuffer[2] & 0x07) << 8) + mHeaderBuffer[3];
        mFrameSizeBytes = (frmsiz + 1) * sizeof(int16_t);

        uint32_t numblkscod = 3; // 6 blocks default
        if (fscod == 3) {
            uint32_t fscod2 = (mHeaderBuffer[4] >> 4) & 0x03;
            if (fscod2 >= AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES) {
                ALOGW("Invalid EAC3 fscod2 = %d", fscod2);
                return false;
            } else {
                mSampleRate = kEAC3ReducedSampleRateTable[fscod2];
            }
        } else {
            mSampleRate = kAC3SampleRateTable[fscod];
            numblkscod = (mHeaderBuffer[4] >> 4) & 0x03;
        }
        mRateMultiplier = EAC3_RATE_MULTIPLIER; // per IEC 61973-3 Paragraph 5.3.3
        // Don't send data burst until we have 6 blocks per substream.
        mAudioBlocksPerSyncFrame = kEAC3BlocksPerFrameTable[numblkscod];
        // Keep track of how many audio blocks we have for each substream.
        // This should be safe because mSubstreamID is ANDed with 0x07 above.
        // And the array is allocated as [8].
        if ((mStreamType == AC3_STREAM_TYPE_0)
                || (mStreamType == AC3_STREAM_TYPE_2)) {
            mSubstreamBlockCounts[mSubstreamID] += mAudioBlocksPerSyncFrame;
        }

        // Print enough so we can see all the substreams.
        ALOGD_IF((mFormatDumpCount < 3*8 ),
                "EAC3 mStreamType = %d, mSubstreamID = %d",
                mStreamType, mSubstreamID);
    } else { // regular AC3
        // Extract sample rate and frame size from codes.
        uint32_t frmsizcod = mHeaderBuffer[4] & 0x3F; // frame size code

        if (fscod >= AC3_NUM_SAMPLE_RATE_TABLE_ENTRIES) {
            ALOGW("Invalid AC3 sampleRateCode = %d", fscod);
            return false;
        } else if (frmsizcod >= AC3_NUM_FRAME_SIZE_TABLE_ENTRIES) {
            ALOGW("Invalid AC3 frameSizeCode = %d", frmsizcod);
            return false;
        } else {
            mSampleRate = kAC3SampleRateTable[fscod];
            mRateMultiplier = 1;
            mFrameSizeBytes = sizeof(uint16_t)
                    * kAC3FrameSizeTable[frmsizcod][fscod];
        }
        mAudioBlocksPerSyncFrame = 6;
        if (mFormat == AUDIO_FORMAT_E_AC3) {
            ALOGV("Its a Ac3 substream in EAC3 stream");
            mStreamType = 2;
            mSubstreamID = 0;
            mSubstreamBlockCounts[0] += mAudioBlocksPerSyncFrame;
            mDataType = SPDIF_DATA_TYPE_E_AC3;
            mRateMultiplier = EAC3_RATE_MULTIPLIER;
        }
    }
    ALOGI_IF((mFormatDumpCount == 0),
            "AC3 frame rate = %d * %d, size = %zu, audioBlocksPerSyncFrame = %d",
            mSampleRate, mRateMultiplier, mFrameSizeBytes, mAudioBlocksPerSyncFrame);
    mFormatDumpCount++;
    return true;
}

}  // namespace android
