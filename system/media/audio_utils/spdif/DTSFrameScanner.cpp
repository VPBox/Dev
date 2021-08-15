/*
 * Copyright 2015, The Android Open Source Project
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
//#define LOG_NDEBUG 0

#include <assert.h>
#include <string.h>

#include <log/log.h>
#include <audio_utils/spdif/FrameScanner.h>

#include "BitFieldParser.h"
#include "DTSFrameScanner.h"

namespace android {

// TODO Handle termination frames.
// TODO assert if parse past end of header buffer
// TODO Handle DTS_HD

const uint8_t DTSFrameScanner::kSyncBytes[] =
        { 0x7F, 0xFE, 0x80, 0x01 };

const int32_t DTSFrameScanner::kDTSSampleRateTable[DTS_NUM_SAMPLE_RATE_TABLE_ENTRIES]
        = { -1, 8000, 16000, 32000, -1, -1,
        11025, 22050, 44100, -1, -1, 12000, 24000, 48000, -1, -1 };

// Defined in IEC61937-2
#define IEC61937_DATA_TYPE_DTS_I        11
#define IEC61937_DATA_TYPE_DTS_II       12
#define IEC61937_DATA_TYPE_DTS_III      13
#define IEC61937_DATA_TYPE_DTS_IV       17

#define IEC61937_MAX_SAMPLES_TYPE_I    512
#define IEC61937_MAX_SAMPLES_TYPE_II  1024
#define IEC61937_MAX_SAMPLES_TYPE_III 2048

// Limits defined in DTS spec paragraph 5.3.1
#define DTS_MINIMUM_NBLKS                5
#define DTS_MINIMUM_FSIZE               95

#define DTS_HEADER_BYTES_NEEDED         12

// Scanner for DTS byte streams.
DTSFrameScanner::DTSFrameScanner()
 : FrameScanner(IEC61937_DATA_TYPE_DTS_I,
    DTSFrameScanner::kSyncBytes,
    sizeof(DTSFrameScanner::kSyncBytes),
    DTS_HEADER_BYTES_NEEDED)
 , mSampleFramesPerSyncFrame(0)
{
}

DTSFrameScanner::~DTSFrameScanner()
{
}

// Parse DTS header.
// Detect whether the stream is DTS or DTS_HD. Extract data depending on type.
// Sets mDataType, mFrameSizeBytes,
//      mSampleRate, mRateMultiplier, mLengthCode.
//
// @return true if valid
bool DTSFrameScanner::parseHeader()
{
    BitFieldParser parser(&mHeaderBuffer[mSyncLength]);

    // These variables are named after the fields in the DTS spec 5.3.1
    // Extract field in order.
    (void) /* uint32_t ftype = */ parser.readBits(1);
    (void) /* uint32_t deficit = */ parser.readBits(5); // "short"
    uint32_t cpf = parser.readBits(1);
    uint32_t nblks = parser.readBits(7);
    uint32_t fsize = parser.readBits(14);
    (void) /* uint32_t amode = */ parser.readBits(6);
    uint32_t sfreq = parser.readBits(4);
    // make sure we did not read past collected data
    ALOG_ASSERT((mSyncLength + ((parser.getBitCursor() + 7) >> 3))
            <= mHeaderLength);

    // Validate fields.
    if (cpf != 0) {
        ALOGE("DTSFrameScanner: ERROR - CPF not zero!");
        return false;
    }
    if (nblks < DTS_MINIMUM_NBLKS) {
        ALOGE("DTSFrameScanner: ERROR - nblks = %u", nblks);
        return false;
    }
    if (fsize < DTS_MINIMUM_FSIZE) {
        ALOGE("DTSFrameScanner: ERROR - fsize = %u", fsize);
        return false;
    }

    int32_t sampleRate = kDTSSampleRateTable[sfreq];
    if (sampleRate < 0) {
        ALOGE("DTSFrameScanner: ERROR - invalid sampleRate[%u] = %d", sfreq, sampleRate);
        return false;
    }
    mSampleRate = (uint32_t) sampleRate;

    mSampleFramesPerSyncFrame = (nblks + 1) * DTS_PCM_FRAMES_PER_BLOCK;
    if (mSampleFramesPerSyncFrame <= IEC61937_MAX_SAMPLES_TYPE_I) {
        mDataType = IEC61937_DATA_TYPE_DTS_I;
    } else if (mSampleFramesPerSyncFrame <= IEC61937_MAX_SAMPLES_TYPE_II) {
        mDataType = IEC61937_DATA_TYPE_DTS_II;
    } else if (mSampleFramesPerSyncFrame <= IEC61937_MAX_SAMPLES_TYPE_III) {
        mDataType = IEC61937_DATA_TYPE_DTS_III;
    } else {
        mDataType = IEC61937_DATA_TYPE_DTS_IV;
        // TODO set bits 8,10
    }

    mFrameSizeBytes = fsize + 1;

    mRateMultiplier = 1; // TODO what about "frequency extension"?
    ALOGI_IF((mFormatDumpCount == 0),
            "DTS frame rate = %d * %d, size = %zu",
            mSampleRate, mRateMultiplier, mFrameSizeBytes);
    mFormatDumpCount++;
    return true;
}


}  // namespace android
